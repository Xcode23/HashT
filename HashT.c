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
  void* (*clonekey)(void*);
  void* (*clonevalue)(void*);
  int size;
  int used;
  node** table;
};

htable* newTable(int (*hashfunc)(void*), int (*equalsfunc)(void*,void*), void* (*clonekeyfunc)(void*), void* (*clonevaluefunc)(void*)){
  htable* newtable;

  if(!(newtable=(htable*)malloc(sizeof(htable))))
    return NULL;

  newtable->hashcode=hashfunc;
  newtable->equals=equalsfunc;
  newtable->clonekey=clonekeyfunc;
  newtable->clonevalue=clonevaluefunc;
  newtable->size=STARTINGSIZE;
  newtable->used=0;
  if(!(newtable->table=(node**)malloc(newtable->size*sizeof(node*))))
    return NULL;

  return newtable;
}

void erasenode(node* oldnode){
  free(oldnode->key);
  free(oldnode->value);
  free(oldnode);
}

void eraselist(node* oldlist){
  node* next;
  while(oldlist!=NULL){
    next=oldlist->next;
    erasenode(oldlist);
    oldlist=next;
  }
}

void deleteHtable(htable* hashtable){
  int i;
  for(i=0;i<hashtable->size;i++)
    eraselist(hashtable->table[i]);
  free(hashtable->table);
  free(hashtable);
}

void* getkey(node* node){
  return node->key;
}

void* getvalue(node* node){
  return node->value;
}

void* get(htable* hashtable,void* key){
  int location=hashtable->hashcode(key);
  location=location%hashtable->size;
  node* auxnode=hashtable->table[location];
  if(!key)return NULL;
  if(auxnode==NULL)return NULL;
  while(auxnode){
    if(hashtable->equals(getkey(auxnode),key))return getvalue(auxnode);
    auxnode=auxnode->next;
  }
  return NULL;
}

htable* resize(htable* hashtable){
  node **newtable, **oldtable, *auxnode;
  oldtable=hashtable->table;
  int i,oldsize=hashtable->size;

  hashtable->size=hashtable->size*2;
  if(!(newtable=(node**)malloc(hashtable->size*sizeof(node*))))
    return NULL;

  hashtable->table=newtable;
  hashtable->used=0;


  for(i=0;i<oldsize;i++){
    if(oldtable[i]){
      auxnode=oldtable[i];
      while(auxnode!=NULL){
        put(hashtable,getkey(auxnode),getvalue(auxnode));
        auxnode=auxnode->next;
      }
      eraselist(oldtable[i]);
    }
  }
  free(oldtable);

  return hashtable;
}

void* contains(htable* hashtable,void* key){

}

void* remove(htable hashtable,void* key){
  
}

void* put(htable* hashtable, void* key, void* value){
  int location,equal=0;
  node *auxnode, *newnode;
  key=hashtable->clonekey(key);
  value=hashtable->clonevalue(value);

  location=hashtable->hashcode(key);

  if(location<0)return NULL;

  if((float)hashtable->used/(float)hashtable->size>LOADFACTOR)
    if(!resize(hashtable))return NULL;

  location=location%hashtable->size;

  if(!(newnode=(node*)malloc(sizeof(node))))
    return NULL;

  newnode->key=key;
  newnode->value=value;

  if(hashtable->table[location]==NULL)
    hashtable->table[location]=newnode;
  else{
    if(hashtable->equals(hashtable->table[location]->key,key)){
      newnode->next=hashtable->table[location]->next;
      erasenode(hashtable->table[location]);
      hashtable->table[location]=newnode;
    }
    else{
      auxnode=hashtable->table[location];
      while(auxnode->next){
        equal=hashtable->equals(auxnode->next->key,key);
        if(equal)break;
        auxnode=auxnode->next;
      }
      equal=hashtable->equals(auxnode->next->key,key);
      if(equal){
        newnode->next=auxnode->next->next;
        erasenode(auxnode->next);
        hashtable->used--;
      }
      auxnode->next=newnode;
    }
  }
  hashtable->used++;

  return value;
}
