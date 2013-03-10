#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h> // needed for O_EXCL, O_CREAT, open (?)
#include <sys/wait.h>
#include <sys/stat.h>

#include "stringlist.h"

enum PARSE_ERROR {
  PARSE_OK=0,
  PARSE_MISSING_SPACE,
  PARSE_UNMATCHED_QUOTE,
};

int parse_command(char *command, stringlist** result) {
  int i;
  int inside_quote = 0;
  int inside_word = 0;
  
  stringlist* back;
  *result=back=slst_create(0);
  
  for(i = 0; command[i] != '\0'; i++) {
    if(command[i] == '"'){
      if(inside_quote){
        if( !( isspace(command[i+1]) || command[i+1]=='\0') ){
          return PARSE_MISSING_SPACE;
        } else {
          inside_quote=0;
          command[i]='\0';
        }
      } else {
        if(inside_word){
          return PARSE_MISSING_SPACE;
        } else {
          inside_quote=1;
          slst_append(back,&command[i+1]);
          back=slst_tail(back);
        } 
      }
    } else if(isspace(command[i]) ) {
      if(inside_word){
        command[i]='\0';
        inside_word=0;
      }
    } else {
      if( (!inside_word) && (!inside_quote) ){
        inside_word=1;
        slst_append(back,&command[i]);
        back=slst_tail(back);
      }
    } 
  }
  slst_pop_head(result);
  if(inside_quote) return PARSE_UNMATCHED_QUOTE;
  return PARSE_OK;
}

int executable_exists(const char* path) {
  struct stat s;
  if(stat(path,&s)!=0){
    return 0;
  } else {
    return (s.st_mode & S_IXOTH) // executable for other
       || (s.st_mode & S_IXGRP && s.st_gid==getegid())
       || (s.st_mode & S_IXUSR && s.st_uid==geteuid());
  }
}

char *drop_until_last_slash(char *path) {
  int k;
  int slash_index = -1;
  for(k = 0; path[k] != '\0'; k++) {
    if(path[k] == '/')
      slash_index = k;
  }
  return slash_index == -1 || slash_index + 1 == k
    ? path
    : &path[slash_index + 1];
}

const char* find_path(const char* program){
  static const char *mypaths[] = {
    "/bin/",
    "/usr/bin/",
    "./",
    NULL
  };
  static const int PATH_BUFFER_SIZE=128;
  
  if(program == NULL)
    return NULL;
  else if(program[0] == '/') {
    if(executable_exists(program)) return program;
    else return NULL;
  }
  const char **i=mypaths;
  while(*i){
    char path[PATH_BUFFER_SIZE]; // = "";
    path[0] = '\0'; 
    strncat(path,*i,PATH_BUFFER_SIZE-1);
    strncat(path,program,PATH_BUFFER_SIZE-strlen(path)-1);
    if(executable_exists(path)) return strdup(path);
    i++;    
  }
  return NULL;
}

int main(int argc, char **argv) {
  const char *prompt = "> ";
  const int COMMAND_SIZE = 1024;

  char command[COMMAND_SIZE];

  while(1) {
    /* Wait for input */
    printf("%s%s", drop_until_last_slash(argv[0]), prompt);
    fgets(command, COMMAND_SIZE, stdin);
    
    stringlist * args;

    switch(parse_command(command,&args)) {
    case PARSE_MISSING_SPACE:
      printf("error: missing space\n");
      continue;
    case PARSE_UNMATCHED_QUOTE:
      printf("error: unmatched quote\n");
      continue;
    }

    if(strcmp(slst_head(args),"exit")==0 ) break;
    
    pid_t child;
    const char* path = find_path(slst_head(args));
    if(!path) {
      printf("error: file not found or not executable\n");
    }
    /* Launch executable */
    char ** child_argv = slst_to_charpp(args);
    if ((child = fork ()) == 0) {
      execv(path, child_argv);
      //char* arg[2] = {"/bin/ls",NULL};
      //execv(NULL, arg);
      //printf("hey execv, errno: %d\n", errno);
    } else {
      waitpid(child, 0, 0);
    }
    slst_free(args);
  }
  return 0;
}
