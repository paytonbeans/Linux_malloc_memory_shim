//Name: Payton Beans
//CUID: C21285765
//Shim library

# define _GNU_SOURCE
# include <dlfcn.h>
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/types.h>

struct alloc{
  void *p;
  size_t size;
};

void starting (void) __attribute__((constructor));
void ending (void) __attribute__((destructor));

void *(*original_malloc)(size_t size) = NULL;
void *(*original_free)(void *ptr) = NULL;
struct list_t my_list;

// Node struct
typedef struct node_t{
  int size;
  void *data;
  struct node_t *next;
} node;

// Linked list struct
typedef struct list_t{
  node *head;
  int count;
} list;

// Constructor, should initialize original_free and original_malloc
void starting(void){
  if(original_free == NULL){
    original_free = dlsym(RTLD_NEXT, "free");
  }

  if(original_malloc == NULL){
    original_malloc = dlsym(RTLD_NEXT, "malloc");
  }



}


// Deconstruct linked list and print results
void ending(void){
  node *holder = NULL;
  int *total = original_malloc(sizeof(int));
  *total = 0;
    while(my_list.head != NULL){
      holder = my_list.head;
      fprintf(stderr, "LEAK %i\n", holder->size);
      *total = *total + holder->size;
      my_list.head = holder->next;
      original_free(holder);
    }

    fprintf(stderr, "TOTAL: %i\n", *total);

    original_free(total);
    original_free(holder);

}





void free (void *ptr){
  node *iter;
  node *prev;

  iter = my_list.head;
  // Check head of list
  if(iter != NULL && iter->data == ptr){
    my_list.head = iter->next;
    original_free(ptr);
    original_free(iter);
    my_list.count--;
  }
  else{
    // Search through list for matching pointer variable
    while(iter != NULL && iter->data != ptr){
      prev = iter;
      iter = iter->next;
    }
    // If iter != Null then the correct node has been found
    // If null then no node matchers
    if(iter != NULL){
      prev->next = iter->next;
      original_free(ptr);
      original_free(iter);
      my_list.count--;
    }
    else{
      original_free(ptr);
    }
  }
}

void *malloc(size_t size){
  // Call original_malloc with provided size
  void *point = original_malloc(size);

  // Add new node to list
  node *new_node;
  node *iter;

  //Malloc node using only original malloc
  new_node = (node*) original_malloc(sizeof(node));
  new_node->size = (int) size;
  new_node->data = point;

  // Add new_node to list_t, special case if list is empty
  if(my_list.head == NULL){ // only first Node
    my_list.head = new_node;
    my_list.count = 1;
  }
  else{//This is every case except first node
    iter = my_list.head;
    while(iter->next != NULL){
      iter = iter->next;
    }
    iter->next = new_node;
    my_list.count++;
  }

return point;
}
