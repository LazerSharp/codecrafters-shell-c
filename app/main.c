#include <stdio.h>
#include <string.h>

int main() {
  char input[100];
  for(;;) {
    // Uncomment this block to pass the first stage
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = 0;
    printf("%s: command not found\n", input);
  }
  return 0;
}
