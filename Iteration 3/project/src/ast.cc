/*******************************************************************************
 * Name            : ext_token.h
 * Project         : fcal
 * Module          : ast
 * Copyright       : 2017 CSCI 3081W. All rights reserved.
 * Original Author : Jennifer Lieu, Tommy Her
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/ast.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// Root/Program
// ----------------------------------------------------------------------------
//! Program unparse() returns the var_name_ and stmts_ parameters
std::string Program::unparse() {
  std::cout << "\nunparsing Program (Root) ..." << std::endl;

  return var_name_->unparse() + "() {\n" + stmts_->unparse() + "\n}\n";
}

//! Program deconstructor method
Program::~Program() {
  std::cout << "\ndestructoring Program (Root) ..." << std::endl;
}

// VarName
// ----------------------------------------------------------------------------
//! VarName unparse() returns the lexeme_ parameter
std::string VarName::unparse() {
  std::cout << "unparsing VarName ..." << std::endl;
  return lexeme_;
}

// Stmts
// ----------------------------------------------------------------------------
//! SeqStmts unparse() returns the stmt_ and stmts_ parameters
std::string SeqStmts::unparse() {
  std::cout << "unparsing SeqStmts ..." << std::endl;
  return stmt_->unparse() + stmts_->unparse();
}

//! EmptyStmts unparse() returns nothing
std::string EmptyStmts::unparse() {
  std::cout << "unparsing EmptyStmts ..." << std::endl;
  return "";
}
// Stmt
// ----------------------------------------------------------------------------
//! StmtDecl unparse() returns the decl_ parameter
std::string StmtDecl::unparse() {
  std::cout << "unparsing StmtDecl ..." << std::endl;
  return decl_->unparse();
}

//! BlockStmt unparse() returns the stmts_ parameter
std::string BlockStmt::unparse() {
  std::cout << "unparsing BlockStmt ..." << std::endl;
  return "{" + stmts_->unparse() + "}";
}

//! IfStmt unparse() returns the expr_ and stmt_ parameters
std::string IfStmt::unparse() {
  std::cout << "unparsing IfStmt ..." << std::endl;
  return "if (" + expr_->unparse() + ")" + stmt_->unparse() + "\n";
}

//! IfElseStmt unparse() returns the expr_, stmt_ and stmt2_ parameters
std::string IfElseStmt::unparse() {
  std::cout << "unparsing IfElseStmt ..." << std::endl;
  return "if (" + expr_->unparse() + ")" + stmt_->unparse() + "}\n else" +
    stmt2_->unparse() + "\n";
}

//! PrintStmt unparse() returns the expr_ parameter
std::string PrintStmt::unparse() {
  std::cout << "unparsing PrintStmt ..." << std::endl;
  return "print(" + expr_->unparse() + "); \n";
}

//! AssignStmt unparse() returns var_name_, expr_
std::string AssignStmt::unparse() {
  std::cout << "unparsing AssignStmt ..." << std::endl;
  return var_name_->unparse() + " = " + expr_->unparse() + ";\n";
}

//! AssignLongStmt unparse() returns var_name_, expr_, expr2_ and expr3_
std::string AssignLongStmt::unparse() {
  std::cout<< "unparsing AssignLongStmt ..." << std::endl;
  return var_name_->unparse() + " [ " + expr_->unparse() + " : " +
    expr2_->unparse() + " ] =" + expr3_->unparse() + ";\n";
}

//! RepeatStmt returns var_name_, expr_, expr2_ and stmt_
std::string RepeatStmt::unparse() {
  std::cout<< "unparsing RepeatStmt ..." << std::endl;
  return "repeat (" + var_name_->unparse() + "=" + expr_->unparse() + " to " +
    expr2_->unparse() + " )" + stmt_->unparse();
}

//! WhileStmt returns expr_ and stmt_ parameters
std::string WhileStmt::unparse() {
  std::cout << "unparsing WhileStmt ..." << std::endl;
  return "while(" + expr_->unparse() + ")" + stmt_->unparse() + "\n";
}

//! EndStmt returns a semicolon (;) for end of line
std::string EndStmt::unparse() {
  std::cout<< "unparsing EndStmt ..." << std::endl;
  return ";";
}

// Decl
// ----------------------------------------------------------------------------
//! TypeDecl returns the type_ and var_name_ of a declaration
std::string TypeDecl::unparse() {
  std::cout << "unparsing TypeDecl ..." << std::endl;
  return type_->unparse() + " " + var_name_->unparse() + ";\n";
}

/*! MatrixLongDecl returns var_name_, expr_, expr2_, var_name2_, var_name3_, and
    expr3_
*/
std::string MatrixLongDecl::unparse() {
  std::cout << "unparsing MatrixLongDecl ..." << std::endl;
  return "matrix " + var_name_->unparse() + " [ " + expr_->unparse() + " : " +
    expr2_->unparse() + " ] " + var_name2_->unparse() + " : " +
    var_name3_->unparse() + " = " + expr3_->unparse() + ";\n";
}

