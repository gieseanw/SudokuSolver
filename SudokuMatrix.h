//SudokuMatrix.h
//  Author: Andy Giese
//  Date: June 2011
//  Purpose: See below

// a quadruply-linked circular list (Dancing Links) that uses Algorithm X to solve exact cover

//#define MATRIX_SIZE 9
//#define MAX_COLS 324
//#define MAX_ROWS 729
//729 columns by 324 rows
//this stemps from 81*4 constraints = 324 columns
//and 9*9*9 possibilities = 729 rows
//the constraints are 1) Every row must have values 1-9, and there are 9 rows = 81 constraints
//	      2) Every column must have values 1-9, and there are 9 columns = 81 constraints
//	      3) Every box must have values 1-9, and there are 9 boxes = 81 constraints
//	      4) Every cell must have a value, and there are 9*9 cells = 81 constraints
//Clearly, satisfying one constraint could satisfy another (if we place 1 at 1,1 then we've satisfied the constraints
//that
//row 1 must have value 1 in it, column 1 must have value 1 in it, box 1 must have value in it, and cell 1,1 must have a
//value in it.
//one entry into the puzzle solution satisfies 4 constraints
//There are 729 rows because there are 729 subsets of size 4 {r,c,b,v} the box value is determined by r and c, so it is
//disregarded.
// row1column1value1, row1column1value2,...,row1column1value9,...,row9column9value9 = 9*9*9 = 729 combinations

/*
   The matrix representation I'm using is as follows:
   RV		ColV	    RCFilled	    BV
   RCV   --	  ----	      --------	      --
   11 12 13 14 ... 99  11 12 13 14 ... 99  11 12 13 14 ... 99	  11 12 13 14 ... 99  
   ---------------------------------------------------------------------------------------------
   111      1 0  0  0      0    1  0  0  0      0   1  0  0  0      0       1  0  0  0      0
   112      0 1  0  0      0    0  1  0  0      0   1  0  0  0      0       0  1  0  0      0
   113
   114
   .
   .
   .
   119
   121
   122
   .
   .
   .
   999


   Where RV is Row-Value pairs, ColV = Column-Value pairs, RCFilled is Cell-Filled pairs (is there a value in the cell?),
   and BV is Box-Value pairs
   the matrix is a boolean matrix of 1s and 0s, 1 if RV and RC intersect and 0 elsewise

   (note, I will be using 0-8 instead of 1-9. A printout of the complete above matrix can be found in exactCover.txt,
   though I have not included row and column headings)
 */

/*A sudoku grid will have its boxes labelled in row-dominant order (left right ordering before up and down).
  Therefore a 9X9 grid has boxes: 0 1 2
                                  3 4 5
                                  6 7 8
 */




#pragma once

#include <iostream>
#include <stack>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "Definitions.h"


//Node structure to be used in DLX
struct Node
{
  Node* top;
  Node* bottom;
  Node* left;
  Node* right;
  Node* colHeader;
  int row;
  int column;
  int value;
  bool header;
  Node()
  {
    top=bottom=left=right=colHeader=NULL;
    row=column=value=-1;
    header=false;
  }
  Node(int r,int c,int v)
  {
    top=bottom=left=right=colHeader=NULL;
    row=r;
    column=c;
    value=v;
    header=false;
  }
};

class SudokuMatrix
{
  public:
    SudokuMatrix(); //default constructor
    ~SudokuMatrix();  //destructor
    bool initialize();
    //pre: Root is not null
    //post: creates the DLX structure for the blank sudoku puzzle
    //	This function should be called after the data structure is initialized
    //	if it returns false, then intializing the matrix failed
    //	if it returns true, then initializing the matrix succeeded

    bool AddColumn(Node* newNode);
    //pre: newNode is a column header
    //post: newNode is added to the end of the column headers list in our matrix

    void print();
    //pre: none
    //post: prints debugging information to stdout
    //	commented out code will print the matrix in column-dominant order

    std::stack<Node>* solve(const char* filename);
    //pre: the matrix has been initialized
    //post: will solve the puzzle in the given file and return the solution in the stack structure
    //	A stack structure was chosen so that one could see the order in which the puzzle was solved
    //	with the last entries to the solution found on top
    /* A sudoku board should be represented in an input file where all empty cells are defined as 0s
    *  For example (from the example8by8.txt input file): 

       0 0 0 7 0 0 2 1 0
       0 0 0 0 5 9 0 4 3
       0 0 0 0 0 8 9 0 0
       8 0 2 0 0 0 0 0 0
       6 5 0 0 1 0 0 2 4
       0 0 0 0 0 0 5 0 7
       0 0 7 2 0 0 0 0 0
       9 1 0 5 8 0 0 0 0
       0 8 4 0 0 6 0 0 0

       Note: The above problem is expert level, so it may take you some time to solve it by yourself. The program,
       however, solves it very quickly :)
     */

  private:
    Node* Root;//points to first column header of matrix
    std::stack<Node>* workingSolution; //the partial or full solution to the current puzzle
    bool Solved; //if solution found, true, else false
    int totalCompetition;

    bool AddColumnHelp(Node* newNode, Node* r);
    //helper function for AddColumn

    bool isEmpty(); 
    //returns whether Root is only node in the matrix

    void deleteMatrix();
    //helper function for destructor

    void cover(Node* r); 
    //hides all nodes in r's row, as well as the columns r's row covers, 
    //and all the rows contained in those columns

    void uncover(Node* r); 
    //unhides r from rest of matrix. r is assumed to be a column header

    Node* find(Node* find);
    // searches for find, if found, returns first found node, else returns NULL

    bool solve(); //recursively called function that performs Algorithm X
    /* Algorithm X:
       if the matrix is empty, terminate successfully
       else choose a column c with the least 1s
       if (least number of 1s in a column is 0, terminate unsuccessfully)
       for each row r in c
       add r to partial solution
       cover r
       if (recurse on reduced matrix == unsuccessfull)
       uncover r, remove from partial solution
       else
       return successful, solution

     */

    Node* chooseNextColumn(int& count);
    //returns a pointer to a column in the matrix
    //that has the fewest nodes in its row
    //this is used as the heuristic for choosing the next constraint to satisfy
    //in dancing links

};
