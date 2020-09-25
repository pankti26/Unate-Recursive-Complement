# Unate-Recursive-Complement
Complement of Boolean Expressions in SOP form

This code finds the complement of Boolean Expressions in SOP form represented as a PCN (Positional Cube Notation) cube list. It uses the idea of Unate Recursive Paradigm. URC is based on Shannon Cofactoring. The boolean function F is split into positive and negative co-factors. The complement of the function F can be computed from the complement of the individual co-factors. The splitting is performed in a recursive manner till the cube list is simple and can be complemented directly. 

The input file (.pcn) consists of a boolean function in cube list format. The first line gives the number of variables in the function. The second line gives the number of cubes in the cube list. The subsequent lines represent the cube lists. The first number tells how many variables are not don't cares. The rest of the numbers give the variables in true or complemented form. E.g. x1x5x9' is written as <3 1 5 -9>. 

The output file is a .txt file in the same format as the input file. 
