/* Imports */

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
			// Detect non-ASCII characters and convert them to UTF-16 manually
			if(!isascii(line[i]))
			{
				// Grab UTF-8 representation of the character (3 bytes)
				int value = ( ( (int)line[i] & UTF8BYTE ) << 16 ) + ( ( (int)line[i + 1] & UTF8BYTE ) << 8 ) + ( (int)line[i + 2] & UTF8BYTE );
				
				// Use a converter to get UTF-16
				int u16char = utf8to16(value);
				
				// Output
				out += (wchar_t)u16char;
				
				i = i + 2;
			}
			else
			{
				out += (wchar_t)line[i];
			}
		}
	}
	
	return out;
};