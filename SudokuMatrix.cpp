//  SudokuMatrix.cpp
//  Author: Andy Giese
//  Date: June 2011
//  Purpose: Implements the SudokuMatrix ADT defined in SudokuMatrix.h

#include "SudokuMatrix.h"


SudokuMatrix::SudokuMatrix()
{
  Root = new Node();//points to upper left corner of matrix
  Root->header=true;
  Root->right=Root->left=Root->top=Root->bottom=Root; //Root points to itself in all directions
  workingSolution = new std::stack<Node>();
  Solved = false;
}
SudokuMatrix::~SudokuMatrix()
{
  deleteMatrix();
  delete Root;
  delete workingSolution;
}

bool SudokuMatrix::AddColumn(Node* newNode)
{
  if (!newNode->header)
    return false;
  else
    return AddColumnHelp(newNode,Root);
}

bool SudokuMatrix::AddColumnHelp(Node* newNode, Node* r)
{
  if (r->right == Root && r != newNode) //disallow duplicate pointer insertion
  {
    //add column to the end of the column list
    r->right->left = newNode;
    newNode->right = r->right;
    newNode->left = r;
    r->right = newNode;
    return true;
  }
  else if (r == newNode)
    return false;
  else
    return AddColumnHelp(newNode,r->right);

}


void SudokuMatrix::print()
{
  //simply print matrix to stdout
  if (Root==NULL)
    return;

  int count = 0;
  int colCount = 0;
  Node* printNode = Root->right;
  Node* printNodeNext = Root->right;
  while(printNodeNext != Root)
  {
    colCount++;
    printNode = printNodeNext->bottom;
    while(!printNode->header)
    {
      //std::cout << printNode->row << ", " << printNode->column << ", " << printNode->val << endl;
      printNode=printNode->bottom;
      count++;
    }
    printNodeNext = printNodeNext->right;
  }

  std::cout << "Counted " << count << " values, " << colCount << " Column Headers" << std::endl;
}

void SudokuMatrix::deleteMatrix()
{
  Node* deleteNextCol = Root->right;
  Node* deleteNextRow;
  Node* temp;
  while(deleteNextCol != Root)
  {
    deleteNextRow = deleteNextCol->bottom;
    while(!deleteNextRow->header)
    {
      temp = deleteNextRow->bottom;
      delete deleteNextRow;
      deleteNextRow = temp;
    }
    temp = deleteNextCol->right;
    delete deleteNextCol;
    deleteNextCol = temp;
  }

}

std::stack<Node>* SudokuMatrix::solve(const char* filename)
{
  //read in filename
  //find nodes corresponding to entries in puzzle, and add them to the partial solution
  //cover those nodes

  //perform algorithm X:
  /*
     if the matrix is empty, terminate successfully
     else choose a column c with the least 1s
     if (least number of 1s in a column is 0, terminate unsuccessfully)
     add c to partial solution
     cover c
     if (recurse on reduced matrix == unsuccessfull)
     uncover c, remove from partial solution
     else
     return successful, solution

   */
  Solved = false;
  totalCompetition = 0;
  while(!workingSolution->empty())
    workingSolution->pop();
  std::ifstream fin;
  fin.open(filename);
  if (fin.fail())
  {
    std::cout << "Error, could not open " << filename << " for reading" << std::endl;
    fin.close();
    return NULL;

  }
  int nextVal;

  Node* toFind = NULL;
  Node* insertNext = NULL;
  std::stack<Node*> puzzleNodes;//list of nodes that were covered as part of reading in puzzle
  Node* colNode,*nextRowInCol,*rowNode;
  for (int i=0;i<MATRIX_SIZE;i++)//iterates through rows
  {
    for (int j=0;j<MATRIX_SIZE;j++) //iterates through columns
    {
      fin >> nextVal;
      if (fin.fail() || nextVal > MATRIX_SIZE || nextVal < 0)
      {
	std::cout << "Invalid Sudoku Puzzle specified" << std::endl;
	fin.close();
	return NULL;
      }

      if (nextVal != 0)
      {

	toFind = new Node(i,j,nextVal-1);
	insertNext = find(toFind);
	if (insertNext == NULL)
	{
	  std::cout<<"Error in Sudoku Puzzle " << i << ", " << j << " val= " << nextVal << std::endl;
	  fin.close();
	  return NULL;
	}
	colNode = insertNext->colHeader;
	//print();
	nextRowInCol = insertNext;
	cover(colNode);

	rowNode = nextRowInCol->right;
	while(rowNode != nextRowInCol)
	{
	  cover(rowNode->colHeader);
	  rowNode = rowNode->right;
	}


	puzzleNodes.push(insertNext);
	workingSolution->push(*insertNext);
	delete toFind;
      }
    }
  }
  fin.close();


  std::cout << "Solving..." << std::endl;

  if (solve())
    std::cout << "Puzzle solved successfully!" << std::endl;
  else
    std::cout << "Puzzle not solveable!" << std::endl;

  //print();


  //uncover the nodes that we covered earlier, so that we can solve a different puzzle
  //in the future
  while(!puzzleNodes.empty())
  {
    colNode = (puzzleNodes.top())->colHeader;
    nextRowInCol = (puzzleNodes.top());


    rowNode = nextRowInCol->right;
    while(rowNode != nextRowInCol)
    {
      uncover(rowNode->colHeader);
      rowNode = rowNode->right;
    }
    uncover(colNode);
    puzzleNodes.pop();
  }

  std::stack<Node> temp,*toRet;
  while(!workingSolution->empty())
  {
    temp.push(workingSolution->top());
    workingSolution->pop();
  }
  toRet = new std::stack<Node>();
  while(!temp.empty())
  {
    toRet->push(temp.top());
    temp.pop();
  }



  return toRet;
}

