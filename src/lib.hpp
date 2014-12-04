#ifndef _LIB_H_
#define _LIB_H_

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

class Matrix {
  public:
    /// Construct a empty (0x0) matrix.
    Matrix();

    /// Construct a matrix object with the specified dimensions (rows x cols), initialized to value.
    Matrix(unsigned rows, unsigned cols, double value = 0.0);

    /// Copy a existing matrix.
    Matrix(const Matrix&);

    /// Construct a column vector from a vector.
    Matrix(const vector<double>&);

    /// Construct a matrix from a vector of vectors.
    Matrix(const vector<vector<double> >&);

    /// Return the number of columns of this matrix.
    unsigned getCols() const;

    /// Return the number of rows of this matrix.
    unsigned getRows() const;

    /// Get the value of the ith element. Column-major.
    double get(unsigned i) const;

    /// Get the value of the Aij element.
    double get(unsigned i, unsigned j) const;

    /// Set the value of the ith element. Column-major.
    void set(unsigned i, double value);

    /// Set Aij to value.
    void set(unsigned i, unsigned j, double value);

    // Usual matrix operations.
    Matrix operator+(const Matrix&) const;
    Matrix operator-(const Matrix&) const;
    Matrix operator*(const Matrix&) const;
    Matrix operator*(double) const;
    Matrix operator/(double) const; 
    friend Matrix operator*(double, const Matrix&);

    /// Return the transpose of this matrix.
    Matrix transpose() const;

    /// Transpose alias.
    Matrix t() const;

    /// Return true if this is a vector (i.e., a row vector or a column vector).
    bool isVector() const;

    /// IF this is a 2x2 matrix, return its determinant.
    double det2() const;

    /// If this is a vector, return its module.
    double mod() const;

    /// If this is a 1x1 vector, return its only element.
    double x() const;

    /// If this is a 2x1 vector, return its first element.
    double x1() const;

    /// If this is a 2x1 vector, return its second element.
    double x2() const;

    /// Return a string representation of this matrix.
    void debug() const;

    /// Return the number of elements of this matrix.
    unsigned length() const;

  private:
    /** Dimensions of the Matrix.
     * m = number of lines
     * n = number of columns
     */
    unsigned m, n;

    /// Internal representation of the matrix.
    vector< vector<double> > v;
};

Matrix eye(unsigned n) {
  Matrix w(n,n,0.0);
  for (unsigned i = 1; i <= n; ++i)
    w.set(i,i,1.0);
  return w;
}

Matrix::Matrix() :
  m(0), 
  n(0) {
  v.clear();
}

Matrix::Matrix(unsigned rows, unsigned cols, double value) :
  m(rows),
  n(cols) {
  v = vector< vector<double> >(rows, vector<double>(cols, value));
}

Matrix::Matrix(const Matrix& o) :
  m(o.m),
  n(o.n),
  v(o.v) {
}

Matrix::Matrix(const vector<double>& w) :
  m(w.size()),
  n(1) {
    for (unsigned i = 0; i < w.size(); ++i)
      v.push_back(vector<double>(1, w[i]));
}

double Matrix::det2() const {
  if (m != 2 && n != 2)
    throw std::invalid_argument("Can't apply det2 to a non 2x2 matrix");
  return get(1,1) * get(2,2) - get(1,2) * get(2,1); 
}

Matrix::Matrix(const vector<vector<double> >& w) :
  m(w.size()),
  n(w[0].size()),
  v(w) {
}

unsigned Matrix::getCols() const {
  return n;
}

unsigned Matrix::getRows() const {
  return m;
}

double Matrix::get(unsigned i) const {
  return v.at((i - 1) % m).at((i-1) / m);
}

void Matrix::set(unsigned i, double value) {
  v.at((i - 1) % m).at((i-1) / m) = value;
}

double Matrix::get(unsigned i, unsigned j) const {
  return v.at(i-1).at(j-1);
}

void Matrix::set(unsigned i, unsigned j, double value) {
  v.at(i-1).at(j-1) = value;
}
    
Matrix Matrix::operator+(const Matrix& o) const {
  Matrix a(m, n);
  for (unsigned i = 1; i <= m; ++i)
    for (unsigned j = 1; j <= n; ++j)
      a.set(i, j, get(i,j) + o.get(i,j));
  return a;
}

Matrix Matrix::operator-(const Matrix& o) const {
  Matrix a(m, n);
  for (unsigned i = 1; i <= m; ++i)
    for (unsigned j = 1; j <= n; ++j)
      a.set(i, j, get(i,j) - o.get(i,j));
  return a;
}

Matrix Matrix::operator*(double s) const {
  Matrix a(m, n);
  for (unsigned i = 1; i <= m; ++i)
    for (unsigned j = 1; j <= n; ++j)
      a.set(i, j, s * get(i,j));
  return a;
}

Matrix Matrix::operator/(double s) const {
  Matrix a(m, n);
  for (unsigned i = 1; i <= m; ++i)
    for (unsigned j = 1; j <= n; ++j)
      a.set(i, j, s / get(i,j));
  return a;
}

