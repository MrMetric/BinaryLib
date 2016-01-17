#pragma once

#include "BinaryLibUtil.hpp"

#include <cstdint>
#include <string>

class BinaryReader
{
	public:
		explicit BinaryReader(const std::string& s);
		BinaryReader(uint8_t* data, const uint_fast64_t size);
		~BinaryReader();

		void ChangeFile(const std::string& s);
		void ChangeFile(uint8_t* data, const uint_fast64_t size);
		void Close();

		template <class type> type bytes_to_type();

		bool ReadBool();

		int8_t ReadInt8();
		uint8_t ReadUInt8();

		int16_t ReadInt16();
		uint16_t ReadUInt16();

		int32_t ReadInt32();
		uint32_t ReadUInt32();

		int64_t ReadInt64();
		uint64_t ReadUInt64();

		#if __SIZEOF_INT128__ == 16
		__int128 ReadInt128();
		unsigned __int128 ReadUInt128();
		#endif

		float ReadFloat32();

		double ReadFloat64();

		long double ReadFloat128();

		char* ReadChars(uint_fast64_t bytes);
		uint8_t* ReadBytes(uint_fast64_t bytes);

		std::string ReadString(uint_fast64_t length);

		uint64_t Read7BitEncodedInt();
		std::string ReadStringMS();

		bool isLoaded = false;
		bool usingArray;
		uint8_t* data = nullptr;
		std::string fname;
		uint_fast64_t pos;
		FILE* file = nullptr;
		uint_fast64_t fSize;
};