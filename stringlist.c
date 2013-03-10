#include <stdlib.h>
#include <string.h>

#include "stringlist.h"

struct stringlist {
  struct stringlist* next;
  char* data;
};
/* typedef struct stringlist stringlist; */

void slst_append(struct stringlist* slst, char* str){
  while(slst->next){
    slst=slst->next;
  }
  stringlist* new_item=slst->next=malloc(sizeof(stringlist));
  new_item->next=0;
  new_item->data=str;
}

stringlist* slst_create_with_tail(struct stringlist* slst, char* str){
  stringlist* res=malloc(sizeof(stringlist));
  res->next=slst;
  res->data=str;
  return res;
}

stringlist* slst_create(char* str){
  return slst_create_with_tail(0,str);
}

void slst_free(struct stringlist* slst){
  if(slst!=NULL){
    slst_free(slst->next);
    free(slst);
  }
}

void slst_pop_head(stringlist** slst){
  stringlist* tmp=*slst;
  *slst=slst_tail(*slst);
  free(tmp);
}

stringlist* slst_tail(struct stringlist* slst){
  return slst->next;
}

char* slst_head(struct stringlist* slst){
  return slst->data;
}

int slst_size(stringlist* slst) {
  int size = 0;
  while(slst) {
    size++;
    slst = slst_tail(slst);
  }
  return size;
}

char** slst_to_charpp(stringlist* slst) {
  char** charpp = malloc((slst_size(slst) + 1) * sizeof(char*));
  int i = 0;
  while(slst) {
    charpp[i++] = slst->data;
    slst = slst_tail(slst);
  }
  charpp[i] = NULL;
  return charpp;
}

void slst_free_with_data(stringlist* slst){
  if(slst!=NULL){
    slst_free_with_data(slst->next);
    free(slst->data);
    free(slst);
  }
}
