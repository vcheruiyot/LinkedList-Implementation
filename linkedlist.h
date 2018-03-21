/*
 * File: Linked_list.h
 * Defines the interface of the linkedlist type
 *
 * The Linkedlist manages a list of entries just like the List
 * object in any object-oriented programming language. This data
 * structure is particularly useful and has it distinct advantages. 
 *
 * Victor Cheruiyot
 *
 */
#ifndef _linked_list_h
#define _linked_list_h

#include <stddef.h>
#include <stdbool.h>
typedef struct node
{
	void *data;
	struct node *next;
}node;
typedef struct linked_list
{
	node *start;
	size_t elements;
	size_t elemsize;
	void (*free_fn)(void *);

}linked_list;

typedef void (*call_back)(void *);
typedef void (*call_free)(void *);
/*
 * This function creates a new empty linked list. The pointer storage is allocated in
 * the heap. When done the client must call delete_list to dispose of it. 
 */
linked_list *create_list(size_t elemsize, call_free f);
void list_prepend(linked_list *list, void *data);
void delete_list(linked_list *list);
void traverse(linked_list *list, call_back h);
void *list_nth(linked_list *list, size_t position);
void to_delete(linked_list *list, size_t position);
void list_append(linked_list *list, void *data);
void list_insert(linked_list *list, void *data, size_t pos);
bool is_empty(linked_list *list);
#endif
