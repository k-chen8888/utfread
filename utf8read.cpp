/* Imports */

#include <bitset>

// Class header
#include "utf8read.h"


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
int utf8to16(int value)
{
	if(value > (int)0b01111111)
	{
		if(value > (int)0b1101111110111111)
		{
			if(value > (int)0b111011111011111110111111)
			{
				// Out of bounds
				return value;
			}
			else
			{
				// 3 bytes
				// Extract the part in brackets: 1110[xxxx] 10[xxxxxx] 10[xxxxxx]
				return ( ( ( (value & (15 << 16)) >> 2 ) + (value & (63 << 8)) ) >> 2 ) + (value & (63));
			}
		}
		else
		{
			// 2 bytes
			// Extract the part in brackets: 110[xxxxx] 10[xxxxxx]
			return ( (value & (31 << 8)) >> 2 ) + (value & (63));
		}
	}
	else
	{
		// It's ASCII (1 byte)
		// Extract the part in brackets: 0[xxxxxxx]
		return value & 127;
	}
}


/* Read a single line of characters from a text file encoded in UTF-8
 * Ignores BOM (Byte-Order Mark), as it is not recommended for UTF-8
 * Converts everything to wide characters (wchar_t)
 * Stop at the end of the line and pick up from there when called again
 * The actual output is a more standard UTF-16
 */
std::wstring fReadUTF8(std::ifstream &infile)
{
	std::wstring out;
	
	// Start out by getting a line from the file
	std::string line;
	getline(infile, line);
	
	if(line.length() > 0)
	{
		// Ignore BOM
		unsigned int start = 0;
		if(line[0] == BOM0 && line[1] == BOM1 && line[2] == BOM2)
		{
			start = 3;
		}
		
		
		// Get all characters (single bytes) from the line
		for(unsigned int i = start; i < line.length(); i++)
		{
			// 1-byte UTF-8 is the same as ASCII
			if(isascii(line[i]))
			{
				out += (wchar_t)line[i];
			}
			// Now we know it's either 2-bit or 3-bit UTF-8
			else
			{
				int leading = (int)line[i] & UTF8ID;
				int result = 0;
				
				switch(leading)
				{
					case TWOBITC: case TWOBITD:
						result += (line[i] & 31) << 6;
						result += line[i + 1] & 63;
						
						// How many spaces to increment i by? That depends on whether or not the third character is equivalent to 0...
						if (line[i + 2] == 0)
						{
							i += 2;
						}
						else
						{
							i += 1;
						}
						
						break;
					
					case THREEBIT:
						result += (line[i] & 15) << 12;
						result += (line[i + 1] & 63) << 6;
						result += line[i + 2] & 63;
						
						i += 2;
						
						break;
					
					default:
						break; // Invalid UTF-8
				}
				
				out += (wchar_t)result;
			}
		}
	}
	
	return out;
};