/*******************************************************************************
 * Name            : tests/scanner_tests.h
 * Project         : Labs
 * Module          : Iteration 1
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Dan Challou
 * Modifications by: John Harwell, Tommy Her, Jennifer Lieu
 ******************************************************************************/

#ifndef PROJECT_TESTS_SCANNER_TESTS_H_
#define PROJECT_TESTS_SCANNER_TESTS_H_

#include <cxxtest/TestSuite.h>

#include <stdio.h>

#include <iostream>
#include <string>

#include "include/read_input.h"
#include "include/scanner.h"
#include "include/regex.h"

namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite {
 public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    void test_setup_code() {
      s = new scanner::Scanner();
    }

    bool token_type_tester(const char *text,
      const char* lexeme, fcal::scanner::TokenType terminal) {
      scanner::Token* generated_token = s->Scan(text);
      if (generated_token == NULL) {
        return false;
      } else if (generated_token->get_terminal_() != terminal ||
        generated_token->get_lexeme_() !=  lexeme) {
        return false;
      } else {
        return true;
      }
    }  // token_type_tester

    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.

    // REGEX MAKE/MADE TEST CASES

    // Regex for whitespaces and block comments
    void test_make_matchRegex_white_space(void) {
      regex_t *re = fcal::scanner::make_regex("^[\n\t\r ]+");
      TS_ASSERT(re);
      int numMatchedChars = fcal::scanner::match_regex(re, " ");
      TS_ASSERT(numMatchedChars == 1);
    }

    void test_make_matchRegex_block_comment(void) {
      regex_t *re =
        fcal::scanner::make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
      TS_ASSERT(re);
      int numMatchedChars = fcal::scanner::match_regex(re, "/**/");
      TS_ASSERT(numMatchedChars == 4);
    }

    void test_make_matchRegex_single_comment(void) {
      regex_t *re = fcal::scanner::make_regex("^(\\/\\/)[^\n\r]*(\r|\n)+");
      TS_ASSERT(re);
      int numMatchedChars = fcal::scanner::match_regex(re, "//a\n");
      TS_ASSERT(numMatchedChars == 4);
    }

    // TA commented in iteration 1 that these were redundant and unnecessary
    // Regex for special keywords

    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */

    // TOKEN TYPE (TERMINAL) TEST CASES
    void test_terminal_intKwd(void) {
      TS_ASSERT(token_type_tester("int ", "int",
        fcal::scanner::kIntKwd));
    }

    void test_terminal_floatKwd(void) {
      TS_ASSERT(token_type_tester("Float ", "Float",
        fcal::scanner::kFloatKwd));
    }

    // March 3: Added terminal test cases for booleans
    void test_terminal_boolKwd(void) {
      TS_ASSERT(token_type_tester("bool  ", "bool",
        fcal::scanner::kBoolKwd));
    }

    void test_terminal_trueKwd(void) {
      TS_ASSERT(token_type_tester("true ", "true",
        fcal::scanner::kTrueKwd));
    }

    void test_terminal_falseKwd(void) {
      TS_ASSERT(token_type_tester("false ", "false",
        fcal::scanner::kFalseKwd));
    }

    void test_terminal_stringKwd(void) {
      TS_ASSERT(token_type_tester("string ", "string",
        fcal::scanner::kStringKwd));
    }

    void test_terminal_matrixKwd(void) {
      TS_ASSERT(token_type_tester("matrix number", "matrix",
        fcal::scanner::kMatrixKwd));
    }

    void test_terminal_letKwd(void) {
      TS_ASSERT(token_type_tester("let ", "let",
        fcal::scanner::kLetKwd));
    }

    void test_terminal_inKwd(void) {
      TS_ASSERT(token_type_tester("in  ", "in",
        fcal::scanner::kInKwd));
    }

    void test_terminal_endKwd(void) {
      TS_ASSERT(token_type_tester("end ", "end",
        fcal::scanner::kEndKwd));
    }

    void test_terminal_ifKwd(void) {
      TS_ASSERT(token_type_tester("if ", "if",
        fcal::scanner::kIfKwd));
    }

    void test_terminal_thenKwd(void) {
      TS_ASSERT(token_type_tester("then ", "then",
        fcal::scanner::kThenKwd));
    }

    void test_terminal_elseKwd(void) {
      TS_ASSERT(token_type_tester("else ", "else",
        fcal::scanner::kElseKwd));
    }

    void test_terminal_repeatKwd(void) {
      TS_ASSERT(token_type_tester("repeat", "repeat",
        fcal::scanner::kRepeatKwd));
    }

    // March 3: Added while terminal test
    void test_terminal_whileKwd(void) {
      TS_ASSERT(token_type_tester("while ", "while",
        fcal::scanner::kWhileKwd));
    }

    void test_terminal_printKwd(void) {
      TS_ASSERT(token_type_tester("print ", "print",
        fcal::scanner::kPrintKwd));
    }

    void test_terminal_toKwd(void) {
      TS_ASSERT(token_type_tester("to ", "to",
        fcal::scanner::kToKwd));
    }

    void test_terminal_intConst(void) {
      TS_ASSERT(token_type_tester("1", "1",
        fcal::scanner::kIntConst));
    }

    void test_terminal_floatConst(void) {
      TS_ASSERT(token_type_tester("1.20", "1.20",
        fcal::scanner::kFloatConst));
    }

    void test_terminal_stringConst(void) {
      TS_ASSERT(token_type_tester("\"\"", "\"\"",
        fcal::scanner::kStringConst));
    }

    void test_terminal_variableName(void) {
      TS_ASSERT(token_type_tester("a ", "a",
        fcal::scanner::kVariableName));
    }

    void test_terminal_leftParen(void) {
      TS_ASSERT(token_type_tester("(", "(",
        fcal::scanner::kLeftParen));
    }

    void test_terminal_rightParen(void) {
      TS_ASSERT(token_type_tester(") ", ")",
        fcal::scanner::kRightParen));
    }

    void test_tmerinal_leftCurly(void) {
      TS_ASSERT(token_type_tester("{ ", "{",
        fcal::scanner::kLeftCurly));
    }

    void test_terminal_rightCurly(void) {
      TS_ASSERT(token_type_tester("} ", "}",
        fcal::scanner::kRightCurly));
    }

    void test_terminal_leftSquare(void) {
      TS_ASSERT(token_type_tester("[ ", "[",
        fcal::scanner::kLeftSquare));
    }

    void test_terminal_rightSquare(void) {
      TS_ASSERT(token_type_tester("] ", "]",
        fcal::scanner::kRightSquare));
    }

    void test_terminal_semiColon(void) {
      TS_ASSERT(token_type_tester(" ; ", ";",
        fcal::scanner::kSemiColon));
    }

    void test_terminal_colon(void) {
      TS_ASSERT(token_type_tester(" :", ":",
        fcal::scanner::kColon));
    }

    void test_terminal_assign(void) {
      TS_ASSERT(token_type_tester("= ", "=",
        fcal::scanner::kAssign));
    }

    void test_terminal_plusSign(void) {
      TS_ASSERT(token_type_tester("+ ", "+",
        fcal::scanner::kPlusSign));
    }

    void test_terminal_star(void) {
      TS_ASSERT(token_type_tester(" *", "*",
        fcal::scanner::kStar));
    }

    void test_terminal_dash(void) {
      TS_ASSERT(token_type_tester("-     ", "-",
        fcal::scanner::kDash));
    }

    void test_terminal_forwardSlash(void) {
      TS_ASSERT(token_type_tester("/", "/",
        fcal::scanner::kForwardSlash));
    }

    void test_terminal_lessThan(void) {
      TS_ASSERT(token_type_tester(" <", "<",
        fcal::scanner::kLessThan));
    }

    void test_terminal_lessThanEqual(void) {
      TS_ASSERT(token_type_tester("<=", "<=",
        fcal::scanner::kLessThanEqual));
    }

    void test_terminal_greaterThan(void) {
      TS_ASSERT(token_type_tester("> ", ">",
        fcal::scanner::kGreaterThan));
    }

    void test_terminal_greaterThanEqual(void) {
      TS_ASSERT(token_type_tester(">= ", ">=",
        fcal::scanner::kGreaterThanEqual));
    }

    void test_terminal_equalsEquals(void) {
      TS_ASSERT(token_type_tester(" ==", "==",
        fcal::scanner::kEqualsEquals));
    }

    void test_terminal_notEquals(void) {
      TS_ASSERT(token_type_tester("!= ", "!=",
        fcal::scanner::kNotEquals));
    }

    void test_terminal_andOp(void) {
      TS_ASSERT(token_type_tester(" &&", "&&",
        fcal::scanner::kAndOp));
    }

    void test_terminal_orOp(void) {
      TS_ASSERT(token_type_tester("|| ", "||",
        fcal::scanner::kOrOp));
    }

    void test_terminal_notOp(void) {
      TS_ASSERT(token_type_tester("! ", "!",
        fcal::scanner::kNotOp));
    }

    void test_terminal_lexicalError(void) {
      TS_ASSERT(token_type_tester("$ ", "$",
        fcal::scanner::kLexicalError));
    }

    void test_terminaL_endOfFile(void) {
      TS_ASSERT(token_type_tester("", "EOF",
        fcal::scanner::kEndOfFile));
    }

    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors(scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->get_terminal_() == scanner::kLexicalError) {
              printf("problem: %s\n", currentToken->get_lexeme_().c_str());
              fflush(stdout);
                                return false;
            } else {
              currentToken = currentToken->get_next_();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while (currentToken != NULL && termIndex < numTerms) {
            if (currentToken->get_terminal_() != ts[termIndex]) {
              printf("item at index: %i: terminal: %i should be terminal: %i\n",
              termIndex, currentToken->get_terminal_(), ts[termIndex]);
              fflush(stdout);
              return false;
            } else {
                ++termIndex;
                currentToken = currentToken->get_next_();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kEndOfFile);
        TS_ASSERT(tks->get_next_() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kEndOfFile);
        TS_ASSERT(tks->get_next_() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
    void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->get_lexeme_(), "$");
        tks = tks->get_next_();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->get_lexeme_(), "&");
        tks = tks->get_next_();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->get_lexeme_(), "1");
        tks = tks->get_next_();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->get_terminal_(), scanner::kEndOfFile);
        TS_ASSERT(tks->get_next_() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kVariableName,
          scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT(sameTerminals(tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst,
          scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst,
          scanner::kStringConst, scanner::kFloatConst,
          scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd, scanner::kSemiColon,
          scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen,
          scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar,
          scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd,
          scanner::kStringKwd, scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT(sameTerminals(tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }
};

#endif  // PROJECT_TESTS_SCANNER_TESTS_H_
