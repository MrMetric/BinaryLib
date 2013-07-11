#include "../include/BinaryWriter.hpp"

/**
	BinaryWriter.cpp
	This class is my implementation of Microsoft's C# BinaryWriter
	It writes the following to a file:
	 - the bytes in a signed or unsigned integer (8-bit, 16-bit, 32-bit, 64-bit, or 128-bit)
	 - single characters (8-bit int) or strings of characters (in char*)
	 - Microsoft-style strings (one byte before the string specifies the length)

	@todo		Make an exception class to throw when errors occur
	@arg s		The file to write
	@arg bak	If this is true, the pre-existing file (if there is one) with the name
				specified in the previous argument will have .bak appended to its name
*/

BinaryWriter::BinaryWriter(std::string s, bool bak)
{
	this->isLoaded = false;
	this->fname = s;
	this->totalBytes = 0;
	if(Util::fileExists(s.c_str()))
	{
		if(bak)
		{
			std::stringstream ss;
			ss << s << ".bak";
			Util::moveFile(s.c_str(), ss.str().c_str(), true);
		}
		else
		{
			Util::fileDelete(s.c_str());
		}
	}
	this->file = fopen(s.c_str(), "ar+b");
	if(this->file == NULL)
	{
		std::cerr << "BinaryWriter: Error opening file: " << strerror(errno) << "\n";
	}
	else if(ferror(this->file))
	{
		perror("BinaryWriter: Error opening file");
	}
	else
	{
		this->isLoaded = true;
	}
}

void BinaryWriter::addBytes(int i)
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
	else // TODO: throw an exception
	{
		throw -1;
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
		std::cerr << "BinaryReader: __int128 size is " << sizeof(__int128) << " (expected 16)\n";
		throw -1;
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
		std::cerr << "BinaryReader: unsigned __int128 size is " << sizeof(unsigned __int128) << " (expected 16)\n";
		throw -1;
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
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryWriter: float size is " << sizeof(float) << " (expected 4)\n";
		throw -1;
	}
}

bool BinaryWriter::WriteFloat64(double value)
{
	if(sizeof(double) == 8)
	{
		return this->WriteInt64(*(int64_t*)&value);
	}
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryWriter: double size is " << sizeof(double) << " (expected 8)\n";
		throw -1;
	}
}

bool BinaryWriter::WriteFloat128(FLOAT16 value)
{
	if(sizeof(FLOAT16) == 16)
	{
		return this->WriteInt128(*(__int128*)&value);
	}
	else // TODO: throw a proper exception
	{
		std::cerr << "BinaryWriter: long double size is " << sizeof(FLOAT16) << " (expected 16)\n";
		throw -1;
	}
}

// This is faster than using WriteInt8 in a loop
bool BinaryWriter::WriteChars(int8_t* c, int len)
{
	if(!this->isLoaded) return false;
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
bool BinaryWriter::WriteBytes(uint8_t* c, int len)
{
	if(!this->isLoaded) return false;
	addBytes(len);
	fwrite(c, 1, len, this->file);
	if(ferror(this->file))
	{
		perror("BinaryWriter: Error writing file is WriteBytes");
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
