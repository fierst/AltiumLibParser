#include "direntries.hpp"

namespace fs = boost::filesystem;

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: ./direntries [path to library file]\n";
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

    // Seek to the beginning of the file
    ifs.seekg(0, std::ios::beg);

    // Build the header
    build_header(ifs);

    // build the sector allocation table
    build_sat(ifs);

    build_dir_entry_chain(ifs);

    std::cout << "number of dir entries: " << dir_entries.size() << "\n\n";

    // Print the name of each directory entry
    // See: https://stackoverflow.com/a/5616234/2614831
    // UTF16 is bad, so hopefully we don't need to print these often
    for(size_t i = 0; i < dir_entries.size(); i++)
    {
        std::u16string utf16_name(dir_entries[i].entry_name);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> uconv;
        std::string dir_entry_name = uconv.to_bytes(utf16_name);
        std::cout << dir_entry_name << "\n";
    }

    ifs.close();

}

void build_header(std::ifstream &cfb_file)
{
    cfb_file.read(reinterpret_cast<char *>(&header), sizeof(header_t));
}

void build_sat(std::ifstream &cfb_file)
{
    cfb_file.read(reinterpret_cast<char *>(&sat), sizeof(sat));
}

void build_dir_entry_chain(std::ifstream &cfb_file)
{
    uint16_t offset = 0;

    int32_t current_sector = header.first_sec_id;

    uint32_t sector_size = (1 << header.sec_size);

    uint32_t dir_entries_per_sector = sector_size / (CFB_DIR_ENTRY_SIZE);
    
    while(current_sector != ENDOFCHAIN)
    {
        offset = (1 + current_sector) * (sector_size);

        cfb_file.seekg(offset, std::ios::beg);

        for(uint8_t i = 0; i < dir_entries_per_sector; i++)
        {
            read_next_directory_entry(cfb_file);
        }

        // Move to the next sector containing directory entries
        current_sector = sat[current_sector];
    }
}

void read_next_directory_entry(std::ifstream &cfb_file)
{
    directory_entry_t direntry;

    cfb_file.read(reinterpret_cast<char *>(&direntry), sizeof(directory_entry_t));

    dir_entries.push_back(direntry);
}