#include "altium_lib.hpp"

altium_lib::altium_lib(void)
{

}

altium_lib::altium_lib(const std::string & path_to_file)
{
    this->filepath_ = path_to_file;
}

bool altium_lib::load_from_file(const std::string & path_to_file)
{
    std::string file_to_load;

    if(path_to_file.empty())
    {
        if(filepath_.empty())
        {
            return false;
        }
        else
        {
            file_to_load = filepath_;
        }
    }
    else
    {
        file_to_load = path_to_file;
    }

    // Create the file stream object
    fs::ifstream ifs;

    // Path to the library, specified by user at the command line
    fs::path libfile(file_to_load);

    // Check to make sure it's actually a valid file
    if(exists(libfile))
    {
        if(is_directory(libfile))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    ifs.open(libfile, std::ios::binary | std::ios::ate);

    // Seek to the beginning of the file
    ifs.seekg(0, std::ios::beg);

    // Build the header
    build_header(ifs);

    // build the sector allocation table
    build_sat(ifs);

    build_dir_entry_chain(ifs);

    ifs.close();

    return true;
}

bool altium_lib::get_library_entries(std::vector<std::string> & list_of_entries)
{
    // Clear the entries vector and set its size to zero
    list_of_entries.erase(list_of_entries.begin(), list_of_entries.end());

    std::string current_name;
    bool is_keyword;

    for(size_t i = 0; i < dir_entry_tree.size(); i++)
    {
        current_name = dir_entry_tree.at(i).get_name();
        is_keyword = (std::find(altium_lib_keywords.begin(), altium_lib_keywords.end(), current_name) != altium_lib_keywords.end());

        if( dir_entry_tree.at(i).get_object_type() == dir_entry_object_type::STORAGE && !is_keyword)
        {
            list_of_entries.push_back(current_name);
        }
    }

    // Return true if there are any entries to read
    return (!list_of_entries.empty());
}

void altium_lib::build_header(std::ifstream &cfb_file)
{
    cfb_file.read(reinterpret_cast<char *>(&header_), sizeof(header_t));
}

void altium_lib::build_sat(std::ifstream &cfb_file)
{
    uint32_t offset;

    uint32_t sector_size = (1 << header_.sec_size);

    int32_t sector_id;

    for(uint32_t i = 0; i < header_.sec_count; i++)
    {
        offset = (1 + header_.master_sec_ids[i]) * (sector_size);
        cfb_file.seekg(offset, std::ios::beg);
        for(uint32_t j = 0; j < sector_size / sizeof(uint32_t); j++)
        {
            cfb_file.read(reinterpret_cast<char *>(&sector_id), sizeof(int32_t));
            sector_alloc_table.push_back(sector_id);
        }
    }
}

void altium_lib::build_dir_entry_chain(std::ifstream &cfb_file)
{
    uint32_t offset = 0;

    int32_t current_sector = header_.first_sec_id;

    uint32_t sector_size = (1 << header_.sec_size);

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

void altium_lib::read_next_directory_entry(std::ifstream &cfb_file)
{
    uint8_t dir_entry_bytes[CFB_DIR_ENTRY_SIZE];

    cfb_file.read(reinterpret_cast<char *>(&dir_entry_bytes), CFB_DIR_ENTRY_SIZE);

    directory_entry new_dir_entry(dir_entry_bytes, sizeof(dir_entry_bytes));

    dir_entry_tree.push_back(new_dir_entry);
}