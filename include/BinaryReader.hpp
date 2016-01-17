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

		bool ReadBool();

		int8_t ReadInt8();
		uint8_t ReadUInt8();

		int16_t ReadInt16();
		uint16_t ReadUInt16();

		int32_t ReadInt32();
		uint32_t ReadUInt32();

		int64_t ReadInt64();
		uint64_t ReadUInt64();

		#if defined(__GNUC__) && !defined(__MINGW32__) // MingW gives an error - does not appear to support __int128
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

		bool isLoaded;
		bool usingArray;
		uint8_t* data;
		std::string fname;
		uint_fast64_t pos;
		FILE* file;
		uint_fast64_t fSize;
};

#endif // BINARYREADER_H