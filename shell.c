#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "stringlist.h"

enum PARSE_ERROR {
  PARSE_OK=0,
  PARSE_MISSING_SPACE,
  PARSE_UNMATCHED_QUOTE,
};

/* parse_word(char **command,stringlist* result){ */
/*   skipws; */
/*   if(*command=='"'){ */
/*     slst_append(result,&(*command)[1]); */
/*     skip_to_quote(); */
/*     **command='\0'; */
/*   } */
  

int parse_command(char *command, stringlist** result) {
  int i;
  int inside_quote = 0;
  int inside_word = 0;
  
  stringlist* back;
  *result=back=slst_create(0);
  
  for(i = 0; command[i] != '\0'; i++) {
    if(command[i] == '"'){
      if(inside_quote){
	if(command[++i]!=' '){
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
      if(!inside_word){
	inside_word=1;
	slst_append(back,command);
	back=slst_tail(back);
      }
    } 
  }
  slst_pop_head(*result);
  if(inside_quote) return PARSE_UNMATCHED_QUOTE;
  return PARSE_OK;
}

int file_exists(const char* path) {
  if(open(path,O_EXCL | O_CREAT) == -1) {
    return errno == EEXIST;
  } else {
    remove(path);
  }
  return 0;
}

char* find_path(char* program){
  static const char *mypaths[] = {
    "./",
    "/usr/bin/",
    "/bin/",
    NULL
  };
  static const int PATH_BUFFER_SIZE=128;
  
  if(program == NULL)
    return NULL;
  else if(program[0] == '/') {
    if(file_exists(program)) return program;
    else return NULL;
  }
  char **i=mypaths;
  while(*i){
    char path[PATH_BUFFER_SIZE]="";
    strncat(path,*i,PATH_BUFFER_SIZE-1);
    strncat(path,program,PATH_BUFFER_SIZE-strlen(path)-1);
    if(file_exists(path)) return path;
  }
  return NULL;
}

int main(int argc, char **argv) {
  const char *prompt = "> ";
  const int COMMAND_SIZE = 1024;

  char command[COMMAND_SIZE];
  int flag;

  while(1) {
    /* Wait for input */
    printf("%s%s", argv[0], prompt);
    fgets(command, COMMAND_SIZE, stdin);
    
    stringlist * args;
    flag = parse_command(command,&args);
    switch(flag) {
    case PARSE_MISSING_SPACE:
      printf("error: missing space\n");
      continue;
    case PARSE_UNMATCHED_QUOTE:
      printf("error: unmatched quote\n");
      continue;
    }

    
    /* /\* Launch executable *\/ */
    /* if (fork () == 0) { */

    /* } else { */
    /*   //wait (...); */
    /* } */
    slst_free(args);
  }
}
