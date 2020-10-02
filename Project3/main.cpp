#include "solver.hpp"
#include "planet.hpp"


int main(int argc, char const *argv[]) {

  Planet sun(1.989e30, vec p(3) = {0, 0, 0}, vec v(3) = {0, 0, 0});
  Planet earth(5.972e24, vec p(3) = {1, 0, 0}, vec v(3) = {0, 1, 0});
  sun.distanceOther(earth);
  //Planet jupyter =planet(1.898E27 kg, vec p(3) = {2, 0, 0}, vec v(3) = {0, 1, 0});

  return 0;
}
