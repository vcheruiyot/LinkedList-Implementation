# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = radix_sort qsort linkedtest

all:: $(PROGRAMS)

CC = gcc
CFLAGS = -g -O0 -std=gnu99 -Wall $$warnflags
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -fno-diagnostics-show-option
LDFLAGS =
LDLIBS = -lm

$(PROGRAMS): %:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

radix_sort qsort linkedtest: linkedlist.c read_line.c

clean::
	rm -f $(PROGRAMS) *.o

.PHONY: clean all
