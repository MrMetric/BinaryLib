#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <stdint.h>
#include "BinaryLibUtil.hpp" // for Util functions and the file functions in stdio.h (included in fstream)

class BinaryWriter
{
	public:
		BinaryWriter(std::string s, bool bak = true);
		void Close();

		bool WriteBool(bool b);

		bool WriteInt8(int8_t c);
		bool WriteUInt8(uint8_t value);

		bool WriteInt16(int16_t i);
		bool WriteUInt16(uint16_t i);

		bool WriteInt32(int32_t i);
		bool WriteUInt32(uint32_t i);

		bool WriteInt64(int64_t i);
		bool WriteUInt64(uint64_t i);

		#if defined(__GNUC__) && !defined(__MINGW32__) // MingW gives an error - does not appear to support __int128
		bool WriteInt128(__int128 i);
		bool WriteUInt128(unsigned __int128 i);
		#endif

		bool WriteFloat32(float value);
		bool WriteFloat64(double value);
		bool WriteFloat128(FLOAT16 value);

		bool WriteChars(const char* c, uint64_t bufSize, uint64_t len, uint64_t startpos = 0);
		bool WriteBytes(const uint8_t* c, uint64_t bufSize, uint64_t len, uint64_t startpos = 0);

		bool WriteString(std::string s);

		void Write7BitEncodedInt(uint64_t value);
		bool WriteStringMS(std::string s);

		bool isLoaded;
		std::string fname;
		FILE *file;
		uint_fast32_t totalBytes;
};

#endif // BINARYWRITER_H
