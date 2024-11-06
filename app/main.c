#include <stdio.h>
#include <string.h>

void removeNewLine(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int startsWith(char *pre, char *str) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

char *builtins[] = {"type", "echo", "exit"}; 
void type(char *cmd) {
  int len = sizeof(builtins) / sizeof(builtins[0]);
  for (int i=0; i < len; i++) {
    if(strcmp("type", cmd) == 0) {
      printf("%s is a shell builtin\n", cmd);
      return;
    }
  }  
  printf("%s: command not found\n", cmd);
}

int execCmd(char *cmd) {
    removeNewLine(cmd);
    if(strcmp("exit 0", cmd) == 0) {
      return -1;
    }
    if(startsWith("echo ", cmd)) {
      puts(&cmd[5]);
      return 0;
    }
    if(startsWith("type ", cmd)) {
        type(&cmd[5]);
        return 0;
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

