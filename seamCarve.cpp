/**
 * Yuvraj Subedi
 * Seam Carving
 * seamCarve.cpp
 */

#include "seamCarve.hpp"

/*
    get the absolute value of an integer

    @param n integer subjected to find the absolute value
    @return absolute value of n
*/
int abs(int n){
    return (n < 0 ? -n : n);
}

/*
    get the minimum of two integers

    @param n first integer to be compared
    @param m second integer to be compared
    @return minimum value of n and m
*/
int min(int n, int m){
    return (n < m ? n : m);
}

/*
    read the pgm file and fill matrix with greyscale values

    @param filename Image file name to be carved
    @return matrix of greyscale values
*/
Matrix read_pgm(std::string filename){

    std::string line;
    int col, row, maximum;

    // open pgm input image
    std::ifstream inputImage(filename.c_str());

    // discard the version
    getline(inputImage, line);

    // discard the comments
    while(inputImage.peek() == '#') 
    	getline(inputImage, line);
    
    // read columns and rows
    inputImage >> col; 
    inputImage >> row; 

    // discard the maximum value
    inputImage >> maximum; 

    // fill Matrix with grayscale values
    Matrix greyScale(row, col); 
    for(int i = 0; i < row; ++i)
        for(int j = 0; j < col; ++j)
            inputImage >> greyScale.items[i][j];
    return greyScale;
}

/*
    write the pgm file after seam carving

    @param filename Image file name which is carved
    @param greyScale greyscale matrix 
*/
void write_pgm(const Matrix &greyScale, std::string filename){

    // find the . in filename
    std::size_t i = filename.find_last_of(".");

    // store the name of the file without extension
    std::string name = filename.substr(0, i);

    // output filename created as <filename>_processed.pgm
    std::ofstream outputImage((name + "_processed.pgm").c_str());

    // generate the output file
    outputImage << "P2\n";
    outputImage << "# created by Yuvraj Subedi\n";
    outputImage << greyScale.items[0].size() << ' ' << greyScale.items.size() << '\n';
    outputImage << "255\n";
    for(int i = 0; i < greyScale.items.size(); ++i){
        for(int j = 0; j < greyScale.items[i].size(); ++j){
            outputImage << greyScale.items[i][j] << ' ';
        }
        outputImage << '\n';
    }
}

/*
    constructor
*/
Matrix::Matrix(int x, int y){

    items.resize(x);
    for(int i = 0; i < items.size(); ++i)
        items[i].resize(y);
}

/*
    rotate the matrix right to make rows as easy as columns
*/
void Matrix::rotate_right(){

    //create temporary matrix
    std::vector< std::vector<int> > temp(items[0].size());
    for(int i = 0; i < temp.size(); ++i)
        temp[i].resize(items.size());

    //transpose the matrix
    for(int i = 0; i < items[0].size(); ++i)
        for(int j = 0; j < items.size(); ++j)
            temp[i][j] = items[j][i];

    //reverse the columns
    for(int i = 0; i < temp[0].size(); ++i)
        for(int j = 0, k = temp.size()-1; j < k; j++, k--)
            std::swap(temp[j][i], temp[j][i]);

    items = temp;
}

/*
    rotate the matrix left to make rows as easy as columns
*/
void Matrix::rotate_left(){

    // create temporary grid
    std::vector< std::vector<int> > temp(items[0].size());
    for(int i = 0; i < temp.size(); ++i)
        temp[i].resize(items.size());

    // transpose the matrix
    for(int i = 0; i < items[0].size(); ++i)
        for(int j = 0; j < items.size(); ++j)
            temp[i][j] = items[j][i];

    // reverse the rows
    for(int i = 0; i < temp.size(); ++i)
        for(int j = 0, k = temp[0].size()-1; j < k; j++, k--)
            std::swap(temp[i][j], temp[i][k]);

    items = temp;
}

/*
    carve the minimum energy column seam
*/
void Matrix::carve_column(){

    // find minimum energy seam
    Matrix e = energy_matrix(*this);
    std::vector<int> minimums = e.min_column_seam();

    // remove the seam
    for(int i = 0; i < items.size(); ++i){
        items[i].erase(items[i].begin() + minimums[i]);
    }
}

/*
    carve the minimum energy row seam
*/
void Matrix::carve_row(){

    // transpose grid
    rotate_right();

    // carve the columns
    carve_column();

    // transpose the grid back
    rotate_left();
}

