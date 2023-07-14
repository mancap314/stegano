CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -I. -lfreeimage

lsb: lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) -I.

test_lsb.o: test_lsb.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) -I.

test_converters.o: test_converters.c converters.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 