void SudokuMatrix::cover(Node* r)
{
  Node *RowNode, *RightNode,*ColNode=r->colHeader;
  ColNode->right->left = ColNode->left;
  ColNode->left->right = ColNode->right;
  for(RowNode = ColNode->bottom; RowNode!=ColNode; RowNode = RowNode->bottom)
  {
    for(RightNode = RowNode->right; RightNode!=RowNode; RightNode = RightNode->right) 
    {
      RightNode->top->bottom = RightNode->bottom;
      RightNode->bottom->top = RightNode->top;
    }
  }
}

void SudokuMatrix::uncover(Node* r)
{
  Node *RowNode, *LeftNode,*ColNode=r->colHeader;
  for(RowNode = ColNode->top; RowNode!=ColNode; RowNode = RowNode->top) 
  {
    for(LeftNode = RowNode->left; LeftNode!=RowNode; LeftNode = LeftNode->left) {
      LeftNode->top->bottom = LeftNode;
      LeftNode->bottom->top = LeftNode;
    }
  }
  ColNode->right->left = ColNode;
  ColNode->left->right = ColNode;
}

bool SudokuMatrix::isEmpty()
{
  return (Root->right == Root);
}

bool SudokuMatrix::solve()
{
  if (isEmpty())
    return true; //matrix is empty, solutions is filled

  int numCols;
  Node* nextCol = NULL;
  nextCol = chooseNextColumn(numCols);

  if (numCols < 1)
    return false;

  totalCompetition += numCols;

  Node* nextRowInCol = nextCol->bottom;
  Node* rowNode;
  cover(nextCol);

  //need check for solved so that matrix is successfully uncovered after solve, for memory management purposes
  while(nextRowInCol != nextCol && !Solved) 
  {
    workingSolution->push(*nextRowInCol);
    //cover(nextRowInCol);
    rowNode = nextRowInCol->right;
    while(rowNode != nextRowInCol)
    {
      cover(rowNode->colHeader);
      rowNode = rowNode->right;
    }


    Solved=solve();
    if (!Solved)
    {
      workingSolution->pop();
    }


    rowNode = nextRowInCol->right;
    while(rowNode != nextRowInCol)
    {
      uncover(rowNode->colHeader);
      rowNode = rowNode->right;
    }

    nextRowInCol = nextRowInCol->bottom;
  }

  uncover(nextCol);

  return Solved; //could not satisfy constraints of this column
}

Node* SudokuMatrix::chooseNextColumn(int& count)
{
  Node* currentBest = Root->right;
  int best = -1;
  int tempCount = 0;

  //iterate through currentBest and count nodes, then iterate through currentBest's neighbors and count their nodes

  Node* next = currentBest->bottom;
  Node* nextCol = currentBest;
  while(nextCol != Root)
  {
    next = nextCol->bottom;
    tempCount = 0;
    while(next != nextCol)
    { 
      if (next == next->bottom)
      {
	std::cout<<"Err!" << std::endl;
      }
      tempCount++;
      next = next->bottom;
    }
    if (tempCount < best || best == -1)
    {
      currentBest = nextCol;
      best = tempCount;
    }
    nextCol = nextCol->right;
  }

  if (currentBest == Root) //then we have a problem
  {
    std::cout << "Attempted to choose column from empty matrix!" << std::endl;
    exit(-1);
  }

  count = best;
  return currentBest;
}

