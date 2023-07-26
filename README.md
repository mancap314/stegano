# STEGANO

C programm for LSB steganography

## Installation

1. Install [FreeImage](https://freeimage.sourceforge.io/)
2. Clone this repository
3. [For Windows only] copy FreeImage.dll in the cloned folder. Adapt `LDFLAGS` (see [Makefile](Makefile))
4. Execute `make`

## Description

_Stegano_ conceals any file in any other file using LSB (Last Significant Bit) steganography. 

_Stegano_ stores *at the end* of the file, in order to prevent header corruption making the resulting file unreadable. 

Information about file extension and size of the covered filed is also stored in the covering file. This ensures more efficient subsequent extraction from this file.

In case the covering file is a jpeg or a png image, it is possible to have an intermediary bitmap (bmp) conversion. In this case, the file to covert will be inserted in the intermediary bmp image. Then this bmp file will be converted into png format *exclusively* (because bmp -> jpg -> bmp conversions generate losses in the last bmp file, making the original file unrecoverable).

size(covering file) >= 8 * size(file to cover) + 96 (in bytes), because 96 bytes are used to store size and extension of the original file, and each byte of the covering file can cover only one bit of the file to cover (lsb approach). Basically, *any file can be covered in any other file* as long as this condition applies. NB: in case of intermediary bmp conversion, this condition must apply for the bmp conversion of the covering image, not to the covering image itself.

## Usage

For example
```sh
./stegano -a insertion -t data/message.txt -c data/fractal_2.jpg -b -o data/f2.png
```
will insert `data/message.txt` into `data/fractal_2.jpg` with intermediary bitmap conversion, and the result will be stored in `data/f2.png`.

On the other hand,
```sh
./stegano -a extraction -c data/f2.png -b -o data/message_extracted
```
will extract the file encoded in `data/f2.png` with intermediary bitmap conversion and store the result in `data/message_extracted.ext`, where `ext` is the extension of the original file.

For more details:
```
./stegano --help
```