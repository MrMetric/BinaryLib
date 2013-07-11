#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <stdint.h>
#include <sstream> // for std::stringstream
#include "Util.hpp" // for Util functions and the file functions in stdio.h (included in fstream)

// if you change this, don't forget to recompile the library!
#define FLOAT16 long double
//#define FLOAT16 __float128

class BinaryWriter
{
	public:
		BinaryWriter(std::string s, bool bak = true);
		void addBytes(int i);
		void Close();

		void Write7BitEncodedInt(int value);
		bool WriteBool(bool b);

		bool WriteInt8(int8_t c);
		bool WriteUInt8(uint8_t value);

		bool WriteInt16(int16_t i);
		bool WriteUInt16(uint16_t i);

		bool WriteInt32(int32_t i);
		bool WriteUInt32(uint32_t i);

		bool WriteInt64(int64_t i);
		bool WriteUInt64(uint64_t i);

		bool WriteInt128(__int128 i);
		bool WriteUInt128(unsigned __int128 i);

		bool WriteFloat4(float value);
		bool WriteFloat8(double value);
		bool WriteFloat16(FLOAT16 value);

		bool WriteBytes(unsigned char *c, int len);
		bool WriteBytes(unsigned char *c, int startpos, int len);
		bool WriteChars(char *c, int len);
		bool WriteStringMS(std::string s);

		bool isLoaded;
		std::string fname;
		FILE *file;
		uint_fast32_t totalBytes;
};

#endif // BINARYWRITER_H
