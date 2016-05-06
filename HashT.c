#define STARTINGSIZE 16
#define LOADFACTOR 0.7

typedef struct _node_{
  void* key;
  void* value;
  struct _node_* next;
}node;

typedef struct _hashtable_{
  int *hashcode(void*);
  int size;
  int used;
  node** table;
}hashtable;
