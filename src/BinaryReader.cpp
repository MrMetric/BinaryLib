#include "../include/BinaryReader.hpp"
#include <stdio.h> // for file functions
#include <sstream>
#include <string.h> // for strerror and errno
#include <iostream>
#include <algorithm> // std::copy_n

#define MAKESTR(ss) static_cast<std::ostringstream&>(std::ostringstream().seekp(0) << ss).str()

template <class type> type BinaryReader::bytes_to_type()
{
	uint8_t* buf = this->ReadBytes(sizeof(type));
	type ret = *(reinterpret_cast<type*>(buf));
	delete[] buf;
	return ret;
}

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
BinaryReader::BinaryReader(const std::string& s)
{
	this->ChangeFile(s);
}

/**
	@arg		data The byte array to read
	@arg		size The size (in bytes) of the byte array
*/
BinaryReader::BinaryReader(uint8_t* data, const uint_fast64_t size)
{
	this->ChangeFile(data, size);
}

BinaryReader::~BinaryReader()
{
	if(this->isLoaded)
	{
		this->Close();
	}
}

/**
	Close the current file, if one is loaded, and open a file
	@arg		s The file to read
*/
void BinaryReader::ChangeFile(const std::string& s)
{
	if(this->isLoaded)
	{
		this->Close();
	}

	this->fname = s;
	this->pos = 0;
	this->data = nullptr;
	this->file = fopen(s.c_str(), "rb");
	if(this->file == nullptr || ferror(this->file))
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
	if(!this->isLoaded)
	{
		return;
	}

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
	return this->bytes_to_type<int8_t>();
}

/**
	Read 1 byte as an unsigned 8-bit integer
*/
uint8_t BinaryReader::ReadUInt8()
{
	return this->bytes_to_type<uint8_t>();
}

/**
	Read 2 bytes as a signed 16-bit integer
*/
int16_t BinaryReader::ReadInt16()
{
	return this->bytes_to_type<int16_t>();
}

/**
	Read 2 bytes as an unsigned 16-bit integer
*/
uint16_t BinaryReader::ReadUInt16()
{
	return this->bytes_to_type<uint16_t>();
}

/**
	Read 4 bytes as a signed 32-bit integer
*/
int32_t BinaryReader::ReadInt32()
{
	return this->bytes_to_type<int32_t>();
}

/**
	Read 4 bytes as an unsigned 32-bit integer
*/
uint32_t BinaryReader::ReadUInt32()
{
	return this->bytes_to_type<uint32_t>();
}

/**
	Read 8 bytes as a signed 64-bit integer
*/
int64_t BinaryReader::ReadInt64()
{
	return this->bytes_to_type<int64_t>();
}

/**
	Read 8 bytes as an unsigned 64-bit integer
*/
uint64_t BinaryReader::ReadUInt64()
{
	return this->bytes_to_type<uint64_t>();
}

#if __SIZEOF_INT128__ == 16
/**
	Read 16 bytes as a signed 128-bit integer
*/
__int128 BinaryReader::ReadInt128()
{
	return this->bytes_to_type<__int128>();
}

/**
	Read 16 bytes as an unsigned 128-bit integer
*/
unsigned __int128 BinaryReader::ReadUInt128()
{
	return this->bytes_to_type<unsigned __int128>();
}
#endif

/**
	Read 4 bytes as a 32-bit floating-point number
*/
float BinaryReader::ReadFloat32()
{
	if(sizeof(float) == 4)
	{
		return this->bytes_to_type<float>();
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
		return this->bytes_to_type<double>();
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
	if(sizeof(FLOAT16) == 16)
	{
		return this->bytes_to_type<FLOAT16>();
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
			uint8_t byte = this->data[this->pos + i];
			buf[i] = *reinterpret_cast<char*>(&byte);
		}
	}
	else
	{
		fseek(this->file, this->pos, SEEK_SET);
		fread(buf, 1, bytes, this->file);
		if(ferror(this->file))
		{
			delete[] buf;
			throw MAKESTR("Error reading file: " << strerror(errno));
		}
	}

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
		std::copy_n(this->data + this->pos, bytes, buf);
	}
	else
	{
		fseek(this->file, this->pos, SEEK_SET);
		fread(buf, 1, bytes, this->file);
		if(ferror(this->file))
		{
			throw MAKESTR("Error reading file: " << strerror(errno));
		}
	}

	this->pos += bytes;

	return buf;
}

std::string BinaryReader::ReadString(uint_fast64_t length)
{
	char* buf = this->ReadChars(length);
	std::string ret(buf, length);
	delete[] buf;
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
	auto len = this->Read7BitEncodedInt();
	return this->ReadString(len);
}
