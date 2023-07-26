CC = gcc
CFLAGS = -Wall -Werror
# LDFLAGS = -I. -lfreeimage  # For Linux
LDFLAGS = -I. -I..\FreeImage3180Win32Win64\FreeImage\Dist\x64\ -L..\FreeImage3180Win32Win64\FreeImage\Dist\x64\ -lFreeImage  # For Windows, if FreeImage is in ..
TARGET = stegano
SRC = main.c stegano.c converters.c lsb.c utils.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

test_lsb: test/test_lsb.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) -I.

test_converters: test/test_converters.c converters.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

test_stegano: test/test_stegano.c stegano.c converters.c lsb.c utils.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
