#include "../include/BinaryWriter.hpp"
#include <sstream> // for std::stringstream
#include <stdexcept> // for std::out_of_range

#define MAKESTR(ss) static_cast<std::ostringstream&>(std::ostringstream().seekp(0) << ss).str()

/**
	BinaryWriter.cpp
	This class is my implementation of Microsoft's C# BinaryWriter
	It writes the following to a file:
	 - the bytes in a signed or unsigned integer (8-bit, 16-bit, 32-bit, 64-bit, or 128-bit)
	 - single characters (8-bit int) or strings of characters (in char*)
	 - Microsoft-style strings (one byte before the string specifies the length)

	@todo		Make an exception class to throw when errors occur instead of using std::string
	@arg s		The file to write
	@arg bak	If this is true, the pre-existing file (if there is one) with the name
				specified in the previous argument will have .bak appended to its name
*/

BinaryWriter::BinaryWriter(std::string s, bool bak)
{
	this->isLoaded = false;
	this->fname = s;
	this->totalBytes = 0;
	if(BinaryLibUtil::fileExists(s.c_str()))
	{
		if(bak)
		{
			std::string s_bak = s + std::string(".bak");
			BinaryLibUtil::moveFile(s.c_str(), s_bak.c_str(), true);
		}
		else
		{
			BinaryLibUtil::fileDelete(s.c_str());
		}
	}
	this->file = fopen(s.c_str(), "ar+b");
	if(this->file == NULL || ferror(this->file))
	{
		throw MAKESTR("BinaryWriter: Error opening file: " << strerror(errno));
	}
	else
	{
		this->isLoaded = true;
	}
}

inline void BinaryWriter::addBytes(uint_fast32_t i)
{
	this->totalBytes += i;
	// I used this for debugging
	//cout << "Wrote " << i << (i==1?" byte":" bytes") << endl;
}

void BinaryWriter::Close()
{
	if(this->isLoaded)
	{
		fclose(this->file);
	}
	else
	{
		throw std::string("BinaryWriter: Called Close(), but no file is open");
	}
}

