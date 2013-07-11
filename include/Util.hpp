#ifndef UTIL_H
#define UTIL_H

#include <string.h> // for strerror
#include <errno.h> // for errno
#include <fstream> // for std::ifstream, remove, and rename
#include <iostream> // for std::cerr

class Util
{
	public:
		static bool fileExists(const char *filename);
		static bool fileDelete(const char *filename);
		static bool moveFile(std::string src, std::string dst, bool overwrite);
};

#endif // UTIL_H
