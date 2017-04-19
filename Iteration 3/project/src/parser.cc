/*******************************************************************************
 * Name            : parser.h
 * Project         : fcal
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Erik Van Wyk
 * Modifications by: Dan Challou, John Harwell
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/parser.h"
#include <assert.h>
#include <stdio.h>
#include "include/ext_token.h"
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
//! Parser deconstructor function
Parser::~Parser() {
  if (scanner_) delete scanner_;

  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /* while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->get_next_();
    delete to_delete2;
  } /* while() */
} /* Parser::~Parser() */
//! Parse constructor function
ParseResult Parser::Parse(const char *text) {
  assert(text != NULL);

  ParseResult pr;
  try {
    scanner_ = new scanner::Scanner();
    stokens_ = scanner_->Scan(text);
    tokens_ = tokens_->ExtendTokenList(this, stokens_);

    assert(tokens_ != NULL);
    curr_token_ = tokens_;
    pr = ParseProgram();
  }
  catch (std::string errMsg) {
    pr.ok(false);
    pr.errors(errMsg);
    pr.ast(NULL);
  }
  return pr;
} /* Parser::parse() */

/*
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

// Program
//! ParseProgram creates the first node in the AST, the Program Node
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  // root
  // Program ::= varName '(' ')' '{' Stmts '}'
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  // create VarName obj
  ast::VarName *varname = new ast::VarName(name);
  match(scanner::kLeftParen);
  match(scanner::kRightParen);
  match(scanner::kLeftCurly);
  ParseResult pr_stmts = parse_stmts();
  ast::Stmts *s = NULL;
  if (pr_stmts.ast()) {
    s = dynamic_cast<ast::Stmts *>(pr_stmts.ast());
    if (!s) throw ((std::string) "Bad cast of State in parseProgram");
  }
  match(scanner::kRightCurly);
  match(scanner::kEndOfFile);

  // create Program (Root) node
  pr.ast(new ast::Program(varname, s));
  return pr;
} /* Parser::ParseProgram() */

// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
/*! parse_matrix_decl parses a matrix declaration.
    If the second token is a left square bracker then it will parse according to the MatrixLongDecl,
    but there is not left square bracket then it will parse according to the regular MatrixDecl
*/
ParseResult Parser::parse_matrix_decl() {
  ParseResult pr;
  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  // create VarName obj
  ast::VarName *varname = new ast::VarName(name);

  // Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '='
  // Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
    ParseResult par_expr = parse_expr(0);
    ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kColon);
    ParseResult par_expr2 = parse_expr(0);
    ast::Expr *expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
    match(scanner::kRightSquare);
    ParseResult par_var = parse_variable_name();
    ast::VarName *var = dynamic_cast<ast::VarName*>(par_var.ast());
    match(scanner::kColon);
    ParseResult par_var2 = parse_variable_name();
    ast::VarName *var2 = dynamic_cast<ast::VarName*>(par_var2.ast());
    match(scanner::kAssign);
    ParseResult par_expr3 = parse_expr(0);
    ast::Expr *expr3 = dynamic_cast<ast::Expr*>(par_expr3.ast());
    pr.ast(new ast::MatrixLongDecl( varname, expr, expr2, var, var2, expr3));
  } else if (attempt_match(scanner::kAssign)) {
    // Decl ::= 'matrix' varName '=' Expr ';'
    ParseResult par_expr = parse_expr(0);
    ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    pr.ast(new ast::MatrixDecl(varname, expr));
  } else {
    throw((std::string) "Bad Syntax of Matrix Decl in in parseMatrixDecl");
  }

  match(scanner::kSemiColon);

  return pr;
}
// standardDecl
// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
/*! parse_standard_decl parses a type declaration. The decl_type will be passed to the general TypeDecl subclass
    and the decl_type will be placed in front of the varName ensuring correct parsing
*/
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;

  // Updated 03/31/17
  if (attempt_match(scanner::kIntKwd)) {
    // Type ::= intKwd
    std::string type(prev_token_->lexeme());
    ast::VarName *decl_type = new ast::VarName(type);
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::VarName *varname = new ast::VarName(name);
    pr.ast(new ast::TypeDecl(decl_type, varname));
    // pr.ast(new ast::IntDecl(new ast::VarName(name)));
  } else if (attempt_match(scanner::kFloatKwd)) {
    // Type ::= floatKwd
    std::string type(prev_token_->lexeme());
    ast::VarName *decl_type = new ast::VarName(type);
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::VarName *varname = new ast::VarName(name);
    pr.ast(new ast::TypeDecl(decl_type, varname));
    // pr.ast(new ast::FloatDecl(new ast::VarName(name)));
  } else if (attempt_match(scanner::kStringKwd)) {
    // Type ::= stringKwd
    std::string type(prev_token_->lexeme());
    ast::VarName *decl_type = new ast::VarName(type);
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::VarName *varname = new ast::VarName(name);
    pr.ast(new ast::TypeDecl(decl_type, varname));
    // pr.ast(new ast::StringDecl(new ast::VarName(name)));
  } else if (attempt_match(scanner::kBoolKwd)) {
    // Type ::= boolKwd
    std::string type(prev_token_->lexeme());
    ast::VarName *decl_type = new ast::VarName(type);
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::VarName *varname = new ast::VarName(name);
    pr.ast(new ast::TypeDecl(decl_type, varname));
    // pr.ast(new ast::BoolDecl(new ast::VarName(name)));
  }
  // match(scanner::kVariableName);
  match(scanner::kSemiColon);
  return pr;
}

