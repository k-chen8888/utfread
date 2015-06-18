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
 */
int utf8to16(int value);
 

/* Read characters from a text file encoded in UTF-8
 * Ignores BOM (Byte-Order Mark), as it is not recommended for UTF-8
 * Converts everything to wide characters (wchar_t)
 * Stop at the end of the line and pick up from there when called again
 * The actual output is a more standard UTF-16
 */
std::wstring fReadUTF8(std::ifstream &infile);


#endif