/*
    find the minimum energy column in a matrix using 
    the dynammic programming

    @return a vector of row indices of contiguous column seam
*/
std::vector<int> Matrix::min_column_seam(){

    int rows = items.size(), columns = items[0].size();

    // go upward calculating minimum energy path
    for(int i = rows-2; i >= 0; --i){

        //left corner
        items[i][0] = items[i][0] + min(items[i+1][0], items[i+1][1]);
        
        //right corner
        items[i][columns-1] = items[i][columns-1]
            + min(items[i+1][columns-1], items[i+1][columns-2]);
        
        //inside
        for(int j = 1; j < columns-1; ++j){
            items[i][j] = items[i][j]
                + min(items[i+1][j-1], min(items[i+1][j], items[i+1][j+1]));
        }
    }

    // hold the indices of paths
    std::vector<int> minimums(rows);
    int index, left, right, middle;

    // get initial index of minimum energy path
    std::vector<int>::iterator min;
    min = std::min_element(items[0].begin(), items[0].end());
    minimums[0] = std::distance(items[0].begin(), min);

    // go downward to get indices of minimum energy path
    for(int i = 1; i < rows; ++i){

        // previous index
        index = minimums[i-1];
        middle = items[i][index];

        // left side case
        if(index == 0){
            right = items[i][index+1];

            if(middle <= right)
                minimums[i] = 0;
            else minimums[i] = 1;
        }

        // right side case
        else if(index == columns-1){
            left = items[i][index-1];
            if(left <= middle)
                minimums[i] = columns-2;
            else minimums[i] = columns-1;
        }

        // inside case
        else{
            left = items[i][index-1];
            right = items[i][index+1];

            // left is the smallest
            if(left <= middle && left <= right)
                minimums[i] = index-1;

            // middle is the smallest
            else if(middle <= left && middle <= right)
                minimums[i] = index;
            
            // right is the smallest
            else if(right <= left && right <= middle)
                minimums[i] = index+1; //right is smallest
        }
    }

    // return the minimum energy path
    return minimums;
}

/*
    generate the energy matrix of a grid of values

    @param g Matrix of greyscale values
    @return energy matrix
*/
Matrix Matrix::energy_matrix(const Matrix &g){

    int value, rows = g.items.size(), columns = g.items[0].size();

    // make matrix of equal size
    Matrix e(rows, columns);

    //visit all items and calculate energy
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            value = g.items[i][j];
            // top row
            if(i == 0){ 
                // top left corner
                if(j == 0){ 
                    e.items[i][j] = abs(value - g.items[1][0])
                                    + abs(value - g.items[0][1]);
                }

                // top right corner
                else if(j == columns-1){
                    e.items[i][j] = abs(value - g.items[1][columns-1])
                                    + abs(value - g.items[0][columns-2]);
                }

                // inner top row
                else{
                    e.items[i][j] = abs(value - g.items[1][j])
                                    + abs(value - g.items[0][j-1])
                                    + abs(value - g.items[0][j+1]);
                }
            }

            // the leftmost column
            else if(j == 0){

                // bottom left corner
                if(i == rows-1){
                    e.items[i][j] = abs(value - g.items[rows-2][0])
                                    + abs(value - g.items[rows-1][1]);
                }

                // inner leftmost side
                else{
                    e.items[i][j] = abs(value - g.items[i][1])
                                    + abs(value - g.items[i-1][0])
                                    + abs(value - g.items[i+1][0]);
                }
            }

            // bottom row
            else if(i == rows-1){
                
                // bottom right corner 
                if(j == columns-1){
                    e.items[i][j] = abs(value - g.items[rows-1][columns-2])
                                    + abs(value - g.items[rows-2][columns-1]);
                }

                // inner bottom row
                else{
                    e.items[i][j] = abs(value - g.items[rows-2][j])
                                    + abs(value - g.items[rows-1][j-1])
                                    + abs(value - g.items[rows-1][j+1]);
                }
            }

            // inner rightmost column
            else if(j == columns-1){
                e.items[i][j] = abs(value - g.items[i][columns-2])
                                + abs(value - g.items[i-1][columns-1])
                                + abs(value - g.items[i+1][columns-1]);
            }

            // inside
            else{
                e.items[i][j] = abs(value - g.items[i-1][j])
                                + abs(value - g.items[i+1][j])
                                + abs(value - g.items[i][j-1])
                                + abs(value - g.items[i][j+1]);

            }
        }
    }

    // return energy matrix
    return e;
}