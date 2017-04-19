/* A small meaningless comment */

/*******************************************************************************
 * Name            : scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Header file for scanner module
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_SCANNER_H_
#define PROJECT_INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <regex.h>
#include <string>
/*******************************************************************************
 * Namespaces

 ******************************************************************************/
namespace fcal {
namespace scanner {

/* below are the possible terminal types for each token */
/* DO NOT remove any of these declarations */
  enum kTokenEnumType {
    /*0*/ kIntKwd,
    kFloatKwd,
    kStringKwd,
    kMatrixKwd,
    /*4*/ kLetKwd,
    kInKwd,
    kEndKwd,
    kIfKwd,
    kThenKwd,
    kElseKwd,
    /*10*/ kRepeatKwd,
    kPrintKwd,
    kToKwd,

    // Constants
    /*13*/ kIntConst,
    kFloatConst,
    kStringConst,

    // Names
    /*16*/ kVariableName,

    // Punctuation
    /*17*/ kLeftParen,
    kRightParen,
    kLeftCurly,
    kRightCurly,
    /*21*/ kLeftSquare,
    kRightSquare,

    kSemiColon,
    kColon,

    // Operators
    /*25*/ kAssign,
    kPlusSign,
    kStar,
    kDash,
    kForwardSlash,
    /*30*/ kLessThan,
    kLessThanEqual,
    kGreaterThan,
    kGreaterThanEqual,
    /*34*/ kEqualsEquals,
    kNotEquals,
    /*36*/ kAndOp,
    kOrOp,
    kNotOp,

    // Special terminal types
    /*39*/ kEndOfFile,
    kLexicalError
  };

  typedef enum kTokenEnumType TokenType;

/*******************************************************************************
 * Class Definitions
 *
 * You will need to define a Token class (remember, a pointer of type Token *
 * is returned by the scan method of the scanner class.)
 * Also, Classes are typically declared (and sometimes also defined) in
 * their own header files
 *
 * You will also need to define a Scanner class with a scan and other methods
 ******************************************************************************/
class Token {
 public:
  // Token constructors
  Token();
  Token(TokenType terminal, std::string lexeme, Token* next);
    // :  terminal_(currentType), lexeme_(currentLexeme), next_(nextToken) { }
  ~Token();

  TokenType get_terminal_();
  void set_terminal_(TokenType terminal);
  std::string get_lexeme_();
  void set_lexeme_(std::string lexeme);
  Token* get_next_();
  void set_next_(Token* next);

  void return_terminal_();
  void return_lexeme_();
  void return_next_();

  int length_of_lexeme_;

 private:
  TokenType terminal_;
  std::string lexeme_;
  Token* next_;
};  // Token class

class Scanner {
 public:
  Scanner();  // calls to make_str_regex_token_array
  ~Scanner();
  Token* Scan(const char *text);
  Token* Token_Generator(const char *text);
  Token* EOF_Token_Generator(const char *text);

  bool token_type_tester(const char *text, const char *regex_str,
    TokenType terminal);
  bool end_token_tester(const char *text, const char *regex_str,
    TokenType terminal);

  struct regex_str_token_pair{
    std::string regex_str;
    TokenType token_type;
  };

  struct make_str_regex_token_set{
    std::string regex_str;
    regex_t *regex_made;
    TokenType token_type;
  };

  int consume_whitespace_and_comments(regex_t *white_space,
    regex_t *block_comment, regex_t *single_comment, const char *text);
};  // Scanner class

} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_SCANNER_H_
