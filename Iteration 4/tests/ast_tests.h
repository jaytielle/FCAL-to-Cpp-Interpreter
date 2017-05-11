/*******************************************************************************
 * Name            : tests/parser_tests.h
 * Project         : Labs
 * Module          : Iteration 1
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Dan Challou
 * Modifications by: John Harwell
 ******************************************************************************/
 #ifndef PROJECT_TESTS_AST_TESTS_H_
 #define PROJECT_TESTS_AST_TESTS_H_

#include <cxxtest/TestSuite.h>
#include <iostream>
#include "include/parser.h"
#include "include/read_input.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;
using namespace fcal;
using namespace parser;
using namespace ast;
using namespace scanner;

class AstTestSuite : public CxxTest::TestSuite
{
public:

    Parser p ;

    void writeFile ( const string text, const string filename) {
        ofstream out(filename.c_str());
        out << (string) text << endl ;
    }

    void unparse_tests ( string file) {
        string path = "./samples/" + file ;

        // 1. Test that the file can be parsed.
    		//! passed test on 4/6/17
    		/*! this was meant to test the scanner and ExtendedTokenList to see if they were able read to
    		    from the given file and generate a linked list of tokens
            */
            ParseResult pr1 = p.Parse(ReadInputFromFile( path.c_str()));
        TSM_ASSERT(file + " failed to parse.", pr1.ok()) ;

        // 2. Verify that the ast field is not null
    		//! passed test on 4/6/17
    		/*! this was meant to test if the parser was able to generate an AST
    		*/
        TSM_ASSERT(file + " failed to generate an AST.", pr1.ast() != NULL);

        // 3. Verify that the "unparsing" is non-empty.
    		//! passed test on 4/6/17
    		/*! this test is to see if the parser's unparse() was actually able to produce code and write it
    		    to another file
    		*/
        string up1 = pr1.ast()->unparse() ;
        writeFile(up1,(path+"up1").c_str());
        TSM_ASSERT(file + " failed to generate non-empty unparsing.",
                     up1.length() > 0) ;

        // 4. Verify that the un-parsed string can be parsed.
    		//! passed test on 4/6/17
    		/*! this test is to unparse() the file that we created with our original unparse)() of the FCAL
    		    file
    		*/
        ParseResult pr2 = p.Parse(up1.c_str()) ;
        TSM_ASSERT(file + " failed to parse the first un-parsing.",
                     pr2.ok()) ;

        // 5. Verify that the ast field is not null after first unparsing
    		//! passed test on 4/6/17
    		/*! this test is to check if the second unparsing was able to create another successful AST
    		*/
        TSM_ASSERT(file + " first unparsing failed to generate an AST.",
                     pr2.ast() != NULL);

        // 6. Verify that this second unparsing can be parsed.
    		//! passed test on 4/6/17
    		/*! this test will unparse() our second unparse() and create another file with the c++ language
    		*/
        string up2 = pr2.ast()->unparse() ;
        writeFile(up2,(path+"up2").c_str());
        ParseResult pr3 = p.Parse(up2.c_str()) ;
        TSM_ASSERT(file + " failed to unparse the second un-parsing.",
                     pr3.ok()) ;

        string up3 = pr3.ast()->unparse() ;
        writeFile(up3,(path+"up3").c_str());
        // 7. Verify that the first and second unparsings are the same.
    		//! passed test on 4/6/17
    		/*! this will check is the first unparse() is equivalent to the second unparse()
    		*/
        TSM_ASSERT_EQUALS(file + " unparse-1 != unparse-2.", up1, up2) ;
        // 8. Verifty that the second and third unparsings are the same.
    		//! passed test on 4/6/17
    		/*! this will check if the second unparsed() is equivalent to the third unparse() and in turn
    		    be checking if the first unparse() and third unparse() are equivalent
    		*/
        TSM_ASSERT_EQUALS(file + " unparse-2 != unparse-3.", up2, up3) ;
    }

    void test_sample_1(void) { unparse_tests("sample_1.dsl"); }
    void test_sample_2(void) { unparse_tests("sample_2.dsl"); }
    void test_sample_3(void) { unparse_tests("sample_3.dsl"); }
    void test_sample_4(void) { unparse_tests("sample_4.dsl"); }
    void test_sample_5(void) { unparse_tests("sample_5.dsl"); }
    void test_mysample(void) { unparse_tests("mysample.dsl"); }
    void test_forest_loss(void) { unparse_tests("forest_loss_v2.dsl"); }
} ;

#endif  // PROJECT_TESTS_AST_TESTS_H_