// Decl
/*! parse_decl will categorize what type of declaration using the current Token types to either
    parse_matrix_decl or parse_standard_decl
*/
ParseResult Parser::parse_decl() {
  ParseResult pr;
  // Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {
    // Decl ::= Type variableName semiColon
    pr = parse_standard_decl();
  }
  return pr;
}

// Stmts
/*! parse_stmts will parse EmptyStmts if it is the last Node of the AST, but if the next Node in the AST
    is neither a kRightCurly or a kInKwd then it will continue parsing with SeqStmts
*/
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    // Stmts ::= Stmt Stmts
    ParseResult par_stmt = parse_stmt();
    ast::Stmt *stmt = dynamic_cast<ast::Stmt *>(par_stmt.ast());
    ParseResult par_stmts = parse_stmts();
    ast::Stmts *stmts = dynamic_cast<ast::Stmts *>(par_stmts.ast());
    pr.ast(new ast::SeqStmts(stmt, stmts));
  } else {
    // Stmts ::=
    // nothing to match.k
  pr.ast(new ast::EmptyStmts());
  }
  return pr;
}

// Stmt
/*! parse_stmt will categorize the type of statement by identifying the keyword and will create
    the according subclass for it.
	  If the current token is a keyword associated with declarations; kIntKwd, kFloatKwd, etc. it will
	  create a StmtDecl subclass.
    If the current token is the keyword kLeftCurly then a BlockStmt subclass will be created
	  If the current token is the keyword kIfKwd then a IfStmt subclass will be created, but if there is
	  a token after that is kElseKwd then the subclass IfElseStmt will created instead
	  If the current token is the keyword kVariableName and the next token is of type kLeftSquare then a
	  AssignLongStmt subclass will be created, but if then tokens are just kVariableName and kAssign
    then an AssignStmt will be created
    If the current token is the keyword kPrintKwd then a PrintStmt will be created
	  If the current token is the keyword kRepeatKwd then a RepeatStmt will be created
	  If the current token is the keyword kWhileKwd then a WhileStmt will be created
	  If the current token is the keyword kSemiColon then an EndStmt will be created
    If there is current token then throw an error message
*/
ParseResult Parser::parse_stmt() {
  ParseResult pr;

  // Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
      ParseResult par_decl = parse_decl();
      ast::Decl *decl = dynamic_cast<ast::Decl *>(par_decl.ast());
      pr.ast(new ast::StmtDecl(decl));
      // pr = parse_decl();
  } else if (attempt_match(scanner::kLeftCurly)) {
    // Stmt ::= '{' Stmts '}'
    ParseResult par_stmts = parse_stmts();
    ast::Stmts* stmts = dynamic_cast<ast::Stmts *>(par_stmts.ast());
    match(scanner::kRightCurly);
    pr.ast(new ast::BlockStmt(stmts));
  } else if (attempt_match(scanner::kIfKwd)) {
    // Stmt ::= 'if' '(' Expr ')' Stmt
    // Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    match(scanner::kLeftParen);
    ParseResult par_Expr = parse_expr(0);
    ast::Expr* expr = dynamic_cast<ast::Expr*>(par_Expr.ast());
    match(scanner::kRightParen);
    ParseResult par_stmt = parse_stmt();
    ast::Stmt* stmt = dynamic_cast<ast::Stmt*>(par_stmt.ast());
    pr.ast(new ast::IfStmt(expr, stmt));

    if (attempt_match(scanner::kElseKwd)) {
    ParseResult par_stmt2 = parse_stmt();
    ast::Stmt* stmt2 = dynamic_cast<ast::Stmt*>(par_stmt2.ast());
    pr.ast(new ast::IfElseStmt(expr, stmt, stmt2));
    }
  } else if (attempt_match(scanner::kVariableName)) {
    /*
     * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
     * '=' Expr ';'
     */
  std::string name(prev_token_->lexeme());
  ast::VarName *var = new ast::VarName(name);
  // might not need these new VarName
    if (attempt_match(scanner::kLeftSquare)) {
      ParseResult par_expr = parse_expr(0);
      ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
      match(scanner::kColon);
      ParseResult par_expr2 = parse_expr(0);
      ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
      match(scanner::kRightSquare);
      match(scanner::kAssign);
      ParseResult par_expr3 = parse_expr(0);
      ast::Expr* expr3 = dynamic_cast<ast::Expr*>(par_expr3.ast());
      pr.ast(new ast::AssignLongStmt(var, expr, expr2, expr3));
      match(scanner::kSemiColon);
    // modified this part to include an if and else because if you don't
      // there will be no spot where short assign is placed
    } else {
  match(scanner::kAssign);
    ParseResult par_expr = parse_expr(0);
    ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    pr.ast(new ast::AssignStmt(var, expr));
    match(scanner::kSemiColon);
  }
    } else if (attempt_match(scanner::kPrintKwd)) {
    // Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);
    ParseResult par_expr = parse_expr(0);
    ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kRightParen);
    match(scanner::kSemiColon);
    pr.ast(new ast::PrintStmt(expr));
  } else if (attempt_match(scanner::kRepeatKwd)) {
    // Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    match(scanner::kLeftParen);
    match(scanner::kVariableName);
    std::string name(prev_token_->lexeme());
    ast::VarName *var = new ast::VarName(name);
    match(scanner::kAssign);
    ParseResult par_expr = parse_expr(0);
    ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kToKwd);
    ParseResult par_expr2 = parse_expr(0);
    ast::Expr *expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
    match(scanner::kRightParen);
    ParseResult par_stmt = parse_stmt();
    ast::Stmt *stmt = dynamic_cast<ast::Stmt*>(par_stmt.ast());
    pr.ast(new ast::RepeatStmt(var, expr, expr2, stmt));
  } else if (attempt_match(scanner::kWhileKwd)) {
    // Stmt ::= 'while' '(' Expr ')' Stmt
    match(scanner::kLeftParen);
    ParseResult par_expr = parse_expr(0);
    ast::Expr *expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kRightParen);
    ParseResult par_stmt = parse_stmt();
    ast::Stmt *stmt = dynamic_cast<ast::Stmt*>(par_stmt.ast());
    pr.ast(new ast::WhileStmt(expr, stmt));
  } else if (attempt_match(scanner::kSemiColon)) {
    // Stmt ::= ';
    // parsed a skip
    pr.ast(new ast::EndStmt());
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon

  return pr;
}

