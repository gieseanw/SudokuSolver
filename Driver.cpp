/*
 * driver.cpp
 * Author: Andy Giese
 * Date: June 2011
 * Purpose: creates a SudokuMatrix object and gives it puzzles to solve
 */
#include "SudokuMatrix.h"
#include <iostream>
#include <stack>
#include <string>
using namespace std;
#include "Definitions.h"



bool initialize(SudokuMatrix* &m);
int main()
{
  SudokuMatrix* m=new SudokuMatrix();
  string userInput;
  bool userContinues = true;
  std::stack<Node>* solution;
  Node next;

  if (!(m->initialize()))
  {
    cout << "Could not initialize matrix" << endl;
    delete m;
    return 1;
  }
  cout << "Initialized matrix" << endl << endl;

  //send m a sudoku puzzle to solve
  //m will return solution, or partial solution if failed.

  int puzzle[MATRIX_SIZE][MATRIX_SIZE]; //the sudoku puzzle that will be filled with solution

  while (userContinues)
  {
    for (int i=0;i<MATRIX_SIZE;i++)
    {
      for (int j=0;j<MATRIX_SIZE;j++)
      {
	puzzle[i][j] = 0;
      }
    }

    //print solution to screen

    string puzzleToSolve;
    cout << "\nFilename of puzzle to solve: ";
    cin >> puzzleToSolve;
    solution = m->solve(puzzleToSolve.c_str());
    if (solution != NULL)
    {
      while(!solution->empty())
      {
	next = solution->top();
	puzzle[next.row][next.column] = next.value+1;
	solution->pop();
      }
    }
    else
      cout << "Solution could not be found" << endl;

    cout << "Solution: " << endl;
    for (int i=0;i<MATRIX_SIZE;i++)
    {
      for (int j=0;j<MATRIX_SIZE;j++)
      {
	cout << puzzle[i][j] << " ";
      }

      cout << endl;
    }
    delete solution;
    solution = NULL;

    cout << "\nSolve another puzzle? (Y/N) ";
    while(isspace(cin.peek()))
      cin.ignore();
    getline(cin,userInput);
    if (toupper(userInput[0]) == 'N')
      userContinues = false;
  }


  if (solution != NULL)
  {
    while(!solution->empty())
      solution->pop();
    delete solution;
  }
  if (m!=NULL) 
    delete m;

  return 0;
}
