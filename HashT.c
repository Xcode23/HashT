#include <stdlib.h>
#include "HashT.h"

#define STARTINGSIZE 16
#define LOADFACTOR 0.7


typedef struct _node_{
  void* key;
  void* value;
  struct _node_* next;
}node;

struct _hashtable_{
  int (*hashcode)(void*);
  int (*equals)(void*,void*);
  int size;
  int used;
  node** table;
};

htable* newTable(int (*hashfunc)(void*), int (*equalsfunc)(void*,void*)){
  htable* newtable;
  if(!(newtable=(htable*)malloc(sizeof(htable))))
    return MEM_ERROR;
  newtable->hashcode=hashfunc;
  newtable->equals=equalsfunc;
  newtable->size=STARTINGSIZE;
  newtable->used=0;
  if(!(newtable->table=(node**)malloc(newtable->size*sizeof(node*))))
    return MEM_ERROR;
  return newtable;
}

int insert(htable* hashtable, void* key, void* value){
  int location=hashtable->hashcode(key);
  node* auxnode;
  if(location<0)return KEY_TYPE_ERROR;
  location=location%hashtable->size;
  node* newnode=(node*)malloc(sizeof(node));
  newnode->key=key;
  newnode->value=value;
  if(hashtable->table[location]==NULL)
    hashtable->table[location]=newnode;
  else{
    auxnode=hashtable->table[location];
    while(auxnode->next)auxnode=auxnode->next;
    auxnode->next=newnode;
  }
  return OK;
}
