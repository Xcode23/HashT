#ifndef HASHT
#define HASHT

#define MEM_ERROR NULL
#define KEY_TYPE_ERROR -1
#define OK 0

typedef struct _hashtable_ htable;

htable* newTable(int (*hashfunc)(void*), int (*equalsfunc)(void*,void*));

#endif
