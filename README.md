# Huffman-Code
Uses C to implement order-preserving Huffman coding - using the dynamic programming formulation.

The input is 1) a positive integer n and 2) a sequence of n doubles giving the probabilities for
symbols in an ordered character set. To simplify output, the character set is indexed as 0 . . . n - 1. 

Your program outputs 1) the cost matrix, 2) the optimal order-preserving Huffman code tree, 3)
the bit code assigned to each symbol, and 4) the expected bits per symbols based
on the generated code tree and the input probabilities


To run use 
gcc -o main.c ./a 
a.out < sample_input.dat
