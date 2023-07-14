CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -I. -I..\FreeImage3180Win32Win64\FreeImage\Dist\x64\ -L..\FreeImage3180Win32Win64\FreeImage\Dist\x64\ -lFreeImage

lsb: lsb.c
	gcc -o $@ $^ -I. -Wall

test_lsb: test_lsb.c lsb.c
	gcc -o $@ $^ -I. -Wall
	.\$@.exe

test_converters: test_converters.c converters.c
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS) 
	.\$@.exe