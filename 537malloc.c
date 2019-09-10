#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "537malloc.h"
#include "537tree.h"
#include <stdint.h>

const int BUFFER_SIZE = 1024;
tree *maple = NULL;

tree * constructTree() {
  tree * maple = malloc(sizeof(tree));
	maple->root = NULL;
 return maple;
}

void *malloc537(size_t size) {

if (maple == NULL){
    maple = constructTree();
}
 int * temp;
      tuple * t = malloc(sizeof(tuple));
			t->len = size; 
			temp = (int*)malloc(size);
      t->addr = (uintptr_t)&(*temp);
	if (size == 0) {
		printf("Warning: size is 0!");
	}
   tuple e = *t;
   insert(maple, e); 
   return temp;
}

void free537(void *ptr) {
	// Check to make sure malloc537 was used to malloc
    Node * needsFree = search537(ptr);
    if (needsFree->data.len == 0) {
      printf("Error: Memory has not been previously allocated at this address");
      exit(-1);
    }
    if (needsFree->free == 1) {
    printf("Error: Attempting to double free");
    exit(-1);
    }
    else if (needsFree->free == 0){
    needsFree->free = 1;
    free(ptr);
    }
    //deleteByVal(maple, needsFree->data);
	}

struct Node *search537(uintptr_t *addr) {
  Node *temp = maple->root;
  while (temp != NULL) {
    if (temp->data.addr > (uintptr_t)&(*addr)) {
      //printf("Is smaller than");
      if (temp->left == NULL) {
        //printf("Warning: Memory address 2 not allocated");
        break;
        }
       else { 
      temp = temp->left;
      }
    }
    else if (temp->data.addr == (uintptr_t)&(*addr)) {
    //printf("Is equal to");
    break;
    }
    else {
    //printf("Is larger than");
      if (temp->right == NULL) {
        uintptr_t add = temp->data.addr + temp->data.len;
        if (add > (uintptr_t)&(*addr)){
        //printf("The memory is allocated but not a node");
        }
        else {
        //printf("Warning: Memory address too big");
        
        break;
        }
        }
      else {
    temp = temp->right;
    }
    }
}
return temp;
}

/*void *realloc537(void *ptr, size_t size) {
if (ptr == NULL) {
malloc537(size);
}
if (size == 0 && ptr != NULL) {
free537(ptr);
}
}*/


int main () {
  /*malloc537(5);
  malloc537(5);
  uintptr_t *test = malloc537(6);
  malloc537(5);
  malloc537(5);
  malloc537(5);
  malloc537(5);
  malloc537(5);
  malloc537(5);*/
  for (int i = 0; i < 10; i++) {
  tuple t;
  t.addr = i;
  t.len = i;
  insert(maple, t);
  }
  
  //uintptr_t *test1 = malloc(sizeof(uintptr_t));
  //uintptr_t test1 = (uintptr_t) &(*test);
  //test1 += 1;
  //printf("%ld\n", test1);
  print2D(maple);
  free537(3);
  //Node *search = search537(test);
  print2D(maple);
  //printf("%ld\n", test1);
  //printf("%ld\n", search->data.addr);
  //printf("%ld\n", search->data.len);
  //printf("%c\n", search->color);
}
//find in tree
//see if it is valid for freeing (if it exists in the tree)
//if it doesn't exist, throw an error
//if it does exist, ->
//free the memory passed in from the pointer to the pointer +length.(ex: pointer = 120, length = 16, you would free 120-136)
//delete the node once it has been freed


