#include "planet.hpp"

using namespace std;
using namespace arma;

Planet::Planet(double m, vec position, vec velocity){
  mass = m;
  pos = position;
  vel = velocity;
}



double Planet::distanceOther(){
  vec dr = pos - otherPlanet.pos;
  return sqrt(dr(0)*dr(0) + dr(1)*dr(1) + dr(2)*dr(2));
}

Planet::gravitationalForce(Planet otherPlanet){
  double r = distanceOther(Planet otherPlanet);
  vec Fg = this->G * this->mass * otherPlanet.mass / (r * r);
  return Fg
}
