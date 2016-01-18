#pragma once

#include <cstring> // for strerror and errno
#include <fstream> // for std::ifstream, remove, and rename
#include <iostream> // for std::cerr

#define BINARYLIB_VERSION 11

#define FLOAT16 long double

// not supported by clang
//#define FLOAT16 __float128

class BinaryLibUtil
{
	public:
		static bool fileExists(const char *filename);
		static bool fileDelete(const char *filename);
		static bool moveFile(const std::string& src, const std::string& dst, const bool overwrite);
		static uint32_t version();
};