Matrix Matrix::t() const {
  return this->transpose();
}

Matrix Matrix::transpose() const {
  Matrix w(getCols(), getRows());
  for (unsigned i = 1; i <= getRows(); ++i)
    for (unsigned j = 1; j <= getCols(); ++j)
      w.set(j,i,get(i,j));
  return w;
}

bool Matrix::isVector() const {
  return m == 1 || n == 1;
}

double Matrix::mod() const {
  double sum = 0.0;
  for (unsigned i = 1; i <= length(); ++i)
    sum += get(i) * get(i);
  return sqrt(sum);
}

double Matrix::x() const {
  if (m == 1 && n == 1)
    return get(1,1);
  else
    throw std::invalid_argument("Not a 1x1 Matrix");
}

double Matrix::x1() const {
  if (m == 2 && n == 1)
    return get(1,1);  
  else
    throw std::invalid_argument("Not a 2x1 column vector");
}

double Matrix::x2() const {
  if (m == 2 && n == 1)
    return get(2,1);  
  else
    throw std::invalid_argument("Not a 2x1 column vector");
}

unsigned Matrix::length() const {
  return m * n;
}

Matrix Matrix::operator*(const Matrix& o) const {
  Matrix w(getRows(), o.getCols());
  if (getCols() != o.getRows())
    throw std::invalid_argument("Invalid matrix multiplication");
  for (unsigned i = 1; i <= getRows(); ++i)
    for (unsigned j = 1; j <= o.getCols(); ++j) {
      double sum = 0.0;
      for (unsigned k = 1; k <= getCols(); ++k) {
        sum += get(i,k) * o.get(k,j);
      }
      w.set(i,j,sum);
    }
  return w;
}

Matrix operator*(double s, const Matrix& o) {
  return o * s;
}

void Matrix::debug() const {
  std::cout << "INFO: Matrix debug" << std::endl;
  std::cout << "\t" << "#rows=" << m << ", #cols=" <<  n << std::endl;
  for (unsigned i = 1; i <= m; ++i) {
    std::cout << "\t";
    for (unsigned j = 1; j <= n; ++j)
      std::cout << get(i,j) << " ";
    std::cout << std::endl;
  }
}

double fa(Matrix x) {
  return (x.x1() * x.x1()) + pow(exp(x.x1()) - x.x2(), 2);
}

/**
 * Regra de Armijo.
 * Encontrar um t = sb^m tal que
 * f(x + sb^m d) - f(x) <= o sb^m gradf(x)' d ==>
 * f(x + td) - f(x) <= o t gradf(x)' d
 *
 * Constraints:
 *    m >= 0
 *    0 < o < 1 (sigma)
 *    0 < b < 1 (beta)
 *    0 << t < 1 (bs^m)
 */
// TOTEST.
double armijo_call(
    double s,
    double beta,              // 0 < b < 1
    double sigma,             // 0 < o < 1
    double (*f)(Matrix),
    Matrix (*gradf)(Matrix),
    const Matrix& x,
    const Matrix& d
    ) 
{
  std::cout << "INFO: armijo_call run" << std::endl;

  // Skipping right through the test means 1 iteration.
  unsigned iter = 0;

  while (true) {
    if ( ((*f)(x) - (*f)(x + s * pow(beta, iter) * d)) >= -sigma * s * pow(beta, iter) * (((*gradf)(x)).t() * d).x() )
      break;

    ++iter;
  }

  double t = s * pow(beta, iter);
  std::cout << "\t#iter=" << iter+1 << ", t=" << t << std::endl;
  return t;
}

Matrix gradient_method(
    double (*f)(Matrix),
    Matrix (*gradf)(Matrix),
    Matrix x0,
    double epsilon
    )
{
  std::cout << "INFO: gradient_method run" << std::endl;
  std::cout << "\t" << "initial point: " << "(" << x0.x1() << ", " << x0.x2() << ")" << std::endl;
  std::cout << "\t" << "epsilon: " << epsilon << std::endl;

  Matrix dk;                  // descida (o gradiente)
  Matrix xk = x0;             // x atual
  Matrix xnext;               // Pŕoximo x (computado a cada iteração).
  unsigned iter = 0;          // Iteração
  unsigned n_call_armijo = 0; // Número de chamadas de Armijo.

  while(true) {
    // Critério de parada.
    if (((*gradf)(xk)).mod() < epsilon) 
      break;
    ++iter;

    dk = (-1) * (*gradf)(xk);

    double ak = armijo_call(0.8, 0.8, 0.8, f, gradf, xk, dk);
    ++n_call_armijo;

    xnext = xk + ak * dk;
    xk = xnext;
  }

  std::cout << "\t" << "n_iterations: " << iter + 1 << std::endl;
  std::cout << "\t" << "n_call_armijo: " << n_call_armijo << std::endl;
  std::cout << "\t" << "optimal point: " << "(" << xk.x1() << ", " << xk.x2() << ")" << std::endl;
  std::cout << "\t" << "optimal value: " << (*f)(xk) << std::endl;

  return xk;
}

#endif