bool BinaryWriter::WriteBool(bool b)
{
	if(!this->isLoaded) return false;
	addBytes(1);
	char buf[1];
	buf[0] = (b?1:0);
	fwrite(buf, 1, 1, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteBool");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteInt8(int8_t c)
{
	if(!this->isLoaded) return false;
	++this->totalBytes;
	char buf[1];
	buf[0] = c;
	fwrite(buf, 1, 1, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteInt8");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteUInt8(uint8_t value)
{
	if(!this->isLoaded) return false;
	addBytes(1);
	char buf[1];
	buf[0] = value;
	fwrite(buf, 1, 1, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteUInt8");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteInt16(int16_t i)
{
	if(!this->isLoaded) return false;
	addBytes(2);
	char buf[2];
	buf[0] = i;
	buf[1] = (i >> 8);
	fwrite(buf, 1, 2, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteInt16");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteUInt16(uint16_t i)
{
	if(!this->isLoaded) return false;
	addBytes(2);
	char buf[2];
	buf[0] = i;
	buf[1] = (i >> 8);
	fwrite(buf, 1, 2, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteUInt16");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteInt32(int32_t i)
{
	if(!this->isLoaded) return false;
	addBytes(4);
	char buf[4];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	fwrite(buf, 1, 4, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteInt32");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteUInt32(uint32_t i)
{
	if(!this->isLoaded) return false;
	addBytes(4);
	char buf[4];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	fwrite(buf, 1, 4, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteUInt32");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteInt64(int64_t i)
{
	if(!this->isLoaded) return false;
	addBytes(8);
	char buf[8];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	buf[4] = (i >> 32);
	buf[5] = (i >> 40);
	buf[6] = (i >> 48);
	buf[7] = (i >> 56);
	fwrite(buf, 1, 8, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteInt64");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteUInt64(uint64_t i)
{
	if(!this->isLoaded) return false;
	addBytes(8);
	char buf[8];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	buf[4] = (i >> 32);
	buf[5] = (i >> 40);
	buf[6] = (i >> 48);
	buf[7] = (i >> 56);
	fwrite(buf, 1, 8, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteUInt64");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteInt128(__int128 i)
{
	if(!this->isLoaded)
	{
		return false;
	}
	if(sizeof(__int128) != 16)
	{
		throw MAKESTR("BinaryReader: __int128 size is " << sizeof(__int128) << " (expected 16)");
	}

	addBytes(16);
	char buf[16];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	buf[4] = (i >> 32);
	buf[5] = (i >> 40);
	buf[6] = (i >> 48);
	buf[7] = (i >> 56);
	buf[8] = (i >> 64);
	buf[9] = (i >> 72);
	buf[10] = (i >> 80);
	buf[11] = (i >> 88);
	buf[12] = (i >> 96);
	buf[13] = (i >> 104);
	buf[14] = (i >> 112);
	buf[15] = (i >> 120);
	fwrite(buf, 1, 16, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteInt128");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteUInt128(unsigned __int128 i)
{
	if(!this->isLoaded)
	{
		return false;
	}
	if(sizeof(unsigned __int128) != 16)
	{
		throw MAKESTR("BinaryReader: unsigned __int128 size is " << sizeof(unsigned __int128) << " (expected 16)");
	}

	addBytes(16);
	char buf[16];
	buf[0] = i;
	buf[1] = (i >> 8);
	buf[2] = (i >> 16);
	buf[3] = (i >> 24);
	buf[4] = (i >> 32);
	buf[5] = (i >> 40);
	buf[6] = (i >> 48);
	buf[7] = (i >> 56);
	buf[8] = (i >> 64);
	buf[9] = (i >> 72);
	buf[10] = (i >> 80);
	buf[11] = (i >> 88);
	buf[12] = (i >> 96);
	buf[13] = (i >> 104);
	buf[14] = (i >> 112);
	buf[15] = (i >> 120);
	fwrite(buf, 1, 16, this->file);
	if(ferror(this->file))
	{
		perror("Error writing file in WriteUInt128");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteFloat32(float value)
{
	if(sizeof(float) == 4)
	{
		return this->WriteInt32(*(int32_t*)&value);
	}
	else
	{
		throw MAKESTR("BinaryWriter: float size is " << sizeof(float) << " (expected 4)");
	}
}

bool BinaryWriter::WriteFloat64(double value)
{
	if(sizeof(double) == 8)
	{
		return this->WriteInt64(*(int64_t*)&value);
	}
	else
	{
		throw MAKESTR("BinaryWriter: double size is " << sizeof(double) << " (expected 8)");
	}
}

bool BinaryWriter::WriteFloat128(FLOAT16 value)
{
	if(sizeof(FLOAT16) == 16)
	{
		return this->WriteInt128(*(__int128*)&value);
	}
	else
	{
		throw MAKESTR("BinaryWriter: long double size is " << sizeof(FLOAT16) << " (expected 16)");
	}
}

// This is faster than using WriteInt8 in a loop
bool BinaryWriter::WriteChars(int8_t* c, uint64_t len, uint64_t startpos)
{
	if(!this->isLoaded) return false;

	if(startpos != 0)
	{
		if(startpos == len)
		{
			std::cout << "BinaryWriter: Warning: startpos == len in WriteChars\n";
			return true;
		}

		try
		{
			len -= startpos;
			std::basic_string<int8_t> s = c;
			s = s.substr(startpos);
			c = (int8_t*)s.c_str();
		}
		catch(std::out_of_range e)
		{
			std::cerr << "BinaryWriter: Got std::out_of_range exception in WriteChars\nlen: " << len << " (" << len + startpos << ")\nstartpos: " << startpos << "\n";
			throw e;
		}
	}
	addBytes(len);
	fwrite(c, 1, len, this->file);
	if(ferror(this->file))
	{
		perror("BinaryWriter: Error writing file in WriteChars");
		return false;
	}
	return true;
}

// This is faster than using WriteUInt8 in a loop
bool BinaryWriter::WriteBytes(uint8_t* c, uint64_t len, uint64_t startpos)
{
	if(!this->isLoaded) return false;

	if(startpos != 0)
	{
		if(startpos == len)
		{
			std::cout << "BinaryWriter: Warning: startpos == len in WriteBytes\n";
			return true;
		}

		try
		{
			len -= startpos;
			std::basic_string<uint8_t> s = c;
			s = s.substr(startpos);
			c = (uint8_t*)s.c_str();
		}
		catch(std::out_of_range e)
		{
			std::cerr << "BinaryWriter: Got std::out_of_range exception in WriteBytes\nlen: " << len << " (" << len + startpos << ")\nstartpos: " << startpos << "\n";
			throw e;
		}
	}
	addBytes(len);
	fwrite(c, 1, len, this->file);
	if(ferror(this->file))
	{
		perror("BinaryWriter: Error writing file in WriteBytes");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteString(std::string s)
{
	return this->WriteChars((int8_t*)s.c_str(), s.length());
}

// TODO: check the return value of WriteUInt8
void BinaryWriter::Write7BitEncodedInt(uint32_t value)
{
	while(value >= 128)
	{
		WriteUInt8(static_cast<unsigned char>(value | 0x80));
		value >>= 7;
	}
	WriteUInt8(static_cast<unsigned char>(value));
}

bool BinaryWriter::WriteStringMS(std::string s)
{
	if(!this->isLoaded) return false;
	int len = s.length();
	addBytes(len);
	Write7BitEncodedInt(len);
	fwrite(s.c_str(), 1, len, this->file);
	if(ferror(this->file))
	{
		perror("BinaryWriter: Error writing file in WriteStringMS");
		return false;
	}
	return true;
}
