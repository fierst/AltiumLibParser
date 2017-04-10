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

        std::cout << ">>\tCurrent Sector: " << current_sector << '\n';

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
    directory_entry_t direntry;

    std::cout << ">>\t\tCurrent Offset: " << cfb_file.tellg() << '\n';

    cfb_file.read(reinterpret_cast<char *>(&direntry), sizeof(directory_entry_t));

    dir_entries.push_back(direntry);
}

void print_directory_entry(size_t at_index)
{
    // -- NAME --
    std::u16string utf16_name(dir_entries[at_index].entry_name);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> uconv;
    std::string dir_entry_name = uconv.to_bytes(utf16_name);
    std::cout << "Name: " << dir_entry_name << "\n";
    
    // -- TYPE --
    std::cout << "Entry Type: ";

    switch(dir_entries[at_index].entry_type)
    {
        case 0x00:
            std::cout << "Unknown or Unallocated\n";
            break;
        case 0x01:
            std::cout << "Storage Object\n";
            break;
        case 0x02:
            std::cout << "Stream Object\n";
            break;
        case 0x05:
            std::cout << "Root Storage Object\n";
            break;
    }

    // -- NODE COLOR -- 
    std::cout << "Node Color: " << (dir_entries[at_index].node_color == CF_RED ? "Red\n" : "Black\n");

    // -- LEFT ENTRY --
    std::cout << "Left Node: " << dir_entries[at_index].dir_id_left << "\n";

    // -- RIGHT ENTRY --
    std::cout << "Right Node: " << dir_entries[at_index].dir_id_right << "\n";

    // -- CHILD ENTRY --
    std::cout << "Child Entry: " << dir_entries[at_index].dir_id_root << "\n";

    // -- CREATED TIME --
    std::cout << "Created Time: " << dir_entries[at_index].created_time << "\n";

    // -- MODIFIED TIME --
    std::cout << "Modified Time: " << dir_entries[at_index].modified_time << "\n";

    // -- FIRST SECTOR ID --
    std::cout << "First Sector ID: " << dir_entries[at_index].first_sec_id << "\n";

    // -- TOTAL STREAM SIZE --
    std::cout << "Total Stream Size: " << dir_entries[at_index].total_stream_size << "\n";

    std::cout << "\n\n";
    
}