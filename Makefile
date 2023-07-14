CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -I. -lfreeimage

lsb: lsb.c
	gcc -o $@ $^ -I. -Wall

test_lsb.o: test_lsb.c lsb.c
	gcc -o $@ $^ -I. -Wall
	./$@

test_converters.o: test_converters.c converters.c
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS) 
	./$@
