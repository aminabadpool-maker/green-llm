#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 256

void print_banner() {
    printf("\033[1;32m[iAgent v0.1] - Bare-Metal C Native Agent\033[0m\n");
    printf("Type 'help' for commands or 'exit' to quit.\n\n");
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
            printf("  makec      - Create a boilerplate C file\n");
            printf("  sysinfo    - Show system environment status\n");
            printf("  exit       - Exit the agent\n");
        } else if (strcmp(input, "hello") == 0) {
            printf("سلام! من ایجنت بومیِ شما هستم. آماده‌ام تا فرامینِ سیستم‌دولوپری را اجرا کنم.\n");
        } else if (strcmp(input, "makec") == 0) {
            FILE *f = fopen("main.c", "w");
            if (f) {
                fprintf(f, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello from iAgent generated C file!\\n\");\n    return 0;\n}\n");
                fclose(f);
                printf("[+] Successfully generated 'main.c'!\n");
            } else {
                printf("[-] Error creating file.\n");
            }
        } else if (strcmp(input, "sysinfo") == 0) {
            printf("[*] Environment: Termux / Linux Bare-Metal\n[*] Core Language: C11 Standard\n");
        } else if (strlen(input) > 0) {
            printf("Unknown command: '%s'. Type 'help' for options.\n", input);
        }
    }

    return 0;
}
