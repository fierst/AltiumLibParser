#include "print_header_info.hpp"

namespace fs = boost::filesystem;

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage: ./print_header_info [path to library file]\n";
		return 1;
	}

	// Create the file stream object
	fs::ifstream ifs;

	// Path to the library, specified by user at the command line
	fs::path libfile(argv[1]);
	
	// Check to make sure it's actually a valid file
	if(exists(libfile))
	{
		if(is_directory(libfile))
		{
			std::cout << "Error: " << libfile << " is a directory\n";
			return 1;
		}
	}
	else
	{
		std::cout << "Error: " << libfile << " does not exist\n";
		return 1;
	}

	ifs.open(libfile, std::ios::binary | std::ios::ate);

	// Create a new header_t
	header_t lib_header;

	ifs.seekg(0, std::ios::beg);

	// Since the header_t is packed, we can just raw_copy the bytes from the filesystem
	// There's probably a much better/safer way to do this
	// But this is mostly just for testing...
	ifs.read(reinterpret_cast<char *>(&lib_header), 512);

	ifs.close();

	print_info(&lib_header);

	return 0;
}

void print_info(header_t * header)
{
	std::cout << "HEADER PARAMETER\t\tVALUE\n";
	std::cout << "magic number:\t\t\t" << std::hex << __builtin_bswap64((*header).magic) << "\n";
	std::cout << "uid:\t\t\t\t" << std::hex << *(int*)(*header).uid << "\n";
	std::cout << "version:\t\t\t" << get_version_info(header->version_number) << "\n";
	std::cout << "endianness:\t\t\t" << std::hex << (*header).byte_order << "\n";
	std::cout << "sector size:\t\t\t" << (*header).sec_size << "\n";
	std::cout << "short sector size:\t\t" << (*header).short_sec_size << "\n";
	std::cout << "sector count:\t\t\t" << (*header).sec_count << "\n";
	std::cout << "first sector id:\t\t" << (*header).first_sec_id << "\n";
	std::cout << "min std stream size:\t\t" << std::hex << (*header).min_std_stream_size << "\n";
	std::cout << "first short sector id:\t\t" << (*header).first_short_sec_id << "\n";
	std::cout << "short sector count:\t\t" << (*header).short_sec_count << "\n";
	std::cout << "first master sector id:\t\t" << (*header).first_master_sec_id << "\n";
	std::cout << "master sector count:\t\t" << (*header).master_sec_count << "\n";
}

std::string get_version_info(uint16_t * version)
{
	std::stringstream version_string;

	version_string << version[1] << "." << version[0];

	return version_string.str();
}
