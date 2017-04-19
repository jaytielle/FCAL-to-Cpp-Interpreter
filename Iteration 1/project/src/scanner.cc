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
  TokenType Token::get_terminal_() {
    return this->terminal_;
  }

  void Token::set_terminal_(TokenType terminal) {
    this->terminal_ = terminal;
  }

  std::string Token::get_lexeme_() {
    return this->lexeme_;
  }

  void Token::set_lexeme_(std::string lexeme) {
    this->lexeme_ = lexeme;
  }

  Token* Token::get_next_() {
    return this->next_;
  }

  void Token::set_next_(Token* next) {
    this->next_ = next;
  }

  // Scanner constructor
  Scanner::Scanner() {
  }

  // Scanner deconstructor
  Scanner::~Scanner() {
  }

  Token* Scanner::Token_Generator(const char *text) {
    if (text == NULL) {
      return NULL;
    }

    // initalize whitespace and comment regexes
    regex_t *white_space = make_regex("^[\n\t\r ]+");
    regex_t *block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    regex_t *single_comment = make_regex("^\\/\\/+");

    // initalize regexes for everything else
    regex_t *int_keyword = make_regex("^(I|i)nt");
    regex_t *float_keyword = make_regex("^(F|f)loat");
    regex_t *string_keyword = make_regex("^(S|s)tring");
    regex_t *matrix_keyword = make_regex("^(M|m)atrix");
    regex_t *let_keyword = make_regex("^(let)");
    regex_t *in_keyword = make_regex("^(in)");
    regex_t *end_keyword = make_regex("^(end)");
    regex_t *if_keyword = make_regex("^(if)");
    regex_t *then_keyword = make_regex("^(then)");
    regex_t *else_keyword = make_regex("^(else)");
    regex_t *repeat_keyword = make_regex("^(repeat)");
    regex_t *print_keyword = make_regex("^(print)");
    regex_t *to_keyword = make_regex("^(to)");
    regex_t *int_constant = make_regex("^-?[0-9]+");
    regex_t *float_constant =  make_regex("^-?[0-9]*\\.[0-9]+");
    regex_t *string_constant = make_regex("^(\"([^\"]*)\")");
    regex_t *variable_name = make_regex("^[a-zA-Z_]+[a-zA-Z0-9_-]*");
    regex_t *left_parenthesis = make_regex("^(\\()");
    regex_t *right_parenthesis = make_regex("^(\\))");
    regex_t *left_curly_bracket = make_regex("^(\\{)");
    regex_t *right_curly_bracket = make_regex("^(\\})");
    regex_t *left_square_bracket = make_regex("^(\\[)");
    regex_t *right_square_bracket = make_regex("^(\\])");
    regex_t *semi_colon = make_regex("^(;)");
    regex_t *colon = make_regex("^(:)");
    regex_t *assignment_operator = make_regex("^(=)");
    regex_t *plus_sign = make_regex("^(\\+)");
    regex_t *asterisk_sign = make_regex("^(\\*)");
    regex_t *minus_sign = make_regex("^(\\-)");
    regex_t *foward_slash = make_regex("^(\\/)");
    regex_t *less_than = make_regex("^(<)");
    regex_t *less_than_equal = make_regex("^(<=)");
    regex_t *greater_than = make_regex("^(>)");
    regex_t *greater_than_equal = make_regex("^(>=)");
    regex_t *equal_equal = make_regex("^(==)");
    regex_t *not_equal = make_regex("^(!=)");
    regex_t *and_operator = make_regex("^(&&)");
    regex_t *or_operator = make_regex("^(\\|\\|)");
    regex_t *not_operator = make_regex("^(!)");
    regex_t *lexical_error = make_regex("()");
    regex_t *end_of_file = make_regex("(EOF)");

    // struct set of regex_str, regex_made pointer, and token_type
    struct make_str_regex_token_set str_regex_token_array[41]  = {
      {"^(I|i)nt", int_keyword, kIntKwd},
      {"^(F|f)loat", float_keyword, kFloatKwd },
      {"^(S|s)tring", string_keyword, kStringKwd},
      {"^(M|m)atrix", matrix_keyword, kMatrixKwd},
      {"^(let)", let_keyword, kLetKwd},
      {"^(in)", in_keyword, kInKwd},
      {"^(end)", end_keyword, kEndKwd},
      {"^(if)", if_keyword, kIfKwd},
      {"^(then)", then_keyword, kThenKwd},
      {"^(else)", else_keyword, kElseKwd},
      {"^(repeat)", repeat_keyword, kRepeatKwd},
      {"^(print)", print_keyword, kPrintKwd},
      {"^(to)", to_keyword, kToKwd},
      {"^-?[0-9]+", int_constant, kIntConst},
      {"^-?[0-9]+\\.[0-9]+", float_constant, kFloatConst},
      {"^(\"([^\"]*)\") ", string_constant, kStringConst},
      {"^[a-zA-Z_]+[a-zA-Z0-9_-]+", variable_name, kVariableName},
      {"^(\\()", left_parenthesis, kLeftParen},
      {"^(\\))", right_parenthesis, kRightParen},
      {"^(\\{)", left_curly_bracket, kLeftCurly},
      {"^(\\})", right_curly_bracket, kRightCurly},
      {"^(\\[)", left_square_bracket, kLeftSquare},
      {"^(\\])", right_square_bracket, kRightSquare},
      {"^(;)", semi_colon, kSemiColon},
      {"^(:)", colon, kColon},
      {"^(=)", assignment_operator, kAssign},
      {"^(\\+)", plus_sign, kPlusSign},
      {"^(\\*)", asterisk_sign, kStar},
      {"^(\\-)", minus_sign, kDash},
      {"^(\\/)", foward_slash, kForwardSlash},
      {"^(<)", less_than, kLessThan},
      {"^(<=)", less_than_equal, kLessThanEqual},
      {"^(>)", greater_than, kGreaterThan},
      {"^(>=)", greater_than_equal, kGreaterThanEqual},
      {"^(==)", equal_equal, kEqualsEquals},
      {"^(!=)", not_equal, kNotEquals},
      {"^(&&)", and_operator, kAndOp},
      {"^(\\|\\|)", or_operator, kOrOp},
      {"^(!)", not_operator, kNotOp},
      {"()", lexical_error, kLexicalError},
      {"(EOF)", end_of_file, kEndOfFile}
    };

    TokenType match_type = kLexicalError;
    int num_matched_chars = 0;
    int max_num_matched_chars = 0;
    int array_size_minus_two = 41;

    // init new token to work with
    Token* new_token = new Token();

    // consume whitespaces and comments
    num_matched_chars = consume_whitespace_and_comments(white_space,
      block_comment, single_comment, text);

    // offset by num_matched_chars found
    text = text + num_matched_chars;

    // begin loop
    for (int i = 0; i < array_size_minus_two; i++) {
      num_matched_chars =
        match_regex(str_regex_token_array[i].regex_made, text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        match_type = str_regex_token_array[i].token_type;
      }
    }  // for-loop
    if (match_type != kLexicalError) {
      std::string casted_text = static_cast<std::string>(text);
      std::string lexeme_found =
        casted_text.substr(0, max_num_matched_chars);
      new_token->set_lexeme_(lexeme_found);
      new_token->set_terminal_(match_type);
      } else {
      std::string casted_text = static_cast<std::string>(text);
      std::string lexeme_found =
        casted_text.substr(0, 1);
      new_token->set_lexeme_(lexeme_found);
    }
    return new_token;
  }  // Token_Generator

  bool Scanner::token_type_tester(const char *text,
    const char* lexeme, TokenType terminal) {
    Token* generated_token = Token_Generator(text);
    if (generated_token == NULL) {
      return false;
    } else if (generated_token->get_terminal_() != terminal ||
      generated_token->get_lexeme_() !=  lexeme) {
      return false;
    } else {
      return true;
    }
  }  // token_type_tester

  Token* Scanner::EOF_Token_Generator(const char *text) {
    regex_t *white_space = make_regex("^[\n\t\r ]+");
    regex_t *block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    regex_t *single_comment = make_regex("^\\/\\/+");

    TokenType match_type = kLexicalError;
    int num_matched_chars = 0;

    // init new token to work with
    Token* end_token = new Token();

    // consume whitespaces and comments
    num_matched_chars = consume_whitespace_and_comments(white_space,
      block_comment, single_comment, text);

    // offset by num_matched_chars found
    text = text + num_matched_chars;

    match_type = kEndOfFile;
    end_token->set_lexeme_("EOF");
    end_token->set_terminal_(match_type);

    return end_token;
  }  // EOF_Token_Generator

  bool Scanner::end_token_tester(const char *text,
    const char* lexeme, TokenType terminal) {
    Token* generated_token = EOF_Token_Generator(text);
    if (generated_token == NULL) {
      return false;
    } else if (generated_token->get_terminal_() != terminal ||
      generated_token->get_lexeme_() !=  lexeme) {
      return false;
    } else {
      return true;
    }
  }

  Token *Scanner::Scan(const char *text) {
    if (text == NULL) {
      return NULL;
    }

    // initalize whitespace and comment regexes
    regex_t *white_space = make_regex("^[\n\t\r ]+");
    regex_t *block_comment = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    regex_t *single_comment = make_regex("^\\/\\/+");

    // initalize regexes for everything else
    regex_t *int_keyword = make_regex("^(I|i)nt");
    regex_t *float_keyword = make_regex("^(F|f)loat");
    regex_t *string_keyword = make_regex("^(S|s)tring");
    regex_t *matrix_keyword = make_regex("^(M|m)atrix");
    regex_t *let_keyword = make_regex("^(let)");
    regex_t *in_keyword = make_regex("^(in)");
    regex_t *end_keyword = make_regex("^(end)");
    regex_t *if_keyword = make_regex("^(if)");
    regex_t *then_keyword = make_regex("^(then)");
    regex_t *else_keyword = make_regex("^(else)");
    regex_t *repeat_keyword = make_regex("^(repeat)");
    regex_t *print_keyword = make_regex("^(print)");
    regex_t *to_keyword = make_regex("^(to)");
    regex_t *int_constant = make_regex("^-?[0-9]+");
    regex_t *float_constant =  make_regex("^-?[0-9]*\\.[0-9]+");
    regex_t *string_constant = make_regex("^(\"([^\"]*)\")");
    regex_t *variable_name = make_regex("^[a-zA-Z_]+[a-zA-Z0-9_-]*");
    regex_t *left_parenthesis = make_regex("^(\\()");
    regex_t *right_parenthesis = make_regex("^(\\))");
    regex_t *left_curly_bracket = make_regex("^(\\{)");
    regex_t *right_curly_bracket = make_regex("^(\\})");
    regex_t *left_square_bracket = make_regex("^(\\[)");
    regex_t *right_square_bracket = make_regex("^(\\])");
    regex_t *semi_colon = make_regex("^(;)");
    regex_t *colon = make_regex("^(:)");
    regex_t *assignment_operator = make_regex("^(=)");
    regex_t *plus_sign = make_regex("^(\\+)");
    regex_t *asterisk_sign = make_regex("^(\\*)");
    regex_t *minus_sign = make_regex("^(\\-)");
    regex_t *foward_slash = make_regex("^(\\/)");
    regex_t *less_than = make_regex("^(<)");
    regex_t *less_than_equal = make_regex("^(<=)");
    regex_t *greater_than = make_regex("^(>)");
    regex_t *greater_than_equal = make_regex("^(>=)");
    regex_t *equal_equal = make_regex("^(==)");
    regex_t *not_equal = make_regex("^(!=)");
    regex_t *and_operator = make_regex("^(&&)");
    regex_t *or_operator = make_regex("^(\\|\\|)");
    regex_t *not_operator = make_regex("^(!)");
    regex_t *lexical_error = make_regex("()");
    regex_t *end_of_file = make_regex("(EOF)");

    // struct set of regex_str, regex_made pointer, and token_type
    struct make_str_regex_token_set str_regex_token_array[41]  = {
      {"^(I|i)nt", int_keyword, kIntKwd},
      {"^(F|f)loat", float_keyword, kFloatKwd },
      {"^(S|s)tring", string_keyword, kStringKwd},
      {"^(M|m)atrix", matrix_keyword, kMatrixKwd},
      {"^(let)", let_keyword, kLetKwd},
      {"^(in)", in_keyword, kInKwd},
      {"^(end)", end_keyword, kEndKwd},
      {"^(if)", if_keyword, kIfKwd},
      {"^(then)", then_keyword, kThenKwd},
      {"^(else)", else_keyword, kElseKwd},
      {"^(repeat)", repeat_keyword, kRepeatKwd},
      {"^(print)", print_keyword, kPrintKwd},
      {"^(to)", to_keyword, kToKwd},
      {"^-?[0-9]+", int_constant, kIntConst},
      {"^-?[0-9]+\\.[0-9]+", float_constant, kFloatConst},
      {"^(\"([^\"]*)\") ", string_constant, kStringConst},
      {"^[a-zA-Z_]+[a-zA-Z0-9_-]+", variable_name, kVariableName},
      {"^(\\()", left_parenthesis, kLeftParen},
      {"^(\\))", right_parenthesis, kRightParen},
      {"^(\\{)", left_curly_bracket, kLeftCurly},
      {"^(\\})", right_curly_bracket, kRightCurly},
      {"^(\\[)", left_square_bracket, kLeftSquare},
      {"^(\\])", right_square_bracket, kRightSquare},
      {"^(;)", semi_colon, kSemiColon},
      {"^(:)", colon, kColon},
      {"^(=)", assignment_operator, kAssign},
      {"^(\\+)", plus_sign, kPlusSign},
      {"^(\\*)", asterisk_sign, kStar},
      {"^(\\-)", minus_sign, kDash},
      {"^(\\/)", foward_slash, kForwardSlash},
      {"^(<)", less_than, kLessThan},
      {"^(<=)", less_than_equal, kLessThanEqual},
      {"^(>)", greater_than, kGreaterThan},
      {"^(>=)", greater_than_equal, kGreaterThanEqual},
      {"^(==)", equal_equal, kEqualsEquals},
      {"^(!=)", not_equal, kNotEquals},
      {"^(&&)", and_operator, kAndOp},
      {"^(\\|\\|)", or_operator, kOrOp},
      {"^(!)", not_operator, kNotOp},
      {"()", lexical_error, kLexicalError},
      {"(EOF)", end_of_file, kEndOfFile}
    };

      TokenType match_type;
      int num_matched_chars = 0;
      int max_num_matched_chars = 0;

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

        num_matched_chars =
          match_regex(str_regex_token_array[0].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[0].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[1].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[1].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[2].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[2].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[3].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[3].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[4].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[4].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[5].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[5].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[6].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[6].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[7].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[7].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[8].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[8].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[9].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[9].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[10].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[10].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[11].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[11].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[12].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[12].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[13].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[13].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[14].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[14].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[15].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[15].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[16].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[16].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[17].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[17].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[18].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[18].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[19].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[19].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[20].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[20].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[21].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[21].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[22].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[22].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[23].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[23].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[24].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[24].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[25].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[25].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[26].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[26].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[27].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[27].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[28].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[28].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[29].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[29].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[30].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[30].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[31].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[31].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[32].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[32].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[33].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[33].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[34].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[34].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[35].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[35].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[36].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[36].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[37].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[37].token_type;
        }

        num_matched_chars =
          match_regex(str_regex_token_array[38].regex_made, text);
        if (num_matched_chars > max_num_matched_chars) {
          max_num_matched_chars = num_matched_chars;
          match_type = str_regex_token_array[38].token_type;
        }

        // num_matched_chars =
        //   match_regex(str_regex_token_array[2].regex_made, text);
        // if (num_matched_chars > max_num_matched_chars) {
        //   max_num_matched_chars = num_matched_chars;
        //   match_type = kLexicalError;
        // }
        //
        // num_matched_chars =
        //   match_regex(str_regex_token_array[2].regex_made, text);
        // if (num_matched_chars > max_num_matched_chars) {
        //   max_num_matched_chars = num_matched_chars;
        //   match_type = kEndOfFile;
        // }


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
