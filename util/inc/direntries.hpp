#include <iostream>
#include <sstream>

#include <string>	//--|
#include <locale>	//  |-- THESE ARE ALL FOR UTF-16 PRINTING
#include <codecvt>	//--|

#include <vector>

#include "cfb_types.hpp"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

// Sector Allocation Table
// Starts at sector 0 (post-header) and should comprise a single sector
// TODO: This isn't true for all CFB files, so check the header MSAT...
int32_t sat[128];

header_t header;

// Build the header
void build_header(std::ifstream &cfb_file);

// Build the sector allocation table
void build_sat(std::ifstream &cfb_file);

void build_dir_entry_chain(std::ifstream &cfb_file);

std::vector<directory_entry_t> dir_entries;

// Read the next directory entry and push it back to the dir_entries vector
// Returns true if it read a directory entry
// Returns false if there wasn't one or it wasn't valid
void read_next_directory_entry(std::ifstream &cfb_file);



