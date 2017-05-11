/*******************************************************************************
 * Name            : Matrix
 * Project         : fcal
 * Module          : Matrix
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Tommy Her, Jennifer Lieu
 *
 ******************************************************************************/

#include "include/Matrix.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

// using namespace std;

matrix::matrix(int i, int j) : rows(i), cols(j) {
  data = static_cast<float *>(operator new (sizeof(int) * i * j));
}

matrix::matrix(const matrix& m) : rows(m.rows), cols(m.cols) {
  int i, j;
  data = static_cast<float *>(operator new (sizeof(int) * m.rows * m.cols));
  for (i = 0; i <= rows; ++i) {
    for (j = 0; j <= cols; ++j) {
      *(access(i, j)) = *(m.access(i, j));
    }
  }
}

int matrix::n_rows() const {return rows;}
int matrix::n_cols() const {return cols;}

float *matrix::access(const int i, const int j) const {
  return data + (i * cols) + j;
}

matrix matrix::matrix_read(std::string filename) {
  std::ifstream in(filename.c_str());

  if (!in) {
    throw "File" + filename + " not found.";
  }

  int r;
  in >> r;
  int c;
  in >> c;

  matrix temp(r, c);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      in >> *(temp.access(i, j));
    }
  }
  return temp;
}

matrix matrix::operator +(const matrix &rightside) {
  if (rightside.n_rows() == this->n_rows()) {
    if (rightside.n_cols() == this->n_cols()) {
      for (int i = 0; i < rightside.n_rows(); i++) {
        for (int j = 0; j < rightside.n_cols(); j++) {
          *(rightside.access(i, j)) = *(this->access(i, j))
            + *(rightside.access(i, j));
        }
      }
    }
  }
  return rightside;
}

void matrix::operator =(const matrix &rightside) {
  if (rightside.n_rows() == this->n_rows()) {
    if (rightside.n_cols() == this->n_cols()) {
      for (int i = 0; i < this->n_rows(); i++) {
        for (int j = 0; j < this->n_cols(); j++) {
          *(this->access(i, j)) = *(rightside.access(i, j));
        }
      }
    }
  }
}

matrix matrix::operator *(const matrix &rightside) {
  if (rightside.n_rows() != this->n_cols()) {
    throw "Dimensions of matrices do not match";
  }
  matrix newMatrix(this->n_rows(), rightside.n_cols());
  for (int i = 0; i < this->n_rows(); i++) {
    for (int j = 0; j < rightside.n_cols(); j++) {
      *(newMatrix.access(i, j)) = 0;
    }
  }
  for (int i = 0; i < this->n_rows(); i++) {
    for (int j = 0; j < rightside.n_cols(); j++) {
      for (int k = 0; k < this->n_cols(); k++) {
        *(newMatrix.access(i, j)) = *(newMatrix.access(i, j)) +
          (*(this->access(i, k)) * *(rightside.access(k, j)));
      }
    }
  }
  return newMatrix;
}


/*  Printing a matrix does not print a new-line when complete. This is
    different from printing other data type such as integers and strings.
    In those types a trailing new-line is not printed
*/

std::ostream& operator <<(std::ostream &os, matrix &m) {
  os << m.rows << " " << m.cols << std::endl;
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      os << *(m.access(i, j)) << "  ";
    }
    os << std::endl;
  }
  return os;
}
