#include "cfb_types.hpp"

directory_entry::directory_entry()
{
	this->dir_entry_bytes.resize(CFB_DIR_ENTRY_SIZE);
}

directory_entry::directory_entry(const directory_entry & to_copy)
{
	this->dir_entry_bytes.resize(CFB_DIR_ENTRY_SIZE);
	this->dir_entry_bytes = to_copy.dir_entry_bytes;
}

directory_entry::directory_entry(uint8_t * dir_entry_bytes, size_t bytes_to_read)
{
	this->dir_entry_bytes.resize(CFB_DIR_ENTRY_SIZE);
	this->assign_bytes(dir_entry_bytes, bytes_to_read);
}

void directory_entry::assign_bytes(uint8_t * dir_entry_bytes, size_t bytes_to_read)
{
	this->dir_entry_bytes = std::valarray<uint8_t>(dir_entry_bytes, bytes_to_read);
}

std::string directory_entry::get_name()
{
	std::string name_string = "";

	// Find "name" entry in map
	std::valarray<uint8_t> name_bytes = get_dir_entry_param_bytes("name");	

	// Get the length of the name string and trim the last char16_t because it's NULL
	uint8_t name_length = get_dir_entry_param_bytes("name_length")[0] - sizeof(char16_t);

	// Slice the name string into an appropriate length
	std::valarray<uint8_t> sliced_name_bytes = name_bytes[std::slice(0, name_length, 1)];

	// If we have at least one char16_t to read
	if(name_bytes.size() > sizeof(char16_t))
	{
		// Convert it to a string because UTF-16 strings are very bad + unprintable
		std::u16string u16name(reinterpret_cast<char16_t *>(&sliced_name_bytes[0]), sliced_name_bytes.size() / sizeof(char16_t));
		name_string = boost::locale::conv::utf_to_utf<char>(&u16name[0], &u16name[0] + u16name.length());
	}
	
	// This string will be empty if there was no name found
	return name_string;
}

std::string directory_entry::get_creation_time()
{
	return "";
}

std::string directory_entry::get_modified_time()
{
	return "";
}

dir_entry_object_type directory_entry::get_object_type()
{
	dir_entry_object_type ot = (dir_entry_object_type)(get_dir_entry_param_bytes("type")[0]);

	return ot;
}

// TODO: Template function that returns the value/type we're actually interested in?
std::valarray<uint8_t> directory_entry::get_dir_entry_param_bytes(std::string param_key)
{
	// Find entry in map
	std::map<std::string, std::pair<uint8_t, uint8_t>>::iterator it = dir_entry_params.find(param_key);

	// If the item actually exists
	if(it != dir_entry_params.end())
	{
		// Get the offset and the size (in bytes)
		uint8_t offset = it->second.first;
		uint8_t size = it->second.second;
		// Grab a slice based on 
		std::valarray<uint8_t> bytes(dir_entry_bytes[std::slice(offset,size, 1)]);
		return bytes;
	}

	// If no param was found, or there was an error, return an empty valarray
	return std::valarray<uint8_t>();
}

// TODO: Not implemented yet
uint32_t directory_entry::get_stream_sector_id()
{
	return FREESECT;
}

uint32_t directory_entry::get_stream_size()
{
	return FREESECT;
}