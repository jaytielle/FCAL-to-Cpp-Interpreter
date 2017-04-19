/*******************************************************************************
 * Name            : scanner.cc
 * Project         : Iteration 1
 * Module          : scanner
 * Description     : This file provides Scanner and Token Classes
 * Copyright       : 2017 CSCI3081W. All rights reserved.
 * Original Author : Jennifer Lieu, Tommy Her
 * Modifications by: Jennifer Lieu, Tommy Her
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string>

#include "../include/scanner.h"
#include "../include/regex.h"
/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

  // Token constructors
  Token::Token() {
    terminal_ = kLexicalError;
    lexeme_ = "";
    next_ = nullptr;
  }

  Token::Token(TokenType terminal, std::string lexeme, Token* next) {
    this->terminal_ = terminal;
    this->lexeme_ = lexeme;
    this->next_ = next;
  }

  // Token deconstructor
  Token::~Token() {
  }

  // Token accessor and setter functions
  //! Token terminal_ accessor method
  TokenType Token::get_terminal_() {
    return this->terminal_;
  }

  //! Token terminal_ mutator method
  void Token::set_terminal_(TokenType terminal) {
    this->terminal_ = terminal;
  }

  //! Token lexeme_ accessor method
  std::string Token::get_lexeme_() {
    return this->lexeme_;
  }

  //! Token lexeme_ mutator method
  void Token::set_lexeme_(std::string lexeme) {
    this->lexeme_ = lexeme;
  }

  //! Token next_ accessor method
  Token* Token::get_next_() {
    return this->next_;
  }

  //! Token next_ mutator method
  void Token::set_next_(Token* next) {
    this->next_ = next;
  }

  // Scanner constructor
  //! Scanner() constructor; calls to InitRegexTokenArray to initalize array
  Scanner::Scanner() {
    InitRegexTokenArray();
    // initalize the array
  }

  // Scanner deconstructor
  //! Scanner() deconstructor
  Scanner::~Scanner() {
  }

  /*! InitRegexTokenArray creates an array of regular expressions that matches
      to the token types, and the array is indexed based on the enum
      kTokenEnumType variable names.
  */
  void Scanner::InitRegexTokenArray() {
    // initalize whitespace and comment regexes
    white_space = make_regex("^[\n\t\r ]+");
    block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    single_comment = make_regex("^(\\/\\/)[^\n\r]*(\r|\n)+");

    regex_token_array[kIntKwd] = make_regex("^(I|i)nt");
    regex_token_array[kFloatKwd] = make_regex("^(F|f)loat");
    regex_token_array[kBoolKwd] = make_regex("^(bool)");
    regex_token_array[kTrueKwd] = make_regex("^(true)");
    regex_token_array[kFalseKwd] = make_regex("^(false)");
    regex_token_array[kStringKwd] = make_regex("^(S|s)tring");
    regex_token_array[kMatrixKwd] = make_regex("^(M|m)atrix");
    regex_token_array[kLetKwd] = make_regex("^(let)");
    regex_token_array[kInKwd] = make_regex("^(in)");
    regex_token_array[kEndKwd] = make_regex("^(end)");
    regex_token_array[kIfKwd] = make_regex("^(if)");
    regex_token_array[kThenKwd] = make_regex("^(then)");
    regex_token_array[kElseKwd] = make_regex("^(else)");
    regex_token_array[kRepeatKwd] = make_regex("^(repeat)");
    regex_token_array[kWhileKwd] = make_regex("^(while)");
    regex_token_array[kPrintKwd] = make_regex("^(print)");
    regex_token_array[kToKwd] = make_regex("^(to)");
    regex_token_array[kIntConst] = make_regex("^[0-9]+");
    regex_token_array[kFloatConst] = make_regex("^[0-9]*\\.[0-9]+");
    regex_token_array[kStringConst] = make_regex("^(\"([^\"]*)\")");
    regex_token_array[kVariableName] = make_regex("^[a-zA-Z_]+[a-zA-Z0-9_-]*");
    regex_token_array[kLeftParen] = make_regex("^(\\()");
    regex_token_array[kRightParen] = make_regex("^(\\))");
    regex_token_array[kLeftCurly] = make_regex("^(\\{)");
    regex_token_array[kRightCurly] = make_regex("^(\\})");
    regex_token_array[kLeftSquare] = make_regex("^(\\[)");
    regex_token_array[kRightSquare] = make_regex("^(\\])");
    regex_token_array[kSemiColon] = make_regex("^(;)");
    regex_token_array[kColon] = make_regex("^(:)");
    regex_token_array[kAssign] = make_regex("^(=)");
    regex_token_array[kPlusSign] = make_regex("^(\\+)");
    regex_token_array[kStar] = make_regex("^(\\*)");
    regex_token_array[kDash] = make_regex("^(\\-)");
    regex_token_array[kForwardSlash] = make_regex("^(\\/)");
    regex_token_array[kLessThan] = make_regex("^(<)");
    regex_token_array[kLessThanEqual] = make_regex("^(<=)");
    regex_token_array[kGreaterThan] = make_regex("^(>)");
    regex_token_array[kGreaterThanEqual] = make_regex("^(>=)");
    regex_token_array[kEqualsEquals] = make_regex("^(==)");
    regex_token_array[kNotEquals] = make_regex("^(!=)");
    regex_token_array[kAndOp] = make_regex("^(&&)");
    regex_token_array[kOrOp] = make_regex("^(\\|\\|)");
    regex_token_array[kNotOp] = make_regex("^(!)");
    regex_token_array[kLexicalError] = make_regex("()");
    regex_token_array[kEndOfFile] = make_regex("(EOF)");
    /* intializes the regex_token_array with regex expressions and places them in the enumerated index
    locations so if the enumerated values are switched around in the header file it will not cause an
    error in the code and allows us to just create an array with regex_t pointers.
    */
  }

  /*! The Scan method reads in a file or string, but if the file or string is
      determined to be NULL, than the Scan method will return NULL and
      terminate. If the Scan continues then it will check for an EOF character
      before continuing to scan in all the characters. As the Scan method scans
      the file, it will iterate through the InitRegexTokenArray to determine
      possible matches to the string; as it finds a given match, it stores the
      longest max_num_matched_chars and once it's done iterating through the
      string, it will return the token match_type. As it returns the match_type
      it will push the determined string of characters to an array list of
      tokens, which stores the token type, string and a pointer to the next
      token on an array. Once the scan reaches an EOF character, it will return
      the array list of tokens.
      \param *text is a string or file that is read by the scanner
  */
  Token *Scanner::Scan(const char *text) {
    if (text == NULL) {
      return NULL;
    }

      TokenType match_type;
      int num_matched_chars = 0;
      int max_num_matched_chars = 0;
      const int array_size = 45;

      // initalize singly linked list with null pointers
      Token* head_of_list = nullptr;
      Token* tail_of_list = nullptr;

      // consume whitespaces and comments
      num_matched_chars = consume_whitespace_and_comments(white_space,
        block_comment, single_comment, text);

      // offset by num_matched_chars found
      text = text + num_matched_chars;

      // begin loop
      while (text[0] != '\0') {
        // create new token to begin working with
        Token* current_token = new Token();
        max_num_matched_chars = 0;
        match_type = kLexicalError;

        // Refined code to match Token_Generator & because laziness happened
        for (int i = 0; i < array_size; i++) {
          num_matched_chars =
            match_regex(regex_token_array[i], text);
          if (num_matched_chars > max_num_matched_chars) {
            max_num_matched_chars = num_matched_chars;
            match_type = TokenType(i);
            /* modified this part of the code to use the regex_token_array and the matchtype to
            use TokenType instead of the old structure we had in the old iteration
            */
          }
        }  // for-loop

        if (match_type == kLexicalError) {
          // length_of_lexeme_ for single char
          current_token->length_of_lexeme_ = 1;
          // cast char* to string
          std::string casted_text = static_cast<std::string>(text);
          // set lexeme_found to single char ( )
          std::string lexeme_found =
            casted_text.substr(0, 1);
          // set lexeme_ to lexeme_found
          current_token->set_lexeme_(lexeme_found);
          if (tail_of_list == nullptr) {
            // init head to current_token
            head_of_list = current_token;
            // point head to tail
            head_of_list->set_next_(tail_of_list);
            // set tail to current_token
            tail_of_list = current_token;
            // might need tail?
          } else {
            tail_of_list->set_next_(current_token);
            tail_of_list = current_token;
          }
          text = text + 1;  // offset by 1 char (space)
        } else {
          current_token->length_of_lexeme_ = max_num_matched_chars;
          std::string casted_text = static_cast<std::string>(text);
          std::string lexeme_found =
            casted_text.substr(0, max_num_matched_chars);
          current_token->set_lexeme_(lexeme_found);
          current_token->set_terminal_(match_type);
          if (tail_of_list == nullptr) {
            head_of_list = current_token;
            head_of_list->set_next_(tail_of_list);
            tail_of_list = current_token;
          } else {
            tail_of_list->set_next_(current_token);
            tail_of_list = current_token;
          }
          text = text + max_num_matched_chars;  // offset by length_of_lexeme_
        }

        num_matched_chars = consume_whitespace_and_comments(white_space,
          block_comment, single_comment, text);

        text = text + num_matched_chars;
      }
        Token* end_token = new Token();
        end_token->length_of_lexeme_ = 3;
        end_token->set_lexeme_("EOF");
        end_token->set_terminal_(kEndOfFile);
        if (head_of_list == nullptr) {
          head_of_list = end_token;
        } else {
          tail_of_list->set_next_(end_token);
        }

        return head_of_list;
  }  // Scanner::*Scan

  /*! The consume_whitespace_and_comments method scans through a file or string
      and looks for comment lines, block comments and white spaces and removes
      them from consumption so that the Scan method can pass through the file
      without mistaking one of them for a token type.
  */
  int Scanner::consume_whitespace_and_comments(regex_t *white_space,
    regex_t *block_comment, regex_t *single_comment, const char *text) {
    int num_matched_chars = 0;
    int total_num_matched_chars = 0;
    int still_consuming_white_space;

    do {
      still_consuming_white_space = 0;  // exit loop if not reset by a match

      // Try to match white space
      num_matched_chars = match_regex(white_space, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }

      // Try to match block comments
      num_matched_chars = match_regex(block_comment, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }

      // Try to match single line comments
      num_matched_chars = match_regex(single_comment, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }
    } while (still_consuming_white_space);

    return total_num_matched_chars;
  }

}  // namespace scanner
}  // namespace fcal
