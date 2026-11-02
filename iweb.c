#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

void clean_html(FILE *input, int code_only_mode, int smart_mode) {
    char read_buf[BUFFER_SIZE];
    char write_buf[BUFFER_SIZE];
    size_t write_idx = 0;
    
    int in_tag = 0;
    int in_script = 0;
    int in_style = 0;
    int in_title = 0;
    int in_code_block = 0;
    int in_md_code_block = 0;
    int last_was_newline = 0;

    size_t bytes_read;
    while ((bytes_read = fread(read_buf, 1, sizeof(read_buf), input)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            char c = read_buf[i];

            // تشخیص بلوک‌های کد Markdown (```)
            if (c == '`' && i + 2 < bytes_read && read_buf[i+1] == '`' && read_buf[i+2] == '`') {
                in_md_code_block = !in_md_code_block;
                i += 2;
                continue;
            }

            // مدیریت تگ‌های HTML
            if (c == '<') {
                in_tag = 1;
                if (i + 7 < bytes_read && strncasecmp(&read_buf[i], "<script", 7) == 0) {
                    in_script = 1;
                } else if (i + 6 < bytes_read && strncasecmp(&read_buf[i], "<style", 6) == 0) {
                    in_style = 1;
                } else if (i + 6 < bytes_read && strncasecmp(&read_buf[i], "<title", 6) == 0) {
                    in_title = 1;
                } else if (i + 4 < bytes_read && (strncasecmp(&read_buf[i], "<pre", 4) == 0 || strncasecmp(&read_buf[i], "<code", 5) == 0)) {
                    in_code_block = 1;
                }
                continue;
            }

            if (c == '>') {
                in_tag = 0;
                if (in_script && i >= 8 && strncasecmp(&read_buf[i-8], "</script>", 9) == 0) {
                    in_script = 0;
                } else if (in_style && i >= 7 && strncasecmp(&read_buf[i-7], "</style>", 8) == 0) {
                    in_style = 0;
                } else if (in_title && i >= 7 && strncasecmp(&read_buf[i-7], "</title>", 8) == 0) {
                    in_title = 0;
                }
                if (i >= 5 && strncasecmp(&read_buf[i-5], "</pre>", 6) == 0) in_code_block = 0;
                if (i >= 6 && strncasecmp(&read_buf[i-6], "</code>", 7) == 0) in_code_block = 0;
                continue;
            }

            // فیلتر کردن زوائد ساختاری اسکریپت و استایل
            if (in_script || in_style || in_title) {
                continue;
            }

            int keep = 0;
            if (code_only_mode) {
                if ((in_code_block && !in_tag) || in_md_code_block) {
                    keep = 1;
                }
            } else if (smart_mode) {
                // حالت هوشمند: حفظ ساختارهای متن بدون حذف تگ‌های عمومی به صورت سخت‌گیرانه
                if (!in_tag) {
                    keep = 1;
                }
            } else {
                // حالت پیش‌فرض: تخلیه کامل تگ‌های غیرضروری
                if (!in_tag) keep = 1;
            }

            if (keep) {
                // فشرده‌سازی خطوط خالی متوالی برای بهینه‌سازی توکن‌های ایجنت
                if (c == '\n') {
                    if (last_was_newline) continue;
                    last_was_newline = 1;
                } else {
                    last_was_newline = 0;
                }

                write_buf[write_idx++] = c;
                if (write_idx >= sizeof(write_buf) - 1) {
                    fwrite(write_buf, 1, write_idx, stdout);
                    write_idx = 0;
                }
            }
        }
    }
    
    // تخلیه نهایی بافر
    if (write_idx > 0) {
        fwrite(write_buf, 1, write_idx, stdout);
        fflush(stdout);
    }
}

int main(int argc, char *argv[]) {
    int code_only = 0;
    int smart_mode = 0;
    char *url = NULL;

    // تجزیه پارامترها و فلگ‌ها با ساختار ایمن
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--code") == 0) {
            code_only = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--smart") == 0) {
            smart_mode = 1;
        } else if (argv[i][0] != '-') {
            url = argv[i];
        }
    }

    if (url != NULL) {
        char command[2048];
        int ret = snprintf(command, sizeof(command), "curl -sL --connect-timeout 5 --max-time 15 \"%s\"", url);
        if (ret < 0 || (size_t)ret >= sizeof(command)) {
            fprintf(stderr, "Error: URL string too long or formatting failed.\n");
            return 1;
        }

        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            perror("Error: Failed to open stream via curl");
            return 1;
        }

        clean_html(fp, code_only, smart_mode);
        pclose(fp);
    } else {
        // خواندن مستقیم از استاندارد اینپوت (Pipe)
        clean_html(stdin, code_only, smart_mode);
    }

    return 0;
}
