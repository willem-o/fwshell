#include <stdlib.h>

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

/* will not free() strings inside.*/
void slst_free(struct stringlist* slst){
  while(slst_tail(slst)){
    slst_pop_head(slst);
  }
  free(slst);
}

void slst_pop_head(stringlist* slst){
  stringlist* tmp=slst;
  slst=slst->next;
  free(tmp);
}

stringlist* slst_tail(struct stringlist* slst){
  return slst->next;
}

const char* slst_head(struct stringlist* slst){
  return slst->data;
}
