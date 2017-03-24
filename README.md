# Altium Library Parsing  
  
Altium Designer stores schematic symbol and footprint libraries in binary file formats (.SchLib and .PcbLib, respectively.) Parsing these files should allow for an external program to determine the contents of the files (i.e., names of symbols and footprints) and render them graphically.  
  
The current implementation is in C++ with the end goal of being part of a larger Qt-based front-end for a parts database.  

