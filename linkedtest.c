/*
 * Performs a few tests to test linkedlist implementation
 * Victor Cheruiyot
 */  
#include "linkedlist.h"
#include "samples/prototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <assert.h>
#include <string.h>

void free_string(void *data){
	char *p = *(char **)data;
	free(p);
}

void print_list(linked_list *list){
	printf("[");
	int i;
	for(i = 0; i < list->elements - 1 && list->elements; i++){
		char **vr = (char **)list_nth(list, i);
		printf("%s: ", *vr);
	}
	printf("%s]\n", list->elements ? *((char **)list_nth(list, i)) :"");
}
void random_deletion(linked_list *init){
	while(1){
		if(init->elements == 0)break;
		size_t pos = 0;
		to_delete(init, pos);
		//print_list(init); //verify
	}
	if(is_empty(init))free(init);
	//printf("%d\n", is_empty(init));
	printf("Garbage collector works!\n");
}
void aggressive_insert(linked_list *init, FILE *stream){
	while(1){
		char *line = read_line(stream);
		if(!line)break;
		size_t position = 0;
		list_insert(init, &line, position);
		char *cpy = *(char **)list_nth(init, position);
		assert(strcmp(cpy, line) == 0);
	}
	printf("Insertion into the list works\n");
	random_deletion(init);
}
void appending_list(linked_list *init, FILE *stream){
	size_t count = 0;
	while(1){
		char *line = read_line(stream);
		if(!line)break;
		list_append(init, &line);
		assert(strcmp(*(char **)list_nth(init, count), line) == 0);
		count++;
	}
	printf("Appending seems great\n");

}
void check_file(FILE *stream){
	if(!stream){
		error(1, 0, "cannot open the file\n");
		exit(1);
	}
}
/*
 * This program is meant to test the creation, usage and
 * disposal of the linkedlist class created in the linked-
 * list.c file. 
 * We will have test that test on expandability of the linked
 * list, deletion, appending of nodes, how the linkedlist hand
 * les different data types.
 */

int main (int argc, char *argv[]){
	//linkedlist creation
	linked_list *init = create_list(sizeof(char *), free_string);
	assert(init != NULL);

	if(argc < 3){
	    printf("Please enter a two files to test appending and insertion\n");
	    exit(1);
	}
	FILE *stream = fopen(argv[1],"r");
	FILE *fp = fopen(argv[2], "r");
	check_file(stream);
	check_file(fp);
	appending_list(init, stream);
	//test insertion at some node and nth data
	fclose(stream);
	aggressive_insert(init, fp);
	fclose(fp);
}
