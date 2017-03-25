#include <iostream>
#include <sstream>

#include "cfb_types.hpp"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

// Master function to print the information
void print_info(header_t * header);

// Get the version information from the revision_number and version_number
std::string get_version_info(uint16_t * version);

// Verify the magic number of the file 
bool check_magic_number(uint64_t * magic_number);

