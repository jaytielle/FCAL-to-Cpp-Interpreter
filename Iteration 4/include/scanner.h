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
#include "../include/regex.h"
/*******************************************************************************
 * Namespaces

 ******************************************************************************/
namespace fcal {
namespace scanner {

// March 3: Updated with Bool, True, False and While kTokenEnumTypes
/* below are the possible terminal types for each token */
/* DO NOT remove any of these declarations */
enum kTokenEnumType {
  kIntKwd,
  kFloatKwd,
  kBoolKwd,
  kTrueKwd,
  kFalseKwd,
  kStringKwd,
  kMatrixKwd,
  kLetKwd,
  kInKwd,
  kEndKwd,
  kIfKwd,
  kThenKwd,
  kElseKwd,
  kRepeatKwd,
  kWhileKwd,
  kPrintKwd,
  kToKwd,

  // Constants
  kIntConst,
  kFloatConst,
  kStringConst,

  // Names
  kVariableName,

  // Punctuation
  kLeftParen,
  kRightParen,
  kLeftCurly,
  kRightCurly,
  kLeftSquare,
  kRightSquare,
  kSemiColon,
  kColon,

  // Operators
  kAssign,
  kPlusSign,
  kStar,
  kDash,
  kForwardSlash,
  kLessThan,
  kLessThanEqual,
  kGreaterThan,
  kGreaterThanEqual,
  kEqualsEquals,
  kNotEquals,
  kAndOp,
  kOrOp,
  kNotOp,

  // Special terminal types
  kEndOfFile,
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
/*! The Token class defines the various methods and attributes associated with
    the token types listed in the enum kTokenEnumType.
*/
class Token {
 public:
  //! Token() constructor
  Token();
  /*! Token production takes two parameters: lexeme and next
      \param lexeme is the lexeme of the token
	  \param next is the next token in the linked Token list
  */
  Token(TokenType terminal, std::string lexeme, Token* next);
    // :  terminal_(currentType), lexeme_(currentLexeme), next_(nextToken) { }
  //! Token() deconstructor
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
  /*! Token private members: terminal_, lexeme_ and *next_
      \member terminal_ refers to the type of a token
      \member lexeme_ refers to the string associated with the token
      \member *next_ is a pointer to the next token in the array list
  */
  TokenType terminal_;
  std::string lexeme_;
  Token* next_;
};  // Token class

/*! The Scanner class defines the various methods and attributes associated with
    scanning in a file to determine the token types.
*/
class Scanner {
 public:
  //! Scanner() constructor
  Scanner();
  ~Scanner();
  //! Initalizes an array of regular expressions associated with its token type
  void InitRegexTokenArray();
  //! Scan muethod that reads through a file to determine token type matches
  Token* Scan(const char *text);

  int consume_whitespace_and_comments(regex_t *white_space,
    regex_t *block_comment, regex_t *single_comment, const char *text);

  regex_t *regex_token_array[45];
  // inits the regex_token_array as a variable to be used by the entire class

 private:
  // initalize whitespace and comment regexes
  regex_t *white_space;
  regex_t *block_comment;
  regex_t *single_comment;
};  // Scanner class

} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_SCANNER_H_
