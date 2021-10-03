/**
 * Yuvraj Subedi
 * Seam Carving
 * main.cpp
 */

#include "seamCarve.hpp"

int main(int argc, char* argv[]){

    std::string file(argv[1]);

    // vertical and horizontal seams
    int vert = atoi(argv[2]);
    int horiz = atoi(argv[3]);

    // read pgm file
    Matrix pixels = read_pgm(file); 

    // remove the minimum energy column seam
    for(int i = 0; i < abs(vert); ++i){ 
        pixels.carve_column();
    }

    // remove the minimum energy row seam
    for(int j = 0; j < abs(horiz); ++j){ 
        pixels.carve_row();
    }

    // write new pgm file
    write_pgm(pixels, file); 

    // display new filename
    std::size_t i = file.find_last_of(".");
    std::string name = file.substr(0, i);
    std::cout << name << ".pgm has been carved\n" 
                << name << "_processed.pgm has been generated.\n";

    return 0;
}