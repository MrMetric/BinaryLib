#include "../include/BinaryReader.hpp"
#include <stdio.h> // for file functions
#include <sstream>
#include <string.h> // for strerror and errno
#include <iostream>

#define MAKESTR(ss) static_cast<std::ostringstream&>(std::ostringstream().seekp(0) << ss).str()
#define BYTESTOTYPE(type)\
({\
	uint8_t* buf = this->ReadBytes(sizeof(type));\
	type ret = *(reinterpret_cast<type*>(buf));\
	delete buf;\
	ret;\
})

#if (__cplusplus != 201103L) && !defined(NULLPTR_EMU)
#define NULLPTR_EMU
// From http://stackoverflow.com/a/2419885
const								// this is a const object...
class
{
	public:
		template<class T>			// convertible to any type
			operator T*() const		// of null non-member
			{ return 0; }			// pointer...
		template<class C, class T>	// or any type of null
			operator T C::*() const	// member pointer...
			{ return 0; }
	private:
		void operator&() const;		// whose address can't be taken
} nullptr = {};						// and whose name is nullptr
#endif

/**
	BinaryReader.cpp
	This class is my implementation of Microsoft's C# BinaryReader
	It reads bytes from a file and returns the following:
	 - signed and unsigned integer values (8-bit, 16-bit, 32-bit, 64-bit, or 128-bit)
	 - single characters (8-bit int) or strings of characters (in std::string)
	It can read strings that have their length specified by a 7-bit encoded int

	Thanks to Matt Davis for the reinterpret_cast usage
		http://stackoverflow.com/a/545020/1578318

	@todo		Check return values of fread
	@todo		Make an exception class to throw when errors occur instead of using std::string
*/

/**
	@arg		s The file to read
*/
BinaryReader::BinaryReader(std::string s) : isLoaded(false), data(nullptr), file(nullptr)
{
	this->ChangeFile(s);
}

/**
	@arg		data The byte array to read
	@arg		size The size (in bytes) of the byte array
*/
BinaryReader::BinaryReader(uint8_t* data, const uint_fast64_t size) : isLoaded(false), data(nullptr), file(nullptr)
{
	this->ChangeFile(data, size);
}

/**
	Close the current file, if one is loaded, and open a file
	@arg		s The file to read
*/
void BinaryReader::ChangeFile(std::string s)
{
	if(this->isLoaded)
	{
		this->Close();
	}

	this->fname = s;
	this->pos = 0;
	this->data = nullptr;
	this->file = fopen(s.c_str(), "rb");
	if(this->file == NULL || ferror(this->file))
	{
		throw MAKESTR("BinaryReader: Error opening \"" << s << "\": " << strerror(errno));
	}
	fseek(file, 0, SEEK_END);
	this->fSize = ftell(file);
	rewind(file);
	if(ferror(this->file))
	{
		throw MAKESTR("BinaryWriter: Error getting file info: " << strerror(errno));
	}
	this->isLoaded = true;
	this->usingArray = false;
}

/**
	Close the current file, if one is loaded, and use a byte array
	@arg		data The byte array to read
*/
void BinaryReader::ChangeFile(uint8_t* data, const uint_fast64_t size)
{
	if(this->isLoaded && !this->usingArray)
	{
		this->Close();
	}

	this->fname = "";
	this->pos = 0;
	this->file = nullptr;
	this->data = data;
	this->fSize = size;
	this->isLoaded = true;
	this->usingArray = true;
}

void BinaryReader::Close()
{
	if(this->usingArray)
	{
		this->data = nullptr;
	}
	else
	{
		fclose(this->file);
		this->file = nullptr;
	}
	this->isLoaded = false;
}

/**
	Copy an array
	@arg		arrayIn		The array to read
	@arg		inStart		The index to start reading bytes from
	@arg		arrayOut	The read bytes are copied into this array
	@arg		outStart	Byte insertion starts at this index
	@arg		length		How many bytes to copy
*/
void ArrayCopy(const uint8_t* arrayIn, uint_fast32_t inStart, uint8_t* arrayOut, uint_fast32_t outStart, uint_fast32_t length)
{
	for(uint_fast32_t i = 0; i < length; ++i)
	{
		arrayOut[outStart + i] = arrayIn[inStart + i];
	}
}

/**
	Read 1 byte as a boolean value. 0 is false, everything else is true.
*/
bool BinaryReader::ReadBool()
{
	return (ReadUInt8() != 0);
}

/**
	Read 1 byte as a signed 8-bit integer
*/
int8_t BinaryReader::ReadInt8()
{
	return BYTESTOTYPE(int8_t);
}

/**
	Read 1 byte as an unsigned 8-bit integer
*/
uint8_t BinaryReader::ReadUInt8()
{
	return BYTESTOTYPE(uint8_t);
}

/**
	Read 2 bytes as a signed 16-bit integer
*/
int16_t BinaryReader::ReadInt16()
{
	return BYTESTOTYPE(int16_t);
}

