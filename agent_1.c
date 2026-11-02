#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

void print_banner() {
    printf("\033[1;32m[iAgent v0.2] - Bare-Metal C Native Agent (Real Task Executor)\033[0m\n");
    printf("Type 'help' for commands, 'runtask' to process tasks, or 'exit' to quit.\n\n");
}

void process_tasks() {
    FILE *f1 = fopen("task1.txt", "r");
    if (!f1) {
        printf("[-] Error: Could not open 'task1.txt'. Make sure the file exists.\n");
        return;
    }

    FILE *f2 = fopen("task2.txt", "w");
    if (!f2) {
        printf("[-] Error: Could not create 'task2.txt'.\n");
        fclose(f1);
        return;
    }

    char line[MAX_INPUT];
    int count = 0;
    
    fprintf(f2, "--- iAgent Task Execution Report ---\n");
    while (fgets(line, sizeof(line), f1)) {
        // حذف کاراکتر newline انتهای خط
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        printf("[*] Executing real system command: %s\n", line);
        
        // اجرای واقعی دستور روی سیستم‌عامل
        int ret = system(line);
        
        if (ret == 0) {
            fprintf(f2, "Command: %s -> Status: Success\n", line);
        } else {
            fprintf(f2, "Command: %s -> Status: Failed (Code %d)\n", line, ret);
        }
        count++;
    }

    fclose(f1);
    fclose(f2);
    printf("[+] Successfully processed %d tasks from 'task1.txt' and wrote reports to 'task2.txt'.\n", count);
}

int main() {
    char input[MAX_INPUT];
    print_banner();

    while (1) {
        printf("\033[1;34miagent> \033[0m");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Agent shutting down. Goodbye!\n");
            break;
        } else if (strcmp(input, "help") == 0) {
            printf("Available commands:\n");
            printf("  hello      - Greet the user\n");
            printf("  sysinfo    - Show system environment status\n");
            printf("  runtask    - Read commands from task1.txt and execute them\n");
            printf("  exit       - Exit the agent\n");
        } else if (strcmp(input, "hello") == 0) {
            printf("سلام! ایجنت آماده‌ی اجرای دستورات سیستمی است.\n");
        } else if (strcmp(input, "runtask") == 0) {
            process_tasks();
        } else if (strcmp(input, "sysinfo") == 0) {
            printf("[*] Environment: Termux / Linux Bare-Metal\n[*] Core: C11 Real Execution Engine\n");
        } else if (strlen(input) > 0) {
            printf("Unknown command: '%s'. Type 'help' for options.\n", input);
        }
    }

    return 0;
}