// Expr
ParseResult Parser::parse_expr(int rbp) {
  /*! Examine current token, without consuming it, to call its
     associated parse methods.  The ExtToken objects have 'nud' and
     'led' methods that are dispatchers that call the appropriate
     parse methods.*/
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
/*! parse_true_kwd identifies the current node's Token type and if it is kTrueKwd then it
    generates a BoolTrue subclass
*/
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);
  pr.ast(new ast::BoolTrue());
  return pr;
}

// Expr ::= falseKwd
/*! parser_false_kwd idenitifies the current node's Token type and if it is kFalseKwd then it
	  generates a BoolFalse subclass
*/
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);
  pr.ast(new ast::BoolFalse());
  return pr;
}

// Expr ::= intConst
/*! parse_int_const identifies the current node's Token type and if it is kIntConst then it
    generates a TypeConst subclass and passes in a "int" lexeme with it
*/
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);
  std::string name(prev_token_->lexeme());
  pr.ast(new ast::TypeConst(name));
  return pr;
}

// Expr ::= floatConst
/*! parse_float_const identifies the current node's Token type and if it is kFloatConst then it
    generates a TypeConst subclass and passes in a "float" lexeme with it
*/
ParseResult Parser::parse_float_const() {
  ParseResult pr;
  match(scanner::kFloatConst);
  std::string name(prev_token_->lexeme());
  pr.ast(new ast::TypeConst(name));
  return pr;
}

