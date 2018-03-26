/*
 * Code by Victor Cheruiyot
 */
#include "samples/prototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <assert.h>

#define CAPACITY_HINT 1000

int cmp_fn(const void *a, const void *b){
	return (*(int *)a - *(int *)b);
}
void write_to_file(int *block, FILE *fp, int size){
	for(int i = 0; i < size; i++){
		int value = block[i];
		int length_val = snprintf(NULL, 0, "%d", value);
		char str[length_val + 1];
		snprintf(str, length_val + 1, "%d", value);
		int result = fputs(str, fp);
		if(result == EOF){
			error(1, 0, "unable to write to file\n");
			exit(1);
		}
		fputs("\n", fp);
	}
	free(block);
}

void perform_sort(int *block, FILE *stream, FILE *fp){
	int actual_capacity = 0;
	size_t capacity = CAPACITY_HINT;
	while(1){
		char *line = read_line(stream);
		if(!line)break;
		int number = atoi(line);
		if(actual_capacity == capacity){
			int *alloc = realloc(block, sizeof(int) * (capacity *= 2));
			assert(alloc != NULL);
			block = alloc;
		}
		block[actual_capacity] = number;
		actual_capacity++;
		free(line);
	}
	//perform qsort
	qsort(block, actual_capacity, sizeof(int), cmp_fn);
	write_to_file(block, fp, actual_capacity);

}
int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Enter two files: we'll sort the first one and put the results on the second file\n");
		exit(1);
	}
	FILE *stream = fopen(argv[1], "r");
	FILE *fp = fopen(argv[2], "w");
	int *block = malloc(sizeof(int) * CAPACITY_HINT);
	perform_sort(block, stream, fp);
	fclose(stream);
	fclose(fp);
	return 0;
}
