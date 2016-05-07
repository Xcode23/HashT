#ifndef HASHT
#define HASHT

typedef struct _hashtable_ htable;

htable* newTable(int (*hashfunc)(void*), int (*equalsfunc)(void*,void*), void* (*clonekeyfunc)(void*), void* (*clonevaluefunc)(void*));
void* insert(htable* hashtable, void* key, void* value);

#endif