Node* SudokuMatrix::find(Node* find)
{
  Node* rightNode,*bottomNode;
  rightNode = Root->right;
  while(rightNode != Root) //iterate through column headers
  {
    bottomNode = rightNode->bottom;
    while(bottomNode != rightNode) //iterate through columns
    {
      if (bottomNode->row == find->row && bottomNode->column == find->column && bottomNode->value == find->value)
      {
	return bottomNode;
      }
      bottomNode = bottomNode->bottom;
    }
    rightNode = rightNode->right;
  }

  return NULL;//not found
} //end find method

bool SudokuMatrix::initialize()
{

  //Step 1: Construct maximum matrix (324x729)
  //Step 2: Use maximum matrix to build dancing links structure (matrix A)
  //Step 3: Read in initial sudoku grid of 1s and 0s
  //Step 4: Parse sudoku grid for {r,c,b,v}, and add those to the partial solution S* (removing from A)
  //r=row c=column b=box v=value (1,2,3,...,9)
  //Step 5: Perform Algorithm X on remaining A until exact cover is found --> S* is final solution
  //Step 6: Output final solution

  Node* matrix[MAX_ROWS][MAX_COLS];
  //initialize matrix
  for (int i=0;i<MAX_ROWS;i++)
  {
    for (int j=0;j<MAX_COLS;j++)
    {
      matrix[i][j] = NULL;
    }
  }

  //set appropriate non-null values in matrix
  int row=0;
  Node *rowNode,*colNode,*cellNode,*boxNode;
  for (int i=0;i<MATRIX_SIZE;i++) //rows
  {
    for (int j=0;j<MATRIX_SIZE;j++) //columns
    {
      for (int k=0;k<MATRIX_SIZE;k++) //values
      {
	row = (i*COL_OFFSET+j*MATRIX_SIZE+k);

	//each one of these 729 combinations of r,c,and v results in 4 constraints being satisfied in our grid
	//i.e. 4 NULL values changed to Non-Null values
	//this is a *very* sparse matrix, which is why it will be converted to a DLX (Dancing Links) representation
	rowNode=matrix[row][ROW_OFFSET+(i*MATRIX_SIZE+k)] = new Node(i,j,k);
	colNode=matrix[row][COL_OFFSET+(j*MATRIX_SIZE+k)] = new Node(i,j,k);
	cellNode=matrix[row][CELL_OFFSET+(i*MATRIX_SIZE+j)] = new Node(i,j,k);
	boxNode=matrix[row][BOX_OFFSET+((i/ROW_BOX_DIVISOR + j/COL_BOX_DIVISOR * COL_BOX_DIVISOR)*MATRIX_SIZE+k)] = new
	  Node(i,j,k);

	//link the nodes we just created to save time later
	rowNode->right =colNode;
	rowNode->left=boxNode;
	colNode->left=rowNode;
	colNode->right=cellNode;
	cellNode->left=colNode;
	cellNode->right=boxNode;
	boxNode->left=cellNode;
	boxNode->right=rowNode;

      }
    }
  }

  //write matrix to a text file
  /*ofstream fout;
    fout.open("CoverMatrix.txt");

    for (int i=0;i<MAX_ROWS;i++)
    {
    for (int j=0;j<MAX_COLS;j++)
    {
    if (matrix[i][j] == NULL)
    fout << "0";
    else
    fout << "1";

    }
    fout << endl;
    }

    fout.close();*/

  //Link columns together for dancing links
  Node* nextColHeader;
  Node* nextColRow;
  for (int j=0;j<MAX_COLS;j++)
  {
    nextColHeader = new Node();
    nextColHeader->header=true;
    //link colHeader to self
    nextColHeader->top=nextColHeader->bottom=nextColHeader->left=nextColHeader->right=nextColHeader->colHeader=nextColHeader;
    nextColRow = nextColHeader;

    for (int i=0;i<MAX_ROWS;i++)
    {
      if (matrix[i][j] != NULL)
      {
	//search down rows to add to column
	matrix[i][j]->top=nextColRow;
	nextColRow->bottom = matrix[i][j];
	matrix[i][j]->bottom = nextColHeader;
	nextColHeader->top = matrix[i][j];
	matrix[i][j]->colHeader = nextColHeader;
	nextColRow = matrix[i][j];
      }
    }
    if (nextColHeader->bottom == nextColHeader)
    {
      std::cout<<"Err! column has no rows! col:" << j << std::endl;
    }
    if(!AddColumn(nextColHeader))
    {
      std::cout << "Error in adding column to matrix" << std::endl;

      //cleanup
      for (int i=0;i<MAX_ROWS;i++)
      {
	for (int j=0;j<MAX_COLS;j++)
	{
	  if (matrix[i][j] != NULL)
	    delete matrix[i][j];
	}
      }
      return false;



    }
  }

  return true;
}//end of initialize function
