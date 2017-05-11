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
#include <ctype.h>

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

std::string Program::CppCode() {
  std::cout << "\nbuilding Program (Root) C++..." << std::endl;
  return "#include <iostream>\n#include \"include/Matrix.h\"\n#include <math.h>"
  "\nusing namespace std;\nint " + var_name_->CppCode() +
  "() {\n" + stmts_->CppCode() + "}";
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

std::string VarName::CppCode() {
  std::cout << "building VarName C++ ..." << std::endl;
  if (lexeme_ == "matrix_read") {
    return "matrix::matrix_read";
  } else {
  return lexeme_;
  }
}

// Stmts
// ----------------------------------------------------------------------------
//! SeqStmts unparse() returns the stmt_ and stmts_ parameters
std::string SeqStmts::unparse() {
  std::cout << "unparsing SeqStmts ..." << std::endl;
  return stmt_->unparse() + stmts_->unparse();
}

std::string SeqStmts::CppCode() {
  std::cout << "building SeqStmts C++..." << std::endl;
  return stmt_->CppCode() + stmts_->CppCode();
}

//! EmptyStmts unparse() returns nothing
std::string EmptyStmts::unparse() {
  std::cout << "unparsing EmptyStmts ..." << std::endl;
  return "";
}

std::string EmptyStmts::CppCode() {
  std::cout << "building EmptyStmts C++..." << std::endl;
  return "";
}

// Stmt
// ----------------------------------------------------------------------------
//! StmtDecl unparse() returns the decl_ parameter
std::string StmtDecl::unparse() {
  std::cout << "unparsing StmtDecl ..." << std::endl;
  return decl_->unparse();
}

std::string StmtDecl::CppCode() {
  std::cout << "building StmtDecl C++..." << std::endl;
  return decl_->CppCode();
}

//! BlockStmt unparse() returns the stmts_ parameter
std::string BlockStmt::unparse() {
  std::cout << "unparsing BlockStmt ..." << std::endl;
  return "{" + stmts_->unparse() + "}";
}

std::string BlockStmt::CppCode() {
  std::cout << "building BlockStmt C++..." << std::endl;
  return stmts_->CppCode();
}

//! IfStmt unparse() returns the expr_ and stmt_ parameters
std::string IfStmt::unparse() {
  std::cout << "unparsing IfStmt ..." << std::endl;
  return "if (" + expr_->unparse() + ")" + stmt_->unparse() + "\n";
}

std::string IfStmt::CppCode() {
  std::cout << "building IfStmt C++..." << std::endl;
  return "if (" + expr_->CppCode() + ")" + stmt_->CppCode() + "\n";
}

//! IfElseStmt unparse() returns the expr_, stmt_ and stmt2_ parameters
std::string IfElseStmt::unparse() {
  std::cout << "unparsing IfElseStmt ..." << std::endl;
  return "if (" + expr_->unparse() + ")" + stmt_->unparse() + "}\n else" +
    stmt2_->unparse() + "\n";
}

std::string IfElseStmt::CppCode() {
  std::cout << "building IfElseStmt C++..." << std::endl;
  return "if (" + expr_->CppCode() + ")" + stmt_->CppCode() + "}\n else" +
    stmt2_->CppCode() + "\n";
}

//! PrintStmt unparse() returns the expr_ parameter
std::string PrintStmt::unparse() {
  std::cout << "unparsing PrintStmt ..." << std::endl;
  return "print(" + expr_->unparse() + "); \n";
}

std::string PrintStmt::CppCode() {
  std::cout << "building PrintStmt C++..." << std::endl;
  return "cout << " + expr_->CppCode() + ";\n";
}

//! AssignStmt unparse() returns var_name_, expr_
std::string AssignStmt::unparse() {
  std::cout << "unparsing AssignStmt ..." << std::endl;
  return var_name_->unparse() + " = " + expr_->unparse() + ";\n";
}

std::string AssignStmt::CppCode() {
  std::cout << "building AssignStmt C++..." << std::endl;
  return var_name_->CppCode() + " = " + expr_->CppCode() + ";\n";
}

//! AssignLongStmt unparse() returns var_name_, expr_, expr2_ and expr3_
std::string AssignLongStmt::unparse() {
  std::cout<< "unparsing AssignLongStmt ..." << std::endl;
  return var_name_->unparse() + " [ " + expr_->unparse() + " : " +
    expr2_->unparse() + " ] =" + expr3_->unparse() + ";\n";
}

std::string AssignLongStmt::CppCode() {
  std::cout << "building AssignLongStmt C++..." << std::endl;
  return "*(" + var_name_->CppCode() + ".access(" + expr_->CppCode() + ", " +
    expr2_->CppCode() + ")) = " + expr3_->CppCode() + ";\n";
}

//! RepeatStmt returns var_name_, expr_, expr2_ and stmt_
std::string RepeatStmt::unparse() {
  std::cout<< "unparsing RepeatStmt ..." << std::endl;
  return "repeat (" + var_name_->unparse() + "=" + expr_->unparse() + " to " +
    expr2_->unparse() + " )" + stmt_->unparse();
}

// NOT sure if this is the best way to attempt a Repeat (Do-while Loop)
std::string RepeatStmt::CppCode() {
  std::cout << "building RepeatStmt C++..." << std::endl;
  return "for (" + var_name_->CppCode() + " = " + expr_->CppCode() + "; "
    + var_name_->CppCode() + " <= " + expr2_->CppCode() + "; " +
    var_name_->CppCode() + "++) {\n" + stmt_->CppCode() + "\n}\n";
}

//! WhileStmt returns expr_ and stmt_ parameters
std::string WhileStmt::unparse() {
  std::cout << "unparsing WhileStmt ..." << std::endl;
  return "while(" + expr_->unparse() + ")" + stmt_->unparse() + "\n";
}

std::string WhileStmt::CppCode() {
  std::cout << "building WhileStmt C++..." << std::endl;
  return "while(" + expr_->CppCode() + ")" + "{" + stmt_->CppCode() + "}" + "\n";
}

//! EndStmt returns a semicolon (;) for end of line
std::string EndStmt::unparse() {
  std::cout<< "unparsing EndStmt ..." << std::endl;
  return ";";
}

std::string EndStmt::CppCode() {
  std::cout << "building EndStmt C++..." << std::endl;
  return ";";
}

// Decl
// ----------------------------------------------------------------------------
//! TypeDecl returns the type_ and var_name_ of a declaration
std::string TypeDecl::unparse() {
  std::cout << "unparsing TypeDecl ..." << std::endl;
  return type_->unparse() + " " + var_name_->unparse() + ";\n";
}

std::string TypeDecl::CppCode() {
  std::cout << "building TypeDecl C++..." << std::endl;
  return type_->CppCode() + " " + var_name_->CppCode() + ";\n";
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

/*  for (int vn2 = 0; vn2 < exp; vn2++) {
      for (int vn3 = 0; vn3 < expr2; vn23++) {
    }
}
*/
std::string MatrixLongDecl::CppCode() {
  std::cout << "building MatrixLongDecl C++..." << std::endl;
  return "matrix " + var_name_->CppCode() + "(" + expr_->CppCode() + ", " +
    expr2_->CppCode() + ");\n" + "for (int " + var_name2_->CppCode() + " = 0; "
    + var_name2_->CppCode() + " < " + expr_->CppCode() + "; " +
    var_name2_->CppCode() + "++) {\nfor (int " + var_name3_->CppCode() +
    " = 0; " + var_name3_->CppCode() + " < " + expr2_->CppCode() + "; " +
    var_name3_->CppCode() + "++) {\n *(" + var_name_->CppCode() + ".access("
    + var_name2_->CppCode() + ", " + var_name3_->CppCode() + ")) = " +
    expr3_->CppCode() + ";\n}\n}\n";
}

//! MatrixDecl returns var_name_ and expr_
std::string MatrixDecl::unparse() {
  std::cout << "unparsing MatrixDecl ..." << std::endl;
  return "matrix " + var_name_->unparse() + " = " + expr_->unparse() + ";\n";
}

std::string MatrixDecl::CppCode() {
  std::cout << "building MatrixDecl C++..." << std::endl;
  return "matrix " + var_name_->CppCode() + "(" + expr_->CppCode() + ");\n";
}

// Expr
// ----------------------------------------------------------------------------
//! TypeConst returns the type_const of a data type
std::string TypeConst::unparse() {
  std::cout << "unparsing TypeConst ..." << std::endl;
  return type_const_;
}

std::string TypeConst::CppCode() {
  std::cout << "building TypeConst C++..." << std::endl;
  return type_const_;
}

//! BoolTrue returns the "True" string for a boolean truth
std::string BoolTrue::unparse() {
  std::cout << "unparsing BoolTrue ..." << std::endl;
  return "True";
}

std::string BoolTrue::CppCode() {
  std::cout << "building BoolTrue C++..." << std::endl;
  return "True";
}

//! BoolFalse returns a "False" string for a boolean false
std::string BoolFalse::unparse() {
  std::cout << "unparsing BoolFalse ..." << std::endl;
  return "False";
}

std::string BoolFalse::CppCode() {
  std::cout << "building BoolFalse C++..." << std::endl;
  return "False";
}

//! BinaryOp returns the expr_, op_ and expr2_
std::string BinaryOp::unparse() {
  std::cout << "unparsing BinaryOp ..." << std::endl;
  return expr_->unparse() + " " + *op_ + " " + expr2_->unparse();
}

std::string BinaryOp::CppCode() {
  std::cout << "building BinaryOp C++..." << std::endl;
  return "( " + expr_->CppCode() + " )" + *op_ + "( " + expr2_->CppCode() + ")";
}

//! MatrixRef returns the var_name_, expr_ and expr2_ parameters
std::string MatrixRef::unparse() {
  std::cout << "unparsing MatrixRef ..." << std::endl;
  return var_name_->unparse() + " [ " + expr_->unparse() + " : " +
    expr2_->unparse() + " ] ";
}

std::string MatrixRef::CppCode() {
  std::cout << "building MatrixRef C++..." << std::endl;
  return "*( " + var_name_->CppCode() + ".access(" + expr_->CppCode() + ", " +
  expr2_->CppCode() + ")) ";
}

//! NestedOrFuncCall returns the var_name_ and expr_ paramters
std::string NestedOrFuncCall::unparse() {
  std::cout << "unparsing NestedOrFuncCall ..." << std::endl;
  return var_name_->unparse() + " ( " + expr_->unparse() + " ) ";
}

std::string NestedOrFuncCall::CppCode() {
  std::cout << "building NestedOrFuncCall C++..." << std::endl;
  if (var_name_->CppCode() == "n_rows" || var_name_->CppCode() == "n_cols") {
    return expr_->CppCode() + "." + var_name_->CppCode() + "()";
  }
  return var_name_->CppCode() + "(" + expr_->CppCode() + ")";
}

//! ParenExpr returns the expr_ paramter
std::string ParenExpr::unparse() {
  std::cout << "unparsing ParenExpr ..." << std::endl;
  return "( " + expr_->unparse() + " )";
}

std::string ParenExpr::CppCode() {
  std::cout << "building ParenExpr C++..." << std::endl;
  return expr_->CppCode();
}

//! LetExpr returns the stmts_ and expr_ paramters
std::string LetExpr::unparse() {
  std::cout << "unparsing LetExpr ..." << std::endl;
  return "let " + stmts_->unparse() + " in " + expr_->unparse() + " end";
}

std::string LetExpr::CppCode() {
  std::cout << "building LetExpr C++..." << std::endl;
  return "({" + stmts_->CppCode() + "\n" + expr_->CppCode() + ";})";
}

//! IfExpr returns expr_, expr2_ and expr3_ paramters
std::string IfExpr::unparse() {
  std::cout << "unparsing IfExpr ..." << std::endl;
  return "if " +  expr_->unparse() + " then " +  expr2_->unparse() +
    " else " + expr3_->unparse();
}

std::string IfExpr::CppCode() {
  std::cout << "building IfExpr C++..." << std::endl;
  return "((" + expr_->CppCode() + ") ? " + expr2_->CppCode() + " : " +
    expr3_->CppCode() + ")";
}

//! NotExpr returns a negated expr_ parameter
std::string NotExpr::unparse() {
  std::cout << "unparsing NotExpr ..." << std::endl;
  return "!" + expr_->unparse();
}

std::string NotExpr::CppCode() {
  std::cout << "building NotExpr C++..." << std::endl;
  return "!" +expr_->CppCode();
}

} /* namespace ast */
} /* namespace fcal */
