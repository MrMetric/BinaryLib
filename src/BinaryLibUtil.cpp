#include "../include/BinaryLibUtil.hpp"

// from http://www.cplusplus.com/forum/general/1796/
bool BinaryLibUtil::fileExists(const char *filename)
{
	std::ifstream ifile(filename);
	return ifile != nullptr;
}

bool BinaryLibUtil::fileDelete(const char *filename)
{
	if(!remove(filename) && errno != 0)
	{
		std::cerr << "Error deleting " << filename << ": " << strerror(errno) << " (" << errno << ")\n";
		return false;
	}
	return true;
}

bool BinaryLibUtil::moveFile(const std::string& src, const std::string& dst, const bool overwrite)
{
	if(BinaryLibUtil::fileExists(dst.c_str()))
	{
		if(overwrite && !fileDelete(dst.c_str()))
		{
			std::cerr << "Error renaming \"" << src << "\" to \"" << dst << "\" (failed to delete existing file)\n";
			return false;
		}
		else
		{
			std::cerr << "Error renaming \"" << src << "\" to \"" << dst << "\" (file already exists)\n";
			return false;
		}
	}
	rename(src.c_str(), dst.c_str());
	return true;
}

uint32_t BinaryLibUtil::version()
{
	return BINARYLIB_VERSION;
}
