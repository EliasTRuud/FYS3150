#ifndef JACOBIALGO_HPP
#define JACOBIALGO_HPP

#include <vector>
#include <string.h>
#include <tuple>
#include <armadillo>

using namespace std;
using namespace arma;

class JacobiEigenSolve {
  private:

    // Size of matrix is n = N - 1. So use n+1 when comparing with anaylytical solution!!!

    Mat<double> A; // Symmetric matrix to diagonalize,  Dim: n x n
    Mat<double> A_test;
    Mat<double> R; //Matrix to contain eigenvectors
    int n; // Size of the matrix
    double eps = 1.0e-8;
    double h;
    double max_iterations;
    double a, b; // a - lower & upper diagonal, b - middle diagonal
    vec eig;
    //int k, l; // Changed each time
  public:
    void Initialize(double a_val, double b_val, int n_val, double rho_max_val, double V(double x, double omega), double omega); // make the symmetric matrix and empty V matrix.
    void FindMaxEle(double&, int&, int&); // Returns a tuple of value
    //of element and its index. (val, row, column) => f.eks (2.2, 1, 3)
    void Rotate(int k, int l); // finds the value of cos(theta) and sin(theta)
    void Solve(); //Simply runs the rotation until we reached the max ite or reached the eps
    void PrintA(); // Prints A, used for checks
    void TestInitialize();
    void TestFindMaxEle();
    void TestSolve();
    void TestOrthogonality();
    void Write_Results(string filename, string solution);

    double timeArma; double timeClass;
    int iterations = 0;

    //enkel matrise 3x3, analyisk rotasjonsmatrisen. Gjøre alle steg for hånd
    // sjekke om egenverdiene funker.
};

#endif
