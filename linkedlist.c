/*
 * Complete linkedlist implementation by Victor Cheruiyot
 * Please read the comments from every function to be able to properly
 * call and use linkedlist
 */
#include "linkedlist.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
* This function creates a new node by dynamically allocating memory from the heap.
* If unable to dynamically allocate memory asserts.
*/
linked_list *create_list(size_t elemsize, call_free f){
	linked_list *the_list = malloc(sizeof(struct linked_list));
	assert(the_list != NULL);
	the_list->start = NULL;
	the_list->elements = 0;
	the_list->elemsize = elemsize;
	the_list->free_fn = f;
	return the_list;
}
/*
 * This function creates a new node by allocating memory dynamically just
 * as for the linked list. Only invoked when user appends new data into the
 * the linked list
 */ 
static node *create(linked_list *list, void *data, node *next){
	node *head = malloc(sizeof(node));
	//check if the sizeof(data) matches what the client had asked for
	head->data = malloc(list->elemsize);
	memcpy(head->data, data, list->elemsize);
   	/*for(int i = 0; i < list->elemsize; i++){
		*((char *)head->data + i) = *((char *)data + i);
	}*/
	head->next = next;
	return head;
}
/*
* This function adds a new node to the beginning of a linked list
*/ 
void list_prepend(linked_list *list, void *data){
	node *head = list->start;
	node *new_node = create(list, data, head);
	list->start = new_node;
	list->elements++;
}
/*
* This function traverses the entire linked list. Makes the 
* function generic by introducing a call back function to 
*/
void traverse (linked_list *list, call_back f){
	node *cursor = list->start;
	while(cursor != NULL){
		f(cursor->data);
		cursor = cursor->next;
	}
}
/*
* Deletes the entire list.
*/
void delete_list(linked_list *list){	
	for(size_t i = 0; list->elements ;)
		to_delete(list, i);
	free(list);
}
/*
* THis is a helper function for error checking
*/
static void error_checking(linked_list *list, size_t position){
	assert(list != NULL && position <= list->elements);
}
/*
 * This is a helper function. Returns the node at a given position
 * in the list. Should be made private
 */ 
static node *nth_node(linked_list *list, size_t position){
	node *head = list->start;
	node *temp = head;
	for(size_t i = 0; i < position; i++){
		temp = temp->next;
	}
	return temp;
}
/*
 * Returns an object stored at nth position in the linked list
 */ 
void *list_nth(linked_list *list, size_t position){
	node *n = nth_node(list, position);
	return n->data;
}
/*
 * Deletes an object at a given position in the linked list
 */
static void dispose_data(void *data, void (*free_fn)(void *)){
	if(free_fn){
		free_fn(data);
	}
	free(data);
}
/*
 * Deletes a specific entry from a linked_list 
 */ 
void to_delete(linked_list *list, size_t position){
	error_checking(list, position);
	node *head = list->start;

	if(position == 0){
		node *temp = head->next;
		dispose_data(head->data, list->free_fn);
		free(head);
		list->start = temp;
		list->elements--;
		return;
	}
	node *prev = nth_node(list, position - 1);
	node *to_del = prev->next;
	prev->next = to_del->next;
	dispose_data(to_del->data, list->free_fn);
	free(to_del);
	list->elements--;
	return;
}
/*
* Inserts an object at the given position. 
*/
void list_insert(linked_list *list, void *data, size_t position){
        error_checking(list, position);	
	if(list->start == NULL){
		list_append(list, data);
		return;
	}
	if(position == 0){
		list_prepend(list, data);
		return;
	}
	node *new_node = create(list, data, NULL);
	node *temp = nth_node(list, position - 1);
	new_node->next = temp->next;
	temp->next = new_node;
	list->elements++;
}
/*
* Appends an object at the end of a linked list
*/
void list_append(linked_list *list, void *data){
	node *head = list->start;
	node *new_node = create(list, data, NULL);
	list->elements++;
	if(!head){
		list->start = new_node;
		return;
	}
	node *tmp = head;
	while(tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
}
/* Check if the list is empty */
bool is_empty(linked_list *list){
	return list->start == NULL;
}
