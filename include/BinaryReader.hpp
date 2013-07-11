#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <stdint.h>
#include <stdio.h> // for file functions
#include <string> // for std::string
#include <iostream> // for std::cout and std::cerr

class BinaryReader
{
	public:
		BinaryReader(std::string s);
		void ChangeFile(std::string s);
		void Close();
		char* getBytes(uint_fast32_t bytes);

		int Read7BitEncodedInt(uint_fast8_t b);

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

		float ReadFloat4();
		double ReadFloat8();
		long double ReadFloat16();

		std::string ReadString(uint_fast32_t length);
		std::string ReadStringMS();

		bool isLoaded;
		std::string fname;
		int pos;
		FILE *file;
		long fSize;
};

#endif // BINARYREADER_H
