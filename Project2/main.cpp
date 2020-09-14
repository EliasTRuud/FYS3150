#include "jacobiAlgo.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

int main(int argc, char const *argv[]) {

  JacobiEigenSolve jes;

  jes.Initialize(1.2, 2.3, 10, 2);
  jes.Solve();
  jes.PrintA();

}
