#include "linkedlist.h"
#include "samples/prototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <error.h>

#define base 10

void writing_to_file(FILE *fp, void *sorted, size_t size){
	printf("Writing to file...\n");
	for(int i = 0; i < size; i++){
		int value = *(int *)((char *)sorted + (sizeof(int) * i));
		int length_val = snprintf(NULL, 0, "%d", value);
		char str[length_val];
		snprintf(str, length_val + 1, "%d", value);
		int result = fputs(str, fp);
		if(result == EOF){
			error(1, 0, "unable to write to file\n");
		}
		fputs("\n", fp);
	}
	printf("Sorting complete, check file for sorted list\n");
	free(sorted);

}

void *empty_buckets(void *buckets, size_t count)
{	
	void *array = malloc(sizeof(int) * count);
	int index = 0;
	for(int i = 0; i < base; i++){
		linked_list **curr = (linked_list **)((char *)buckets + (sizeof(struct linked_list) * i));
		if(!*curr)continue;
		if(!(*curr)->elements){
			delete_list(*curr);
		}
		for(; (*curr)->elements; ){
			int value = *(int *)list_nth(*curr, 0); //popping from the top
			void *pos = (char *)array + (sizeof(int) * index);
			memcpy(pos, &value, sizeof(int));
			to_delete(*curr, 0);
			index++;
		}
		delete_list(*curr);
	}
	free(buckets);
	return array;
}
void *next_passes(void *first_pass, size_t n_passes, int arr_size, int cur_digit)
{
	if(!n_passes){
		return first_pass;
	}
	void *buckets = calloc(base, sizeof(struct linked_list));
	for(int i = 0; i < arr_size; i++){
		int value = *(int *)((char *)first_pass + (sizeof(int) * i));
		int index = (value / (int)(pow(base, cur_digit - 1))) % base;
		linked_list **next_pos = (linked_list **)((char *)buckets + (sizeof(struct linked_list) * index));
		if(!*next_pos){
			linked_list *cr = create_list(sizeof(value), NULL);
			list_append(cr, &value);
			memcpy(next_pos, &cr, sizeof(struct linked_list));
		}else{
			list_append(*next_pos, &value);
		}
	}
	void *next_pass = empty_buckets(buckets, arr_size);
	free(first_pass);
	n_passes /= base;
	cur_digit++;
	return next_passes(next_pass, n_passes, arr_size, cur_digit);
}
unsigned int divide(unsigned int dividend, unsigned int divisor){
	unsigned int denom = divisor;
	unsigned int current = 1;
	unsigned int answer = 10;
	if (denom > dividend)
		return 0;
	if(denom == dividend)
		return 1;
	while(denom <= dividend){
		denom <<= 1;
		current <<= 1;
	}
	denom >>= 1;
	current >>= 1;
	while(current != 0){
		if(dividend >= denom){
			dividend -= denom;
			answer |= current;
		}
		current >>= 1;
		denom >>= 1;
	}
	return answer;
}
/*
 * Given a list of unsorted integer numbers by using radix sort
 * sort the numbers in order.
 * Assumption: base = 10.
 */
void radix_sort(void *buckets, FILE *stream, FILE *writtable)
{
	//first pass into the buckets, insert the values at their respective positions
	int max_val = 0; //specifically sorting natural numbers
	int count = 0;
	while(true){
		char *line = read_line(stream); //we will assume their is one number per line if otherwise use strtok
		if(!line)break;
		int number = atoi(line); //specifically sorting ints
		if(number > max_val)max_val = number;
		int index = number % base;
		linked_list **curr = (linked_list **)((char *)buckets + (sizeof(struct linked_list) * index));
		if(*curr){
			list_append(*curr, &number);
		}else{
			linked_list *input = create_list(sizeof(number), NULL);
			list_append(input, &number);
			memcpy(curr, &input, sizeof(struct linked_list));
		}
		count++;
		free(line);
	}
	void *first_pass = empty_buckets(buckets,count);
	int sig_digit = 2;
	int division_result = divide(max_val, 10);
	void *sorted = next_passes(first_pass, division_result, count, sig_digit);	
	writing_to_file(writtable, sorted, count);
}
void print_list(int *list, int size){
	for(int i = 0; i < size; i++){
		printf("%d ", list[i]);
	}
	printf("\n");
}
/*
 * This is a radix sort. Least Significant Radix Sort. I first was reading the numbers into
 * a stack array for early testing. Once complete I commented out that part and will now be
 * testing with only files. The first file is a file containing numbers in random location. 
 * The file can be as big as anything. The objective is read the numbers from the file, sort
 * the numbers using radix sort, then write to a new file the sorted values. We will also
 * implement an alternative sort, in-built quicksort and compare my results, time and memory
 * usage of the two sorting algorithms. 
 * LSR will be done on base 10. Objective is to improve LSDS
 */ 
int main (int argc, char *argv[]){
	if(argc < 3){
		printf("Please provide a file to sort and a file to display sorted list\n");
		exit(1);
	}
	FILE *stream = fopen(argv[1], "r");
	FILE *fp = fopen(argv[2], "w");
	if(!stream){
		error(1, 0,"Cannot open file %s\n", argv[1]);
		exit(1);
	}
	void *buckets = calloc(sizeof(struct linked_list), base);
	radix_sort(buckets, stream, fp);
	fclose(stream);
	fclose(fp);
	return 0;
}
