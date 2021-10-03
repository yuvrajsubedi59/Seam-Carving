/**
 * Yuvraj Subedi
 * Seam Carving
 * seamCarve.hpp
 */

#ifndef INCLUDE_SEAMCARVE_HPP
#define INCLUDE_SEAMCARVE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// get the absolute value of an integer
int abs(int n);

// get the minimum of two integers
int min(int n, int m);

// stores the 2-dimensional matrix of integers
// for PGM array representation and energy matrix
struct Matrix{

    // stores the integers
    std::vector< std::vector<int> > items; 

    // constructor
    Matrix(int x, int y);

    // rotate the matrix to make rows as easy as columns
    void rotate_right();
    void rotate_left();

    //carve the minimum energy column seam
    void carve_column();

    //carve the minimum energy row seam
    void carve_row();

    // find a minimum energy column in a matrix
    std::vector<int> min_column_seam();

    // generate the energy matrix of a Matrix of values
    Matrix energy_matrix(const Matrix &g);
};

// read the pgm file
Matrix read_pgm(std::string filename);

// write the pgm file after seam carving
void write_pgm(const Matrix &greyScale, std::string filename);

#endif