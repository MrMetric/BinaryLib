#include "../include/Util.hpp"

// from http://www.cplusplus.com/forum/general/1796/
bool Util::fileExists(const char *filename)
{
	std::ifstream ifile(filename);
	bool exists = (ifile != NULL);
	//cout << filename << " exists: " << (exists?"yes":"no") << endl;
	return exists;
}

bool Util::fileDelete(const char *filename)
{
	if(!remove(filename) && errno != 0)
	{
		std::cerr << "Error deleting " << filename << ": " << strerror(errno) << " (" << errno << ")\n";
		return false;
	}
	//cout << "Deleted " << filename << endl;
	return true;
}

bool Util::moveFile(std::string src, std::string dst, bool overwrite)
{
	//cout << "Moving " << src << " to " << dst << endl;
	if(overwrite && fileExists(dst.c_str()) && !fileDelete(dst.c_str()))
	{
		std::cerr << "Error renaming \"" << src << "\" to \"" << dst << "\" (failed to delete existing file)\n";
		return false;
	}
	else if(Util::fileExists(dst.c_str()))
	{
		std::cerr << "Error renaming \"" << src << "\" to \"" << dst << "\" (file already exists)\n";
		return false;
	}
	rename(src.c_str(), dst.c_str());
	return true;
}
