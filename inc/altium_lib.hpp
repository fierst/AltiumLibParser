#include "cfb_types.hpp"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

namespace fs = boost::filesystem;

class altium_lib
{

public:
    
    altium_lib(void);
    altium_lib(const std::string & path_to_file);

    // load the lib file from the specified path (or the default path)
    // returns whether or not it was successful.
    bool load_from_file(const std::string & path_to_file = std::string());

    // get all of the library entries (symbol names or footprint names) from the library file
    // returns whether or not there were any.
    bool get_library_entries(std::vector<std::string> & list_of_entries);

private:
    
    void build_header(std::ifstream &cfb_file);
    void build_sat(std::ifstream &cfb_file);
    void build_dir_entry_chain(std::ifstream &cfb_file);
    void read_next_directory_entry(std::ifstream &cfb_file);
    
    std::string filepath_;

    // The CFB header. Shouldn't be directly accessible from outside this class
    header_t header_;
    
    // Sector allocation table/chain, for building the rest of the file.
    std::vector<int32_t> sector_alloc_table;

    // Keywords used by Altium for the library, that are *not* symbol/footprint names
    // TODO: There must be a better way to figure out which are keywords and which are symbols/footprints...
    std::vector<std::string> altium_lib_keywords {"Library", "Models", "ModelsNoEmbed", 
                                                  "Textures", "ComponentParamsTOC", "UniqueIDPrimitiveInformation",
                                                  "FileVersionInfo", "PadViaLibrary"};

    // TODO: Convert to a red-black tree? That's how the CFB spec handles it...
    std::vector<directory_entry> dir_entry_tree;

};