#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <stdint.h>
#include <string> // for std::string

class BinaryReader
{
	public:
		BinaryReader(std::string s);
		void ChangeFile(std::string s);
		void Close();
		char* getBytes(uint_fast32_t bytes);

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

		// 128-bit (16 bytes)
		__int128 ReadInt128();
		unsigned __int128 ReadUInt128();

		// 32-bit (4 bytes)
		float ReadFloat32();

		// 64-bit (8 bytes)
		double ReadFloat64();

		// 128-bit (16 bytes)
		long double ReadFloat128();

		std::string ReadString(uint64_t length);

		//uint32_t Read7BitEncodedInt(uint8_t b);
		uint64_t Read7BitEncodedInt();
		std::string ReadStringMS();

		bool isLoaded;
		std::string fname;
		int pos;
		FILE *file;
		long fSize;
};

#endif // BINARYREADER_H
