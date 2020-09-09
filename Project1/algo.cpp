
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <armadillo>
#include "algo.hpp"
#include "time.h" // you have to include the time.h header

using namespace std;
using namespace arma;


// vector v = [v1,v2,v3..., v_n]
// b_itilde = f(x)*h**2 = -v_i+1 + 2*v_i - v_i-1

inline double f(double x) {
  return 100.0*exp(-10.0*x);
}

inline double exact(double x) { //u(x)
  return 1.0-(1-exp(-10))*x-exp(-10*x);
}

void DiffSolver::Initialize(double a_val, double b_val, double c_val, int n_val, double x_0_val, double x_n_val, bool useSpecial){
  x_0 = x_0_val;
  x_n = x_n_val;
  n = n_val;
  m_useSpecial = useSpecial;
  a = new double[n];
  b = new double[n]; // Diagonal
  c = new double[n];
  g = new double[n]; // Right side

  x = new double[n];
  error = new double[n];

  u = new double[n+1];//Solution
  exact_ = new double[n+1];// Exact solution
  double h_val = (x_n-x_0)/(double)n;
  h = h_val;
  h_sq = pow( h_val , 2);

  for(int i = 1; i<n; i++){
      double x_i = x_0+i*h;
      x[i] = x_i;
      exact_[i] = exact(x_i);
      g[i] = f(x_i)*h_sq;
      //b[i] = b_val; XXXXXXXXXXXX
      if (m_useSpecial){
        b[i] = (i+1.0)/i;
      }
      else{
        b[i] = b_val;
        //cout << b[i] << endl;
      }
      a[i] = a_val;
      c[i] = c_val;
  }

  exact_[0] = exact(x_0);
  exact_[n] = exact(x_n);
}

void DiffSolver::Solve(){

  clock_t start, finish, finish2; // declare start and final time
  start = clock();

  for(int i = 2; i < n; i++){
    if (m_useSpecial){
      //b[i] = (i+1.0)/i; XXXXXXXXXXXX
      g[i] = g[i] + g[i-1]/b[i-1];
    }
    else{
      b[i] = b[i] - a[i-1]*c[i-1]/b[i-1];
      g[i] = g[i] - a[i-1]*g[i-1]/b[i-1];
    }
  }

  u[0] = 0; //boundry condition
  u[n] = 0; //boundry condition
  u[n-1] = g[n-1]/b[n-1]; //boundry condition where u[n] = 0

  int i = n - 2;
  while (i>0){
    if (m_useSpecial){
      u[i] = (g[i] + u[i+1])/b[i]; //3FLOPSx(n-2)
    }
    else{
      u[i] = (g[i] - c[i]*u[i+1])/b[i]; //3FLOPSx(n-2)
    }
    i -= 1;
  }
  //Fill array with log of relative error:
  for(int i = 1; i<n; i++){
    double relError = fabs( (u[i] - exact_[i])/exact_[i] );
    error[i] = log10(relError);
  }

  finish = clock();
  solvetime = ( (finish - start)/(double)CLOCKS_PER_SEC );
  //cout <<"Time to solve: (s): "<<( (finish - start)/(double)CLOCKS_PER_SEC ) << endl;

  //Delete unneccesary arrays: a, b, c, g
  delete [] a; delete [] b; delete [] c; delete [] g;
}

void DiffSolver::PrintError(){
  for (int i = 1; i<n; i++){
    cout << setprecision(8) << error[i] << endl;
  }
}

void DiffSolver::WritetoFile(){
  // Write to file (CSV file) with 4 columns: [x_i, solution, exact_, log_rel_error]
  ofstream outfile;
  string filename ="ResultsComputation/ResultsG_nval=" ;
  if (m_useSpecial){
    filename = "ResultsComputation/ResultsS_nval=";
  }
  filename.append(to_string(n));
  outfile.open(filename);

  //Write the results into a comma seperated file to make use of pandas read_csv()
  for(int i = 1; i < n; i++){
    outfile << setprecision(7);
    outfile << x[i] << ", " << u[i] << ", " << exact_[i] << ", " << error[i] << endl;
  }
  outfile.close();

  //Delete no longer needed data, since its available in a file
  delete[] x; delete[] error;
}

void DiffSolver::Printtest(){
  // Simple test to verify result
  for(int i = 1; i < n; i+= 1){
    cout <<"Sol: " <<u[i] << "   Exact: " << exact_[i] <<endl;
  }
}

void DiffSolver::SolveLU(double a_val, double b_val, double c_val){

  clock_t start, finish; // declare start and final time
  start = clock();

  int n_ = n-1; //Make sure dont change the class variable n
  mat A = zeros<mat>(n_,n_);
  // Set up arrays for the simple case
  vec g(n_);  vec x(n_); //Ax=g

  A(0,0) = b_val;  A(0,1) = c_val;  x(0) = h;  g(0) =  h_sq*f(x(0)); //Correct cases where i+1 gives error in for loop
  x(n_-1) = x(0)+(n_-1)*h; g(n_-1) = h_sq*f(x(n_-1));
  for (int i = 1; i < n_-1; i++){
    x(i) = x(i-1)+h;
    g(i) = h_sq*f(x(i));
    A(i,i-1)  = a_val;
    A(i,i)    = b_val;
    A(i,i+1)  = c_val;
  }
  A(n_-1,n_-1) = b_val; A(n_-2,n_-1) = a_val; A(n_-1,n_-2) = c_val;
  //A.print("A =");
  mat L, U;
  lu(L,U,A); //find LU decomposition
  //(A-L*U).print("Test of LU decomposition");   //Check that A = LU

  vec y = solve(L,g); // find y, Ly=g there y=Ux using forward substitution
  vec solution = solve(U,y); // find x, Ux=y using backward substitution
  finish = clock();

  ofstream outfile;
  string filename ="ResultsComputation/ResultsLU_nval=";
  filename.append(to_string(n));
  outfile.open(filename);
  outfile << solution << endl;
  outfile.close();

  solvetimeLU = ( (finish - start)/(double)CLOCKS_PER_SEC );
}
