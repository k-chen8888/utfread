# utfread
Read a file encoded in UTF-8 and converts it to UTF-16

See [here](http://stackoverflow.com/questions/6240055/manually-converting-unicode-codepoints-into-utf-8-and-utf-16) for information on converting between UTF-8 and UTF-16

This implementation takes advantage of the structure of UTF-8 to determine the size of the representation:
 1. Anything that is not of the form 0xFFFFFFxx is not Unicode; it's ASCII
 2. 0xCx or 0xDx refers to a 2-byte representation: 110x xxxx 10xx xxxx
 3. 0xEx refers to a 3-byte representation: 1110 xxxx 10xx xxxx 10xx xxxx
 4. Any other leading byte is no longer valid UTF-8, since the standard no longer includes 4-6 byte characters

Using this fact, it's simple to switch to the correct character length and extract the relevant bits:
 1. 2-byte representations require a 5-bit extraction on the first byte and a 6-bit extraction on the second byte. There may or may not be a third byte (which is 0 if there is one)
 2. 3-byte representations require a 4-bit extraction on the first byte and 6-bit extractions on the second and third bytes.

Upon extracting a sequence of bits, simply shift the extraction by a multiple of 6 to its correct position.
