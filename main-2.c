#include <stdio.h>
#include <stdlib.h>

/*------------------
Code based off of class website 
1) Huffman coding for Notes 06 
2) C code for optimal matrix multiply (dynamic programming)

Lab 3: Takes in probabilty floats for number of 
alphabets. Does optimal matrix calculation and outputs 
trees and the code and probs* of bits and total expected bits
per symbol

Marium Mannan
UTA ID: 1001541605
------------------
Command to compile:
1) gcc main.c -o output
2) ./output < a.dat
------------------*/


//max values lab can take: 25
static int totalP;//total number of probabilities
float work; //temp work
float sum; //used for summation
static float p[25];//all initial probability read from file
static float c[25][25];//cost matrix
static int trace[25][25];//trace matrix
static int leaf[25][2];//keeps track of depth
static int leafCounter =0;//counter for depth array

/*-----------
Description: Sums from left to right like summation

Parameters: takes in the start and end value of summation

Return: returns the summation
------------*/
float summation(float left, float right){
  int i;
  sum = 0;
  //sum all values from left to right
  for(i = left; i <= right; i++){
    sum = p[i] + sum;
  }
  //return the sum
  return sum;
}


/*
Description: Solves the matrix diagonally after the 
first two columns since new values cannot be calculated
line by line 

Parameters: none (global vars)

Return: none (updated golbally)
*/
void diagonalSolver(){

  //since first two diagonals of matrix can be calculated
  //without other values it starts at the third diagonal
  int i, j, p;
  
  int k;

  //go diagonally
  for(p = 2; p < totalP; p++){
    i = 0; j = p;
    while(j < totalP){
      
      //assume min is the first value
      float min = c[i][i]+c[i+1][j];
      int minK = i; //k value for minimum (used for trace)
      for(k = i+1; k < j; k++){
        //replace min if you find smaller val
        if(min > c[i][k] + c[k+1][j]){
          min = c[i][k] + c[k+1][j];
          minK = k;
        }    
      }
      //cost function from notes 7 p10
      c[i][j] = min + summation(i,j);
      trace[i][j] = minK; //notes 7
      i++; j++;
    }
  }

}


/*
Description: print the matrix

Parameters: none (global vars)

Return: none (print to screen)
*/
void matrixPrinter(){
  printf("  ");
  int i, j;
  //print the first row headers
  for(i = 0; i < totalP; i++){
    printf("          %5d     ",i);
  }
  printf("\n");

  //print column # and values
  for(i = 0; i < totalP; i++){
    printf("%d ", i);
    for(j = 0; j < totalP; j++){
    
      if(i > j){//invalid c[i][j]
        printf("       ------       ");
      }

      else{
        printf("       %f %d   ", c[i][j], trace[i][j]);
      }
    }
    printf("\n\n");
  }
}

/*
Description: print the tree 

Parameters: left branch, right branch and how much to indent the 
ouput values

Return: none (print to screen)
*/
static void tree(int left, int right, int indent)
{
  if (left==right) //Leaf - base case
  {
    printf("%*s%d\n",3*indent,"",left); //Indent and print the index of the matrix

    //saving the depth
    leaf[leafCounter][1] = indent;
    leaf[leafCounter++][0] = left; //leftmost value
    return;
  }


  else//recursive case
  {
    tree(trace[left][right]+1,right,indent+1);  //Prints the right subtree for c(left,right)

    printf("%*s%d %d cost %f\n",3*indent,"",left,right,c[left][right]); //Print the root

    tree(left,trace[left][right],indent+1); //Prints the left subtree for c(left,right)

  }
  
}
/*
Description: calculates the matrix cost function

Parameters: none (global vars)

Return: none (saves to global vars)
*/

void matrix(){

  int i, j, k;

  //for all c[i][j]
  for(i = 0; i < totalP; i++){
    for(j = i; j < totalP; j++){

      //if i == j c[i][j] = 0//notes 7
      int diff = j -i;
      if(diff == 0){
        work = 0;
        trace[i][j] = 0;
        
      }   
      //if i-j = 1 then only one k value which is i
      //no need to use k
      if (diff == 1){
        work = c[i][i]+ c[j][j] + summation(i,j);
        trace[i][j] = i;
      }  
      c[i][j] = work; //save to work matrix
    }
  }

  diagonalSolver(); //solve the rest diagonally
  matrixPrinter(); //print the whole matrix
   
}

/*
Description: gets expected bits and huff code and total bits

Parameters: none (global vars)

Return: none (prints to screen)
*/
void expectedBits(){
  printf("Codes & prob*# of bits\n");
  float totalCount = 0; //sum of prob bits

  //iterate left 0 and right 1 to get huff code
  int i;
  for(i = 0; i < totalP; i++){
    int left = 0; 
    int right = totalP-1;

    //bit we are working with
    printf("%d ", i);

    while(left < right ){
      if(i <= trace[left][right]){
        printf("0");
        right = trace[left][right];
      }
      else{
        printf("1");
        left = trace[left][right]+1;
      }
    }
    
    //print the depth * probabiltiy 
    //depth save in tree function
    //leaf was saved bottom up so we use totalP - i -1
    printf(" %f\n",leaf[totalP - i -1][1] * p[i]);

    //sum all probs
    totalCount = totalCount + leaf[totalP - i -1][1] * p[i];
    
  }
  //print all the probs
  printf("Expected bits per symbol: %lf\n", totalCount);

  
}


/*
  Main function:
  save the probabilties from file
*/
int main(){
  int i;
  //get total values working with
  scanf("%d",&totalP);

  //read all the p values
  for(i= 0; i < totalP; i++){
    scanf("%f", &p[i]);
  }

  //get the matrix
  matrix();
  //get the tree 
  tree(0, totalP-1, 0);
  //get expected bits + code + total
  expectedBits();

}