/**
	Read 2 bytes as an unsigned 16-bit integer
*/
uint16_t BinaryReader::ReadUInt16()
{
	return BYTESTOTYPE(uint16_t);
}

/**
	Read 4 bytes as a signed 32-bit integer
*/
int32_t BinaryReader::ReadInt32()
{
	return BYTESTOTYPE(int32_t);
}

/**
	Read 4 bytes as an unsigned 32-bit integer
*/
uint32_t BinaryReader::ReadUInt32()
{
	return BYTESTOTYPE(uint32_t);
}

/**
	Read 8 bytes as a signed 64-bit integer
*/
int64_t BinaryReader::ReadInt64()
{
	return BYTESTOTYPE(int64_t);
}

/**
	Read 8 bytes as an unsigned 64-bit integer
*/
uint64_t BinaryReader::ReadUInt64()
{
	return BYTESTOTYPE(uint64_t);
}

#if defined(__GNUC__) && !defined(__MINGW32__) // MingW gives an error - does not appear to support __int128
/**
	Read 16 bytes as a signed 128-bit integer
*/
__int128 BinaryReader::ReadInt128()
{
	return BYTESTOTYPE(__int128);
}

/**
	Read 16 bytes as an unsigned 128-bit integer
*/
unsigned __int128 BinaryReader::ReadUInt128()
{
	return BYTESTOTYPE(unsigned __int128);
}
#endif

/**
	Read 4 bytes as a 32-bit floating-point number
*/
float BinaryReader::ReadFloat32()
{
	if(sizeof(float) == 4)
	{
		return BYTESTOTYPE(float);
	}
	else
	{
		throw MAKESTR("float size is " << sizeof(float) << " (expected 4)");
	}
}

/**
	Read 8 bytes as a 64-bit floating-point number
*/
double BinaryReader::ReadFloat64()
{
	if(sizeof(double) == 8)
	{
		return BYTESTOTYPE(double);
	}
	else
	{
		throw MAKESTR("double size is " << sizeof(double) << " (expected 8)");
	}
}

/**
	Read 16 bytes as a 128-bit floating-point number
*/
long double BinaryReader::ReadFloat128()
{
	if(sizeof(long double) == 16)
	{
		return BYTESTOTYPE(long double);
	}
	else
	{
		throw MAKESTR("long double size is " << sizeof(long double) << " (expected 16)");
	}
}

// TODO: check accuracy
/*uint32_t BinaryReader::Read7BitEncodedInt(uint8_t b)
{
	uint_fast32_t ret = 0;
	uint_fast32_t shift = 0;

	do
	{
		ret |= (b & 0x7f) << shift;
		shift += 7;
	}
	while((b & 0x80) == 0x80);

	return ret;
}*/

char* BinaryReader::ReadChars(uint_fast64_t bytes)
{
	if(!this->isLoaded)
	{
		throw MAKESTR("Called ReadChars(" << bytes << "), but no file is loaded");
	}

	char* buf = new char[bytes];

	if(this->usingArray)
	{
		for(uint_fast32_t i = 0; i < bytes; ++i)
		{
			buf[i] = this->data[this->pos + i];
		}
	}
	else
	{
		// seek to the current position in the loaded file
		fseek(this->file, this->pos, SEEK_SET);
		fread(buf, 1, bytes, this->file);
		if(ferror(this->file))
		{
			delete[] buf;
			throw MAKESTR("Error reading file: " << strerror(errno));
		}
	}

	// increment the current position
	this->pos += bytes;

	return buf;
}

uint8_t* BinaryReader::ReadBytes(uint_fast64_t bytes)
{
	if(!this->isLoaded)
	{
		throw MAKESTR("Called ReadBytes(" << bytes << "), but no file is loaded");
	}

	uint8_t* buf = new uint8_t[bytes];

	if(this->usingArray)
	{
		ArrayCopy(this->data, this->pos, buf, 0, bytes);
	}
	else
	{
		// seek to the current position in the loaded file
		fseek(this->file, this->pos, SEEK_SET);
		fread(buf, 1, bytes, this->file);
		if(ferror(this->file))
		{
			throw MAKESTR("Error reading file: " << strerror(errno));
		}
	}

	// increment the current position
	this->pos += bytes;

	return buf;
}

std::string BinaryReader::ReadString(uint_fast64_t length)
{
	char* buf = this->ReadChars(length);
	std::string ret(buf, length);
	delete buf;
	return ret;
}

// Derived from http://www.terrariaonline.com/threads/86509/
uint64_t BinaryReader::Read7BitEncodedInt()
{
	uint_fast64_t ret = 0;
	uint_fast8_t shift = 0;

	while(shift != 70) // maximum shifting is 7 * 9
	{
		uint8_t b = this->ReadUInt8();
		ret |= (b & 127) << shift;
		shift += 7;
		if((b & 128) == 0)
		{
			return ret;
		}
	}
	throw std::string("Failed to read a 7-bit encoded integer");
}

std::string BinaryReader::ReadStringMS()
{
	int len = this->Read7BitEncodedInt();
	return this->ReadString(len);
}
