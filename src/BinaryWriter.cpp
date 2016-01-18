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

BinaryWriter::BinaryWriter(const std::string& filename, bool bak)
	:
	filename(filename)
{
	if(BinaryLibUtil::fileExists(filename.c_str()))
	{
		if(bak)
		{
			std::string s_bak = filename + std::string(".bak");
			BinaryLibUtil::moveFile(filename.c_str(), s_bak.c_str(), true);
		}
		else
		{
			BinaryLibUtil::fileDelete(filename.c_str());
		}
	}
	this->file = fopen(filename.c_str(), "wb");
	if(this->file == nullptr || ferror(this->file))
	{
		throw MAKESTR("BinaryWriter: Error opening \"" << filename << "\": " << strerror(errno));
	}
}

BinaryWriter::~BinaryWriter()
{
	if(this->file != nullptr)
	{
		fclose(this->file);
		this->file = nullptr;
	}
}

bool BinaryWriter::WriteBool(bool b)
{
	return this->WriteUInt8(b ? 1 : 0);
}

bool BinaryWriter::WriteInt8(int8_t value)
{
	return this->type_to_bytes<int8_t>(value);
}

bool BinaryWriter::WriteUInt8(uint8_t value)
{
	return this->type_to_bytes<uint8_t>(value);
}

bool BinaryWriter::WriteInt16(int16_t i)
{
	return this->type_to_bytes<int16_t>(i);
}

bool BinaryWriter::WriteUInt16(uint16_t i)
{
	return this->type_to_bytes<uint16_t>(i);
}

bool BinaryWriter::WriteInt32(int32_t i)
{
	return this->type_to_bytes<int32_t>(i);
}

bool BinaryWriter::WriteUInt32(uint32_t i)
{
	return this->type_to_bytes<uint32_t>(i);
}

bool BinaryWriter::WriteInt64(int64_t i)
{
	return this->type_to_bytes<int64_t>(i);
}

bool BinaryWriter::WriteUInt64(uint64_t i)
{
	return this->type_to_bytes<uint64_t>(i);
}

#if __SIZEOF_INT128__ == 16
bool BinaryWriter::WriteInt128(__int128 i)
{
	static_assert(sizeof(__int128) == 16, "__int128 must be 16 bytes");
	return this->type_to_bytes<__int128>(i);
}

bool BinaryWriter::WriteUInt128(unsigned __int128 i)
{
	static_assert(sizeof(unsigned __int128) == 16, "unsigned __int128 must be 16 bytes");
	return this->type_to_bytes<unsigned __int128>(i);
}
#endif

bool BinaryWriter::WriteFloat32(float value)
{
	static_assert(sizeof(float) == 4, "float must be 4 bytes");
	return this->type_to_bytes<float>(value);
}

bool BinaryWriter::WriteFloat64(double value)
{
	static_assert(sizeof(double) == 8, "double must be 8 bytes");
	return this->type_to_bytes<double>(value);
}

bool BinaryWriter::WriteFloat128(FLOAT16 value)
{
	#if __SIZEOF_INT128__ == 16
	static_assert(sizeof(FLOAT16) == 16, "FLOAT16 must be 16 bytes");
	return this->type_to_bytes<FLOAT16>(value);
	#else
	throw std::string("WriteFloat128 depends on the WriteInt128 function");
	#endif
}

bool BinaryWriter::WriteChars(const char* c, uint64_t bufSize)
{
	return this->WriteChars(c, bufSize, bufSize);
}

bool BinaryWriter::WriteChars(const char* c, uint64_t bufSize, uint64_t len, uint64_t startpos)
{
	static_assert(sizeof(char) == 1, "char must be 1 byte");
	return this->WriteBytes(reinterpret_cast<const uint8_t*>(c), bufSize, len, startpos);
}

bool BinaryWriter::WriteBytes(const uint8_t* c, uint64_t bufSize)
{
	return this->WriteBytes(c, bufSize, bufSize);
}

bool BinaryWriter::WriteBytes(const uint8_t* c, uint64_t bufSize, uint64_t len, uint64_t startpos)
{
	if(this->file == nullptr)
	{
		return false;
	}

	if(startpos != 0)
	{
		return this->WriteBytes(c + startpos, bufSize - startpos, len);
	}
	if(len > bufSize)
	{
		throw MAKESTR("BinaryWriter: len > bufSize (" << len << " > " << bufSize << ")");
	}

	fwrite(c, sizeof(uint8_t), len, this->file);
	if(ferror(this->file))
	{
		perror("BinaryWriter: Error writing file in WriteBytes");
		return false;
	}
	return true;
}

bool BinaryWriter::WriteBytes(const std::vector<uint8_t>& bytes)
{
	return this->WriteBytes(&bytes[0], bytes.size());
}

bool BinaryWriter::WriteString(std::string s)
{
	return this->WriteChars(s.c_str(), s.length());
}

// WARNING: this might not be accurate for large values!
bool BinaryWriter::Write7BitEncodedInt(uint64_t value)
{
	while(value >= 128)
	{
		if(!WriteUInt8(static_cast<uint8_t>(value | 0x80)))
		{
			return false;
		}
		value >>= 7;
	}
	return WriteUInt8(static_cast<uint8_t>(value));
}

bool BinaryWriter::WriteStringMS(std::string s)
{
	size_t len = s.length();
	return this->Write7BitEncodedInt(len) && this->WriteChars(s.c_str(), len);
}

template <class type>
bool BinaryWriter::type_to_bytes(type value)
{
	/*
	uint8_t buf[sizeof(type)];
	for(uint_fast8_t a = 0; a < sizeof(type); ++a)
	{
		buf[a] = static_cast<uint8_t>(value >> (a * 8));
	}
	return this->WriteBytes(buf, sizeof(buf));*/
	return this->WriteBytes(reinterpret_cast<uint8_t*>(&value), sizeof(type));
}