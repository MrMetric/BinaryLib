#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <stdint.h>
#include <string> // for std::string

class BinaryReader
{
	public:
		BinaryReader(std::string s);
		BinaryReader(uint8_t* data, const uint_fast64_t size);
		void ChangeFile(std::string s);
		void ChangeFile(uint8_t* data, const uint_fast64_t size);
		void Close();

		// 8-bit (1 byte)
		bool ReadBool();

		// 8-bit (1 byte)
		int8_t ReadInt8();
		uint8_t ReadUInt8();

		// 16-bit (2 bytes)
		int16_t ReadInt16();
		uint16_t ReadUInt16();

		// 32-bit (4 bytes)
		int32_t ReadInt32();
		uint32_t ReadUInt32();

		// 64-bit (8 bytes)
		int64_t ReadInt64();
		uint64_t ReadUInt64();

		#if defined(__GNUC__) && !defined(__MINGW32__) // MingW gives an error - does not appear to support __int128
		// 128-bit (16 bytes)
		__int128 ReadInt128();
		unsigned __int128 ReadUInt128();
		#endif

		// 32-bit (4 bytes)
		float ReadFloat32();

		// 64-bit (8 bytes)
		double ReadFloat64();

		// 128-bit (16 bytes)
		long double ReadFloat128();

		char* ReadChars(uint_fast64_t bytes);
		uint8_t* ReadBytes(uint_fast64_t bytes);

		std::string ReadString(uint_fast64_t length);

		//uint32_t Read7BitEncodedInt(uint8_t b);
		uint64_t Read7BitEncodedInt();
		std::string ReadStringMS();

		bool isLoaded;
		bool usingArray;
		uint8_t* data;
		std::string fname;
		uint_fast64_t pos;
		FILE* file;
		uint_fast64_t fSize;
};

#endif // BINARYREADER_H
