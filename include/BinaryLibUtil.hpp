#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <string.h> // for strerror and errno
#include <fstream> // for std::ifstream, remove, and rename
#include <iostream> // for std::cerr

#define BINARYLIB_VERSION 8

class BinaryLibUtil
{
	public:
		static bool fileExists(const char *filename);
		static bool fileDelete(const char *filename);
		static bool moveFile(std::string src, std::string dst, bool overwrite);
		static uint32_t version();
};

#endif // UTIL_H
