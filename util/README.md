# Utility Funtions

A small group of utility functions to glean information from the library files or test the types without modifying the main source.

While I'm trying to keep this as portable as possible, there may be some platform-specific additions depending on which computer (home or work) I'm working on.

### Programs
* **print_header_info**: print relevant information from the header 

* **direntries**: populate the directory entries table and print each one's information

* **libload**: read an altium library file and output some debug information, including the names of symbols/footprints in the library