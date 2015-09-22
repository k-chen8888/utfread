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
 *
 *
 * Read characters from a text file encoded in UTF-8
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
		if((unsigned int)line[0] == BOM0 && (unsigned int)line[1] == BOM1 && (unsigned int)line[2] == BOM2)
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
				int result = 0;
				
				// The first byte reveals the length of the UTF-8 character
				switch(line[i] & UTF8ID)
				{
					case TWOBYTEC: case TWOBYTED:
						result += (line[i] & 31) << ENC; // 110[xxxxx] 10xxxxxx
						result += line[i + 1] & 63;      // 110xxxxx 10[xxxxxx]
						
						// How many spaces to increment i by? That depends on whether or not the third character is encoded as 0...
						i += ( line[i + 2] == 0 ? 2 : 1 );
						
						break;
					
					case THREEBYTE:
						result += (line[i] & 15) << 2 * ENC; // 1110[xxxx] 10xxxxxx 10xxxxxx
						result += (line[i + 1] & 63) << ENC; // 1110xxxx 10[xxxxxx] 10xxxxxx
						result += line[i + 2] & 63;          // 1110xxxx 10xxxxxx 10[xxxxxx]
						
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