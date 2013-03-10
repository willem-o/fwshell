#ifndef STRINGLIST_H
#define STRINGLIST_H


/* stringlist is a stack of strings, that will NOT
take ownership of the strings */
struct stringlist;
typedef struct stringlist stringlist;

/* if order is not important use create with tail instead */
/* might be O(n) */
/* str will be copied shallow, ie. might be modified at a later time. */
void slst_append(stringlist* slst, char* str);
stringlist* slst_create_with_tail(stringlist* tail, char* str);

stringlist* slst_create(char* str);

/* will not free() the strings inside.*/
void slst_free(stringlist*);

/* does free() the strings inside.*/
void slst_free_with_data(stringlist*);

void slst_pop_head(stringlist**);

stringlist* slst_tail(stringlist*);
char* slst_head(stringlist*);

int slst_size(stringlist*);

/* stringlist owns contained char*s but the array should be free()'d */
char** slst_to_charpp(stringlist*);

#endif
