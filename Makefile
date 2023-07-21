CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -I. -lfreeimage

test_lsb.o: test/test_lsb.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) -I.

test_converters.o: test/test_converters.c converters.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

test_stegano.o: test/test_stegano.c stegano.c converters.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

stegano: main.c stegano.c converters.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
