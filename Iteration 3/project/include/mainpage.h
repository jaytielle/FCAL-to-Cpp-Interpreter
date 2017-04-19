
/*******************************************************************************
 * Name            : scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Header file for scanner module
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell
 ******************************************************************************/
#ifndef PROJECT_INCLUDE_MAINPAGE_H_
#define PROJECT_INCLUDE_MAINPAGE_H_
/*! \mainpage Interpreter Main Page

\section intro_sec Introduction

This is the introduction to iteration 3 of the interpreter project. So far we have created the
scanner and parser for the interpreter. The scanner will read from a file and create a linked list of tokens
that all contain Enumerated Tokentypes and using these Enumerated Tokentypes the parser is then able
to generate an Abstract Syntax Tree (AST). The linked list of tokens is passed to the parser and using the Tokentypes
is able to parse them into an AST and with each Node in the AST is able to unparse which will generate
c++ code equivalent to the FCAL language we are interpreting from

\subsection Scanner

The scanner reads in characters from another file and and using regex expressions the scanner is able to 
categorize which characters are which Enumerated Tokentype. At the same time the scanner is also scanning for
white space which it gets rid of using the regex for white space and bypasses the white space by moving the pointer
reading the input file. After each character is properly categorized it is placed as a Token type in a linked last.

\subsection Parser

The Parser reads in the Token linked list from the scanner and goes through each Token in the linked list and
generates a subclass according to the TokenType of each Token in the linked list. The first class generated is always
the Root class which is the root of the AST that will be generate by the Parser. After this Root class has been generated
other Stmt, Stmts, Expr, and Decl subclasses will be generated according to the TokenTypes of the rest of the Tokens
in the Token linked list that was passed by the Scanner.
*/
#endif  // PROJECT_INCLUDE_MAINPAGE_H_
