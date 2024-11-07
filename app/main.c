#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void removeNewLine(char *str) {
  int len = strcspn(str, "\n");
  str[len] = 0;
}

int startsWith(char *pre, char *str) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trim(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


char * strdup2(char * str) {
  char* d = malloc(200);
  strcpy(d, str);
  return d;
}

char* searchExternalCmd(char *cmd) {
  char* pathVar = getenv("PATH");
  //if(strcmp("cp", cmd) == 0){
  //fprintf(stderr,"PATH: %s\n", pathVar);
  //}
  char *paths = strdup(pathVar);
  char* path;
  while((path = strsep(&paths, ":")) != NULL) {
    //fprintf(stderr,"while start - path: %s\n", path);
    char* p = strdup2(path);
    if (p == NULL) {
      //fprintf(stderr,"strdup err! \n");
    }
    //fprintf(stderr,"p: '%s'\n", p);
    p = trim(p);
    p = strcat(p, "/");
    p = strcat(p, cmd);
    //if(strcmp("cp", cmd) == 0){
    //fprintf(stderr,"p: '%s'\n", p);
    //}
    struct stat buf;
    if(stat(p, &buf) == 0 && (buf.st_mode & X_OK)) {
      return p;
    }
    free(p);
    //fprintf(stderr,"while end\n");
  }
  return NULL;
}

char *builtins[] = {"type", "echo", "exit"}; 
void type(char *cmd) {
  //fprintf(stderr,"type checking...\n");
  int len = sizeof(builtins) / sizeof(builtins[0]);
  // search builtins
  for (int i=0; i < len; i++) {
    //fprintf(stderr,"comparing.... [%s]\n", cmd);
    int cmp = (strcmp(builtins[i], cmd));
    //fprintf(stderr,"cmp = %d\n", cmp);
    if(cmp == 0) {
      printf("%s is a shell builtin\n", cmd);
      return;
    }
  }
  // search extternal commads in $PATH
  //fprintf(stderr,"searching in path...");
  char* loc;
  if((loc = searchExternalCmd(cmd)) != NULL) {
    printf("%s is %s\n",cmd, loc);
    return;
  }
  printf("%s: not found\n", cmd);
}

int execCmd(char *cmd) {
    //fprintf(stderr,"Exec cmd : %s", cmd);
    removeNewLine(cmd);
    //fprintf(stderr,"Exec cmd (NL removed): %s\n", cmd);
    if(strcmp("exit 0", cmd) == 0) {
      return -1;
    }
    //fprintf(stderr,"Exec cmd  (after exit) : %s\n", cmd);
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

