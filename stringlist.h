#ifndef STRINGLIST_H
#define STRINGLIST_H

struct stringlist;
typedef struct stringlist stringlist;

/* if order is not important use create with tail instead */
/* might be O(n) */
/* str will be copied shallow, ie. might be modified at a later time. */
void slst_append(stringlist* slst, char* str);
stringlist* slst_create_with_tail(stringlist* tail, char* str);

stringlist* slst_create(char* str);


void slst_free(stringlist*);
void slst_pop_head(stringlist*);

stringlist* slst_tail(stringlist*);
const char* slst_head(stringlist*);

#endif
