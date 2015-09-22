#ifndef UTF8READ_H
#define UTF8READ_H


/* Imports */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Type identification
#include <cctype>


// Byte-Order Marks
#define BOM0 0xffffffef
#define BOM1 0xffffffbb
#define BOM2 0xffffffbf

// Significant bytes for UTF-8, which is always of the form ffffff[xx], where [xx] is the byte needed
#define UTF8BYTE 0x000000FF
#define UTF8ID   0x000000F0

// Special structures of UTF
#define TWOBYTEC  0xC0
#define TWOBYTED  0xD0
#define THREEBYTE 0xE0
// The rest are invalid

// Encoding variable, shift to the left by the number of bytes of form 10xxxxxx
#define ENC 6


/* A UTF8 to UTF16 converter
 * Uses range rules from Linux manpages
 * 	2**(6+1)   == 128
 * 	2**(5+1*6) == 2048       == 0x800
 * 	2**(4+2*6) == 65536      == 0x10000
 * 	2**(3+3*6) == 2097152    == 0x200000
 * 	2**(2+4*6) == 67108864   == 0x4000000
 * 	2**(1+5*6) == 2147483648 == 0x80000000
 *
 * Note: 3 bytes upper limit on length
 *
 *
 * Read characters from a text file encoded in UTF-8
 * Ignores BOM (Byte-Order Mark), as it is not recommended for UTF-8
 * Converts everything to wide characters (wchar_t)
 * Stop at the end of the line and pick up from there when called again
 * The actual output is a more standard UTF-16
 */
std::wstring fReadUTF8(std::ifstream &infile);


#endif