// Expr ::= stringConst
/*! parse_string_const identifies the current node's Token type and if it is kStringConst then it
      generates a TypeConst subclass and passes in a "string" lexeme with it
*/
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);
  std::string name(prev_token_->lexeme());
  pr.ast(new ast::TypeConst(name));
  return pr;
}

// Expr ::= variableName .....
/*! parse_variable_name identifies the current token's type and creates a subclass according to it
    If the current token is the keyword kLeftSquare then a MatrixRef subclass will be created
	  If the current token is the keyword kLeftParen then a NestedOrFuncCall will be created
	  Else if the current token matches none of these then it creates a VarName subclass
*/
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  match(scanner::kVariableName);
  std::string name(prev_token_->lexeme());
  // create VarName obj
  ast::VarName *varname = new ast::VarName(name);
  if (attempt_match(scanner::kLeftSquare)) {
    ParseResult par_expr = parse_expr(0);
    ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kColon);
    ParseResult par_expr2 = parse_expr(0);
    ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
    match(scanner::kRightSquare);
    pr.ast(new ast::MatrixRef(varname, expr, expr2));
  }  else if (attempt_match(scanner::kLeftParen)) {
    // Expr ::= varableName '(' Expr ')'
    ParseResult par_expr = parse_expr(0);
    ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
    match(scanner::kRightParen);
    pr.ast(new ast::NestedOrFuncCall(varname, expr));
  } else {
    // variable
    pr.ast(new ast::VarName(name));
  }
  return pr;
}

// Expr ::= leftParen Expr rightParen
/*! parse_nested_expr will generate a ParenExpr subclass with parameter expr
*/
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  match(scanner::kLeftParen);
  ParseResult par_expr = parse_expr(0);
  ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
  match(scanner::kRightParen);
  pr.ast(new ast::ParenExpr(expr));
  return pr;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr
/*! parse_if_expr will generate an IfExpr subclass with parameters expr, expr2, and expr3
*/
ParseResult Parser::parse_if_expr() {
  ParseResult pr;

  match(scanner::kIfKwd);
  ParseResult par_expr = parse_expr(0);
  ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
  match(scanner::kThenKwd);
  ParseResult par_expr2 = parse_expr(0);
  ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  match(scanner::kElseKwd);
  ParseResult par_expr3 = parse_expr(0);
  ast::Expr* expr3 = dynamic_cast<ast::Expr*>(par_expr3.ast());
  pr.ast(new ast::IfExpr(expr, expr2, expr3));
  return pr;
}

