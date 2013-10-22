//  definitions.h
//  Author: Andy Giese
//  Date: June 2011
//  Purpose: includes all the #defines needed for this project

//assume 9x9 sudoku for building initial matrix
//if we wish to expand, then only these defines should need to be changed appropriately

#pragma once

//these are offsets into the matrix that specify the sections of columns pertaining to 
//row,column,cell,and box constraints respectively

#define ROW_OFFSET 0 
#define COL_OFFSET 81
#define CELL_OFFSET 162
#define BOX_OFFSET 243

//these ultimately define the proportions of our matrix
#define MATRIX_SIZE 9
#define MAX_COLS 324
#define MAX_ROWS 729

#define COL_BOX_DIVISOR 3 //number of boxes in a column
#define ROW_BOX_DIVISOR 3 //number of boxes in a row



// If we want to expand from 9x9 Sudoku to 10 x 10 then theoretically :)
// the following defines can replace the above ones though you may need to increase your stack size

/*#define ROW_OFFSET 0 
#define COL_OFFSET 100
#define CELL_OFFSET 200
#define BOX_OFFSET 300

#define MATRIX_SIZE 10
#define MAX_COLS 400
#define MAX_ROWS 1000
#define ROW_BOX_DIVISOR 2
#define COL_BOX_DIVISOR 5*/

//likewise for a 12x12 matrix

/*
#define ROW_OFFSET 0 
#define COL_OFFSET 144
#define CELL_OFFSET 288
#define BOX_OFFSET 432

#define MATRIX_SIZE 12
#define MAX_COLS 576
#define MAX_ROWS 1728

#define ROW_BOX_DIVISOR 4
#define COL_BOX_DIVISOR 3
*/

//...and a 16x16 matrix

/*
#define ROW_OFFSET 0 
#define COL_OFFSET 256
#define CELL_OFFSET 512
#define BOX_OFFSET 768

#define MATRIX_SIZE 16
#define MAX_COLS 1024
#define MAX_ROWS 4096

#define ROW_BOX_DIVISOR 4
#define COL_BOX_DIVISOR 4
*/
