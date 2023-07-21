# STEGANO

C programm for LSB steganography

## Installation

1. Install [FreeImage](https://freeimage.sourceforge.io/)
2. Clone this repository
3. [For Windows only] copy FreeImage.dll in the cloned folder
4. Execute `make stegano`

## Description

_Stegano_ conceals any file in any other file using LSB (Last Significant Bit) steganography. The data is stored at the end of the file, in order to prevent header corruption making the resulting file unreadable. In case the covering file is an image in jpg or png format, it can optionally be converted bmp (bitmap) format before insertion of the information. Information about file extension and size of the covered filed is also stored in the covering file.