// Expr ::= 'let' Stmts 'in' Expr 'end'
/*! parse_let_expr will generate a LetExpr with parameters stmts and expr
*/
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  match(scanner::kLetKwd);
  ParseResult par_stmts = parse_stmts();
  ast::Stmts* stmts = dynamic_cast<ast::Stmts *>(par_stmts.ast());
  match(scanner::kInKwd);
  ParseResult par_expr = parse_expr(0);
  ast::Expr* expr = dynamic_cast<ast::Expr *>(par_expr.ast());
  match(scanner::kEndKwd);
  pr.ast(new ast::LetExpr(stmts, expr));
  return pr;
}

// Expr ::= '!' Expr
/*! parse_not_expr will generate a NotExpr with parameter expr
*/
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  match(scanner::kNotOp);
  ParseResult par_expr = parse_expr(0);
  ast::Expr* expr = dynamic_cast<ast::Expr*>(par_expr.ast());
  pr.ast(new ast::NotExpr(expr));
  return pr;
}
// Expr ::= Expr plusSign Expr
/*! parse_addition will generate a BinaryOp with parameters expr, "+", and expr2
*/
ParseResult Parser::parse_addition(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  match(scanner::kPlusSign);
  std::string *op = new std::string(prev_token_->lexeme());
  // parse_expr(prev_token_->lbp());
  ParseResult par_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  pr.ast(new ast::BinaryOp(expr, op, expr2));
  return pr;
}

// Expr ::= Expr star Expr
/*! parse_multiplication will generate a BinaryOp with parameters expr, "*", and expr2
*/
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  match(scanner::kStar);
  std::string *op = new std::string(prev_token_->lexeme());
  // parse_expr(prev_token_->lbp());
  ParseResult par_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  pr.ast(new ast::BinaryOp(expr, op, expr2));
  return pr;
}

// Expr ::= Expr dash Expr
/*! parse_subtraction will generate a BinaryOp with parameters expr, "-", and expr2
*/
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  match(scanner::kDash);
  std::string *op = new std::string(prev_token_->lexeme());
  // parse_expr(prev_token_->lbp());
  ParseResult par_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  pr.ast(new ast::BinaryOp(expr, op, expr2));
  return pr;
}

// Expr ::= Expr forwardSlash Expr
/*! parse_division will generate a BinaryOp with parameters expr, "/", and expr2
*/
ParseResult Parser::parse_division(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  match(scanner::kForwardSlash);
  std::string *op = new std::string(prev_token_->lexeme());
  // parse_expr(prev_token_->lbp());
  ParseResult par_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr* expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  pr.ast(new ast::BinaryOp(expr, op, expr2));
  return pr;
}

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
*/
/*! parse_relational_expr will generate a BinaryOp with expr, whichever relational expression, and expr2
*/
ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  // parser has already matchekD left expression
  ParseResult pr;
  ast::Expr *expr = dynamic_cast<ast::Expr*>(prLeft.ast());
  next_token();
  // just advance token, since examining it in parse_expr caused
  // this method being called.
  std::string *op = new std::string(prev_token_->lexeme());
  // parse_expr(prev_token_->lbp());
  ParseResult par_expr2 = parse_expr(prev_token_->lbp());
  ast::Expr *expr2 = dynamic_cast<ast::Expr*>(par_expr2.ast());
  pr.ast(new ast::BinaryOp(expr, op, expr2));
  return pr;
}

// Helper function used by the parser.

void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}

bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}

bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}

void Parser::next_token() {
  if (curr_token_ == NULL) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == NULL) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == NULL) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token(terminal, "", NULL);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  std::string s =
    dummy_ext_token->ExtendToken(this, dummy_token)->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

std::string Parser::make_error_msg_expected(
  const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}

std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

std::string Parser::make_error_msg(const char *msg) { return msg; }

} /* namespace parser */
} /* namespace fcal */
