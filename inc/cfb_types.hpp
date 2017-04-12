// Types inherent to the CFB file format
// All of these should be outlined in doc/MSDN_CFB_Format_Spec.pdf
// Or at https://msdn.microsoft.com/en-us/library/dd942138.aspx
#ifndef CFB_TYPES_H
#define CFB_TYPES_H

#include <map>
#include <valarray>

#include "boost/locale.hpp"

// Sector ID Constants
const int32_t MAXREGSECT   = 0xFFFFFFFA;
const int32_t DIFSECT      = 0xFFFFFFFC;
const int32_t FATSECT      = 0xFFFFFFFD;
const int32_t ENDOFCHAIN   = 0xFFFFFFFE;
const int32_t FREESECT     = 0xFFFFFFFF;

// Directory Entry Color Flags
const uint8_t CF_RED        = 0x00;
const uint8_t CF_BLACK      = 0x01;

// Directory Entry IDs
const int32_t MAXREGSID    = 0xFFFFFFFA;
const int32_t NOSTREAM     = 0xFFFFFFFF;

// Directory Entry Object Types
const uint8_t OT_UNKNOWN    = 0x00;
const uint8_t OT_STORAGE    = 0x01;
const uint8_t OT_STREAM     = 0x02;
const uint8_t OT_ROOT       = 0x05;

// Contains map of structure for the rest of the file
// Since the Altium libraries are generally small, there's not a lot to worry about
#pragma pack(push, 1)
struct header_t
{
    uint64_t magic;
    uint8_t  uid[16];
    uint16_t version_number[2];
    uint16_t byte_order;
    uint16_t sec_size;
    uint16_t short_sec_size;
    uint8_t  spare1[10];
    uint32_t sec_count;
    uint32_t first_sec_id;
    uint32_t spare2;
    uint32_t min_std_stream_size;
    uint32_t first_short_sec_id;
    uint32_t short_sec_count;
    uint32_t first_master_sec_id;
    uint32_t master_sec_count;
    uint32_t master_sec_ids[109];
};
#pragma pack(pop)

// Each Directory Entry is 128 bytes in length
const size_t CFB_DIR_ENTRY_SIZE = 128;

enum dir_entry_object_type
{
    UNKNOWN = 0,
    STORAGE,    
    STREAM,
    ROOT = 5
};

// Directory Entry class
// Each directory entry is part of a red-black tree
// Currently only implemented methods required for the application
class directory_entry
{
public:
    directory_entry();
    directory_entry(const directory_entry &);
    directory_entry(uint8_t * dir_entry_bytes, size_t bytes_to_read);

    // Assign the directory entry bytes post-instantiation
    void assign_bytes(uint8_t * dir_entry_bytes, size_t bytes_to_read);

    // Return name string
    std::string get_name();

    // Return creation time in a human-readable string (YYYY-MM-DD hh:mm:ss)
    // TODO: Allow format specifiers?
    std::string get_creation_time();

    // Return modification time in a human-readable string (YYYY-MM-DD hh:mm:ss)
    // TODO: Allow format specifiers?
    std::string get_modified_time();

    // Get the object type of the directory entry 
    dir_entry_object_type get_object_type();

    // Return the sector id of the stream
    uint32_t get_stream_sector_id();

    // Return the size of the stream
    uint32_t get_stream_size();

private:

    std::valarray<uint8_t> get_dir_entry_param_bytes(std::string param_key);

    // Container for the raw bytes of the directory entry 
    std::valarray<uint8_t> dir_entry_bytes;

    // Map of parameters for the directory entry
    // Key - Name of the requested parameter
    // Value - std::pair<uint8_t, uint8_t> with the offset and number of bytes for the requested parameter 
    std::map<std::string, std::pair<uint8_t, uint8_t>> dir_entry_params
    {
        std::make_pair("name",          std::make_pair(0,  64)),
        std::make_pair("name_length",   std::make_pair(64,  2)),
        std::make_pair("type",          std::make_pair(66,  1)),
        std::make_pair("color",         std::make_pair(67,  1)),
        std::make_pair("l_sibling_id",  std::make_pair(68,  4)),
        std::make_pair("r_sibling_id",  std::make_pair(72,  4)),
        std::make_pair("child_id",      std::make_pair(76,  4)),
        std::make_pair("clsid",         std::make_pair(80, 16)),
        std::make_pair("user_flags",    std::make_pair(96,  4)),
        std::make_pair("time_created",  std::make_pair(100, 8)),
        std::make_pair("time_modified", std::make_pair(108, 8)),
        std::make_pair("stream_sec_id", std::make_pair(116, 4)),
        std::make_pair("stream_size",   std::make_pair(120, 4)),
        std::make_pair("spare",         std::make_pair(124, 4))
    };
};

#endif // CFB_TYPES_H