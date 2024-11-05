#include <stdio.h>
#include <string.h>

void removeNewLine(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int execCmd(char *cmd) {
    removeNewLine(cmd);
    if(strcmp("exit 0", cmd) == 0) {
      return -1;
    } 
    printf("%s: command not found\n", cmd);
    return 0;
}

int main() {
  char input[100];
  for(;;) {
    // Uncomment this block to pass the first stage
    printf("$ ");
    fflush(stdout);
    // Wait for user input
    fgets(input, 100, stdin);
    int ret = execCmd(input);
    if(ret == -1) {
      return 0;
    }
  }
  return 0;
}

