#pragma once

#include "BinaryLibUtil.hpp"

#include <cstdint>
#include <vector>

class BinaryWriter
{
	public:
		BinaryWriter(const std::string& filename, bool bak = false);
		~BinaryWriter();

		bool WriteBool(bool b);

		bool WriteInt8(int8_t c);
		bool WriteUInt8(uint8_t value);

		bool WriteInt16(int16_t i);
		bool WriteUInt16(uint16_t i);

		bool WriteInt32(int32_t i);
		bool WriteUInt32(uint32_t i);

		bool WriteInt64(int64_t i);
		bool WriteUInt64(uint64_t i);

		#if __SIZEOF_INT128__ == 16
		bool WriteInt128(__int128 i);
		bool WriteUInt128(unsigned __int128 i);
		#endif

		bool WriteFloat32(float value);
		bool WriteFloat64(double value);
		bool WriteFloat128(FLOAT16 value);

		bool WriteChars(const char* c, uint64_t bufSize);
		bool WriteChars(const char* c, uint64_t bufSize, uint64_t len, uint64_t startpos = 0);
		bool WriteBytes(const uint8_t* c, uint64_t bufSize);
		bool WriteBytes(const uint8_t* c, uint64_t bufSize, uint64_t len, uint64_t startpos = 0);
		bool WriteBytes(const std::vector<uint8_t>& bytes);

		bool WriteString(std::string s);

		bool Write7BitEncodedInt(uint64_t value);
		bool WriteStringMS(std::string s);

	private:
		template <class type>
		bool type_to_bytes(type value);

		std::string filename;
		FILE* file = nullptr;
};