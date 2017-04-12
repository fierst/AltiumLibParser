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

    // Print the name of each directory entry
    // See: https://stackoverflow.com/a/5616234/2614831
    // UTF16 is bad, so hopefully we don't need to print these often
    for(size_t i = 0; i < dir_entries.size(); i++)
    {
        print_directory_entry(i);
    }

    ifs.close();

}

void build_header(std::ifstream &cfb_file)
{
    cfb_file.read(reinterpret_cast<char *>(&header), sizeof(header_t));
}

void build_sat(std::ifstream &cfb_file)
{
    uint32_t offset;

    uint32_t sector_size = (1 << header.sec_size);

    int32_t sector_id;

    for(uint32_t i = 0; i < header.sec_count; i++)
    {
        offset = (1 + header.master_sec_ids[i]) * (sector_size);
        cfb_file.seekg(offset, std::ios::beg);
        for(uint32_t j = 0; j < sector_size / sizeof(uint32_t); j++)
        {
            cfb_file.read(reinterpret_cast<char *>(&sector_id), sizeof(int32_t));
            sector_alloc_table.push_back(sector_id);
        }
    }
}    

void build_dir_entry_chain(std::ifstream &cfb_file)
{
    uint32_t offset = 0;

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
        current_sector = sector_alloc_table.at(current_sector);
    }
}

void read_next_directory_entry(std::ifstream &cfb_file)
{
    uint8_t dir_entry_bytes[CFB_DIR_ENTRY_SIZE];

    cfb_file.read(reinterpret_cast<char *>(&dir_entry_bytes), CFB_DIR_ENTRY_SIZE);

    directory_entry new_dir_entry(dir_entry_bytes, sizeof(dir_entry_bytes));

    dir_entries.push_back(new_dir_entry);
}

void print_directory_entry(size_t at_index)
{
    std::cout << dir_entries[at_index].get_name() << "\n";    
}