//! MatrixDecl returns var_name_ and expr_
std::string MatrixDecl::unparse() {
  std::cout << "unparsing MatrixDecl ..." << std::endl;
  return "matrix " + var_name_->unparse() + " = " + expr_->unparse() + ";\n";
}

// Expr
// ----------------------------------------------------------------------------
//! TypeConst returns the type_const of a data type
std::string TypeConst::unparse() {
  std::cout << "unparsing TypeConst ..." << std::endl;
  return type_const_;
}

//! BoolTrue returns the "True" string for a boolean truth
std::string BoolTrue::unparse() {
  std::cout << "unparsing BoolTrue ..." << std::endl;
  return "True";
}

//! BoolFalse returns a "False" string for a boolean false
std::string BoolFalse::unparse() {
  std::cout << "unparsing BoolFalse ..." << std::endl;
  return "False";
}

//! BinaryOp returns the expr_, op_ and expr2_
std::string BinaryOp::unparse() {
  std::cout << "unparsing BinaryOp ..." << std::endl;
  return expr_->unparse() + " " + *op_ + " " + expr2_->unparse();
}

//! MatrixRef returns the var_name_, expr_ and expr2_ parameters
std::string MatrixRef::unparse() {
  std::cout << "unparsing MatrixRef ..." << std::endl;
  return var_name_->unparse() + " [ " + expr_->unparse() + " : " +
    expr2_->unparse() + " ] ";
}

//! NestedOrFuncCall returns the var_name_ and expr_ paramters
std::string NestedOrFuncCall::unparse() {
  std::cout << "unparsing NestedOrFuncCall ..." << std::endl;
  return var_name_->unparse() + " ( " + expr_->unparse() + " ) ";
}

//! ParenExpr returns the expr_ paramter
std::string ParenExpr::unparse() {
  std::cout << "unparsing ParenExpr ..." << std::endl;
  return "( " + expr_->unparse() + " )";
}

//! LetExpr returns the stmts_ and expr_ paramters
std::string LetExpr::unparse() {
  std::cout << "unparsing LetExpr ..." << std::endl;
  return "let " + stmts_->unparse() + " in " + expr_->unparse() + " end";
}

//! IfExpr returns expr_, expr2_ and expr3_ paramters
std::string IfExpr::unparse() {
  std::cout << "unparsing IfExpr ..." << std::endl;
  return "if " +  expr_->unparse() + " then " +  expr2_->unparse() +
    " else " + expr3_->unparse();
}

//! NotExpr returns a negated expr_ parameter
std::string NotExpr::unparse() {
  std::cout << "unparsing NotExpr ..." << std::endl;
  return "!" + expr_->unparse();
}

} /* namespace ast */
} /* namespace fcal */
