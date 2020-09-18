#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <armadillo>
#include "jacobiAlgo.hpp"

using namespace std;
using namespace arma;


void JacobiEigenSolve::Initialize(double a_val, double b_val, int n_val){
  //Set class variables
  n = n_val; //Points
  h = 1.0/(n+1);  // Step size (x-x0)/N = (x-x0)/(n+1)
  a = a_val/(h*h); b = b_val/(h*h);
  max_iterations = (double) n * (double) n * (double) n;
  //max_iterations = 100;
  A = zeros<mat>(n,n);
  R.eye(n,n);


  for (int i=0; i<n; i++){
    if (i<n-1){
      A(i,i) = b;
      A(i,i+1) = a;
      A(i+1,i) = a;
    } else{
      A(i,i) = b;
    }
  }

  A_test = repmat(A, 1, 1); //Make a copy of A to be used in tests
  cout << "Fasit eigenvalues: \n" << eig_sym(A) << endl;
  //cout << "Fasit eigenvectors: \n " << eigs_gen(A, n) << endl;
  //cout << A << endl;
  return;
}

// Returns a tuple of value of element and its index. (val, row, column) => f.eks (2.2, 1, 3)
tuple<double, int, int> JacobiEigenSolve::FindMaxEle(){
  double max = 0.0;
  int row, col;
  for (int i =0; i <n; i++){
    for (int j = i+1; j<n; j++){
      if ( fabs(A(i,j)) > max){
        max = fabs(A(i,j));
        row = i;
        col = j;
      }
    }
  }
  return {max, row, col};

}

// finds the value of cos(theta) and sin(theta) and rotating matrix A fills up V each time
void JacobiEigenSolve::Rotate(int l, int k){
  /*
  Finding cos(theta) and sin(theta)
  */
  double cos_, sin_, tan_, tau;
  //cout << "I rotate: \n"<< A <<"\n"<< A << endl;
  //cout << "l: " <<l <<  " k: " << k << endl;;
  if (A(l,k) != 0.0){
    //cout << "k " << k<< endl;
    tau = (A(l,l)-A(k,k)) / (2.0*A(k,l));
    //cout << "tau " << tau<< endl;

    if (tau > 0){
      tan_ = 1.0 / (tau + sqrt(1.0 + tau*tau));
    }
    else{
      tan_ = -1.0 / (-tau + sqrt(1.0 + tau*tau));
    }

    cos_ = 1.0 / sqrt(1.0 + tan_*tan_);
    sin_ = cos_ * tan_;
  }
  else{
    cos_ = 1.0;
    sin_ = 0.0;
  }

  /*
  //Rotating matrix A
  */
  double a_kk, a_ll, a_ik, a_il;
  a_kk = A(k,k);
  a_ll = A(l,l);

  //Computing new matrix elements with indices k and l
  A(k,k) = cos_*cos_*a_kk - 2.0*cos_*sin_*A(k,l) + sin_*sin_*a_ll;
  A(l,l) = sin_*sin_*a_kk + 2.0*cos_*sin_*A(k,l) + cos_*cos_*a_ll;
  A(k,l) = 0.0; //
  A(l,k) = 0.0;

  //Computing the new remaining elements
  for (int i = 0; i <n; i++){
    if (i != k && i != l){
      a_ik = A(i,k);
      a_il = A(i,l);
      A(i,k) = cos_*a_ik - sin_*a_il;
      A(k,i) = A(i,k);
      A(i,l) = cos_*a_il + sin_*a_ik;
      A(l,i) = A(i,l);
    }
//cout << n << endl;
    //Computing new eigenvectors
    double r_ik, r_il;
    r_ik = R(i,k);
    r_il = R(i,l);
    R(i,k) = cos_*r_ik - sin_*r_il;
    R(i,l) = cos_*r_il + sin_*r_ik;
  }
  return;
}


//Runs the rotation until we reached the max ite or reached the eps
void JacobiEigenSolve::Solve(){
  int iterations = 0;
  auto [max_value, row, col] = FindMaxEle();
  //cout << A << endl;

  while (max_value > eps || iterations < max_iterations ){
    //cout <<"Largest value: " << max_value << " På plass: " << "( " << col << ", " << row << ")" << endl;
    Rotate(row, col);
    //cout << "Reduced to " << A(row,col) << "\n" <<  endl;
    //cout << A << endl;
    auto[max_value_, row_, col_] = FindMaxEle();
    cout <<  "Kolonne" <<col_ << "row: "<< row_ << endl;
    max_value = max_value_; row = row_; col = col_; //Update variables outside the loop
    iterations ++;
  }
  A.clean(eps); //Remove elements smaller than eps.
  return;
}

// Prints A, used for checks
void JacobiEigenSolve::PrintA(){
  cout << "Matrix A: \n" << A << endl;
  cout << "Matrix R: \n" << R << endl;
  return;
}


//Tests 2-3 times if method finds correct value and postion
void JacobiEigenSolve::TestFindMaxEle(){
  arma_rng::set_seed_random();
  A = mat(4,4, arma::fill::randu);
  cout << "Find max value of this matrix:\n" << A << endl;
  //A = rand
  //Mat<double> C =
  //int i = index_max(A_test);
  //int j = index_min(A_test);
/*
  while(i == j){
    i = 2;
  }
  //index_max
  return;*/
}

//Tests if matrix is set up correctly initially
void JacobiEigenSolve::TestInitialize(){
  //Use this to in a test to compare
  vec X(n); X.fill(0);
  X(0) = 2; X(1) = -1; //Horfor gir dette index out of bounds error??
  mat B = toeplitz(X);

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      //cout << A(i,j) << B(i,j) << endl;
      if( fabs(A_test(i,j) - B(i,j)) > 1e-8 ){
        cout << "Error: A(i,j) - B(i,j) = " << A_test(i,j) - B(i,j) << endl;
      }
    }
  }
  return;
}

void JacobiEigenSolve::TestSolve(){
  for (int i =0; i <n; i++){
    for (int j = i+1; j<n; j++){
      if( i != j){
        assert(abs(A(i,j)) < eps );
      }
    }
  }
}
