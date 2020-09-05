#ifndef ALGO_HPP
#define ALGO_HPP

#include <vector>
#include <string.h>
using namespace std;

class DiffSolver {
  private:
    double x_0, x_n, h, h_sq; //Start and stop interval
    int n; //Number of steps (integration)
    //vector<double> a, b, c, u, g; //maybe use vectors
    double* a;
    double* b;
    double* c;
    double* g; // right side of matrix mul: A*x = g

    double* x; // Linspave of x0 to xn with n values
    double* error;

    double* u; //Solution
    double* exact_;// Exact solution

  public:
    void Initialize(double a_val, double b_val, double c_val, int n, double x_0, double x_n, bool useSpecial = false); //Created all the needed vectors
    void Solve(bool useSpecial = false); // Solves the system using BackAndFw solution
    void PrintError(); //Prints the error
    void WritetoFile(); // write solution to csv file [x_i, solution, exact_, log_rel_error]
    void Printtest();
    double solvetime; double solvetimeLU;
    void SolveLU(double a_val, double b_val, double c_val); //Solves the system using LU decomposition
};

#endif
