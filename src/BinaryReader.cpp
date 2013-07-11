#include "../include/BinaryReader.hpp"

/**
	BinaryReader.cpp
	This class is my implementation of Microsoft's C# BinaryReader
	It reads bytes from a file and returns the following:
	 - signed and unsigned integer values (8-bit, 16-bit, 32-bit, 64-bit, or 128-bit)
	 - single characters (8-bit int) or strings of characters (in std::string)
	It can read Microsoft-style strings (one byte before the string specifies the length)

	@todo		Check return values of fread
	@todo		Make an exception class to throw when errors occur
	@arg		s The file to read
*/

BinaryReader::BinaryReader(std::string s)
{
	this->isLoaded = false;
	this->ChangeFile(s);
}

void BinaryReader::ChangeFile(std::string s)
{
	if(this->isLoaded) this->Close();
	this->fname = s;
	this->pos = 0;
	this->file = fopen(s.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	this->fSize = ftell(file);
	rewind(file);
	if(ferror(this->file))
	{
		perror("Error opening file");
	}
	else this->isLoaded = true;
}

void BinaryReader::Close()
{
	fclose(this->file);
	this->isLoaded = false;
}

char* BinaryReader::getBytes(uint_fast32_t bytes)
{
	if(!this->isLoaded)
	{
		std::cerr << "BinaryReader: called getBytes(" << bytes << "), but no file is loaded\n";
		throw -1; // TODO: throw an exception
	}

	// seek to the current position in the loaded file
	fseek(this->file, this->pos, SEEK_SET);

	// increment the current position for the next call to fseek
	pos += bytes;

	// make a char array and read data into it
	char buf[bytes];
	fread(buf, 1, bytes, this->file);
	if(ferror(this->file))
	{
		perror("Error reading file");
	}

	// get a pointer to the data and return it
	char* ret = buf;
	return ret;
}

bool BinaryReader::ReadBool()
{
	return (ReadUInt8() != 0);
}

int8_t BinaryReader::ReadInt8()
{
	return *(reinterpret_cast<int8_t*>(getBytes(1)));
}

uint8_t BinaryReader::ReadUInt8()
{
	return *(reinterpret_cast<uint8_t*>(getBytes(1)));
}

int16_t BinaryReader::ReadInt16()
{
	return *(reinterpret_cast<int16_t*>(getBytes(2)));
}

uint16_t BinaryReader::ReadUInt16()
{
	/*char tmp1 = buf[0];
	buf[0] = buf[1];
	buf[1] = tmp1;*/
	return *(reinterpret_cast<uint16_t*>(getBytes(2)));
}

int32_t BinaryReader::ReadInt32()
{
	// this cast is from Matt Davis at http://stackoverflow.com/questions/544928/reading-integer-size-bytes-from-a-char-array
	return *(reinterpret_cast<int32_t*>(getBytes(4)));
}

uint32_t BinaryReader::ReadUInt32()
{
	return *(reinterpret_cast<uint32_t*>(getBytes(4)));
}

int64_t BinaryReader::ReadInt64()
{
	return *(reinterpret_cast<int64_t*>(getBytes(8)));
}

uint64_t BinaryReader::ReadUInt64()
{
	return *(reinterpret_cast<uint64_t*>(getBytes(8)));
}

__int128 BinaryReader::ReadInt128()
{
	return *(reinterpret_cast<__int128*>(getBytes(16)));
}

unsigned __int128 BinaryReader::ReadUInt128()
{
	return *(reinterpret_cast<unsigned __int128*>(getBytes(16)));
}

float BinaryReader::ReadFloat32()
{
	if(sizeof(float) == 4)
	{
		return *(reinterpret_cast<float*>(getBytes(4)));
	}
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryReader: float size is " << sizeof(float) << " (expected 4)\n";
		throw -1;
	}
}

double BinaryReader::ReadFloat64()
{
	if(sizeof(double) == 8)
	{
		return *(reinterpret_cast<double*>(getBytes(8)));
	}
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryReader: double size is " << sizeof(double) << " (expected 8)\n";
		throw -1;
	}
}

long double BinaryReader::ReadFloat128()
{
	if(sizeof(long double) == 16)
	{
		return *(reinterpret_cast<long double*>(getBytes(16)));
	}
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryReader: long double size is " << sizeof(long double) << " (expected 16)\n";
		throw -1;
	}
}

std::string BinaryReader::ReadString(uint64_t length)
{
	char* bytes = getBytes(length);
	uint8_t buf[length];
	for(uint64_t c = 0; c < length; ++c)
	{
		buf[c] = bytes[c];
	}
	std::string s(reinterpret_cast<const char*>(buf), length);
	if(s.length() > length)
	{
		s = s.substr(0, length);
	}
	return s;
}

// TODO: check accuracy
/*uint32_t BinaryReader::Read7BitEncodedInt(uint8_t b)
{
	uint_fast32_t ret = 0;
	uint_fast32_t shift = 0;

	do
	{
		ret = ret | ((b & 0x7f) << shift);
		shift += 7;
	} while((b & 0x80) == 0x80);
	return ret;
}*/


// Derived from http://www.terrariaonline.com/threads/86509/
uint32_t BinaryReader::Read7BitEncodedInt()
{
	int_fast32_t num = 0;
	int_fast32_t num2 = 0;
	while(num2 != 35)
	{
		uint8_t b = this->ReadUInt8();
		num |= (int_fast32_t)(b & 127) << num2;
		num2 += 7;
		if((b & 128) == 0)
		{
			return num;
		}
	}
	//throw new FormatException("Failed to read a Microsoft 7-bit encoded integer");
	std::cerr << "BinaryReader: Failed to read a Microsoft 7-bit encoded integer\n";
	throw -1;
}

std::string BinaryReader::ReadStringMS()
{
	int len = this->Read7BitEncodedInt();
	return this->ReadString(len);
}
