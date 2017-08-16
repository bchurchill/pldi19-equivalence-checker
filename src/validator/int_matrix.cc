// Copyright 2013-2016 Stanford University
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/validator/int_matrix.h"

#include <cassert>
#include <iostream>
#include <ostream>
#include <fstream>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define DEBUG_CUTPOINTS(X) { }


using namespace std;
using namespace stoke;


IntVector IntMatrix::operator*(IntVector& vect) const {
  auto& matrix = *this;
  assert(matrix[0].size() == vect.size());
  IntVector results;
  for (size_t i = 0; i < matrix.size(); ++i) {
    int64_t sum = 0;
    for (size_t j = 0; j < vect.size(); ++j) {
      sum += matrix[i][j]*vect[j];
    }
    results.push_back(sum);
  }
  return results;
}

IntMatrix IntMatrix::operator*(IntMatrix& other) const {
  auto& matrix = *this;
  assert(matrix[0].size() == other.size());

  IntMatrix result(matrix.size(), other[0].size());

  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < other[0].size(); ++j) {
      int64_t sum = 0;
      for (size_t k = 0; k < other.size(); ++k) {
        sum += matrix[i][k]*other[k][j];
      }
      result[i][j] = sum;
    }
  }
  return result;
}

bool IntMatrix::in_nullspace(IntVector& vect) const {
  auto& matrix = *this;
  assert(matrix[0].size() == vect.size());
  for (size_t i = 0; i < matrix.size(); ++i) {
    int64_t sum = 0;
    for (size_t j = 0; j < vect.size(); ++j) {
      sum += matrix[i][j]*vect[j];
    }
    if (sum != 0)
      return false;
  }
  return true;
}

IntMatrix IntMatrix::solve_diophantine() const {
  assert(check_rectangle());

  auto& matrix = *this;
  cout << "Writing out sage code" << endl;
  ofstream of("in.sage");
  of << "rows=" << matrix.size() << endl;
  of << "cols=" << matrix[0].size() << endl;
  of << "ZZ=IntegerRing()" << endl;
  of << "A = MatrixSpace(ZZ, rows, cols)([";
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      of << matrix[i][j];
      if (i < matrix.size() - 1 || j < matrix[i].size() - 1)
        of << ", ";
    }
  }
  of << "])" << endl;
  of << "D,U,V=A.smith_form()" << endl;
  of << "min_dim = min(rows,cols)" << endl;
  of << "diagonals = [ D[i][i] for i in range(0,min_dim) if D[i][i] != 0]" << endl;
  of << "nz_diag = len(diagonals)" << endl;
  of << "bv_len = len(V.rows())" << endl;
  of << "basis = [ [0]*nz_diag + [0]*i + [1] + [0]*(bv_len-nz_diag-i-1) for i in range(0,bv_len-nz_diag)]" << endl;
  of << "dim = len(basis)" << endl;
  of << "outputs = [ V*vector(b) for b in basis ]" << endl;
  of << "print len(outputs), len(outputs[0])" << endl;
  of << "for output in outputs:" << endl;
  of << "\tprint \" \".join(map(lambda x:str(x), output))" << endl;
  of << endl;
  of.close();
  int status = system("sage in.sage > sage.out 2>sage.err");

  /** Read basis vectors from sage */
  IntMatrix basis_vectors;
  size_t output_rows, output_cols;
  ifstream in("sage.out");
  in >> output_rows >> output_cols;
  if (!in.good()) {
    assert(false);
    return basis_vectors;
  }
  for (size_t i = 0; i < output_rows; ++i) {
    IntVector row;
    for (size_t j = 0; j < output_cols; ++j) {

      int64_t x;
      in >> x;

      /** Check to make sure that we don't have any parser errors */
      if (!in.good()) {
        assert(false);
        return basis_vectors;
      }

      row.push_back(x);
    }
    basis_vectors.push_back(row);
  }

  return basis_vectors;
}

IntVector IntMatrix::solve_diophantine(IntVector b) const {
  IntVector result;
  return result;
}

void IntMatrix::print() const {
  auto& m = *this;
  for (size_t i =0; i < m.size(); ++i) {
    for (size_t j = 0; j < m[i].size(); ++j) {
      cout << m[i][j] << "  ";
    }
    cout << endl;
  }
}


IntMatrix IntMatrix::remove_column(size_t index) const {
  auto& matrix = *this;
  IntMatrix result;
  for (size_t i = 0; i < matrix.size(); ++i) {
    IntVector v;
    for (size_t j = 0; j < matrix[0].size(); ++j) {
      if (j == index)
        continue;
      v.push_back(matrix[i][j]);
    }
    result.push_back(v);
  }
  return result;
}


