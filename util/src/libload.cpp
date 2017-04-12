#include <iostream>
#include <vector>

#include "altium_lib.hpp"

int main(int argc, char * argv[])
{
	if(argc < 2)
    {
        std::cout << "Usage: ./libload [path to library file]\n";
        return 1;
    }

    std::string lib_path_string(argv[1]);

    std::cout << "Creating new library...\n";

    altium_lib libfile(lib_path_string);
 
    std::cout << "> Loading library at " << lib_path_string << "...\n";

    if(libfile.load_from_file())
    {
    	std::cout << ">\tsucceeded\n";
    }
    else
    {
    	std::cout << ">\tfailed!\n";
    	return 1;
    }

    std::cout << "Getting symbol/footprint names...\n";

    std::vector<std::string> lib_entities;

    if(libfile.get_library_entries(lib_entities))
    {
    	std::cout << ">\tsucceeded\n";
    	std::cout << "Entities:\n";

    	for(size_t i = 0; i < lib_entities.size(); i++)
    	{
    		std::cout << ">> " << lib_entities.at(i) << '\n';
    	}
    }
    else
    {
    	std::cout << ">\tfailed! No entities found\n";
    	return 1;
    }

    return 0;
}