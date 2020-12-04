#include "SIRS.hpp"

SIRS::SIRS(double S_, double I_, double a_, double b_, double c_){
  S = S_;
  I = I_;
  R = 0;
  a = a_;
  b = b_;
  c = c_;
}

inline double dS(double S_, double I_, double R_){
  return c*R_ - a*I_*S_/N
}
inline double dI(double S_, double I_, double R_){
  return a*I_*S_/N - b*I_
}
inline double dR(){
  return b*I - c*R
}

void SIRS::solve(){

}

void SIRS::rk4(){


}



//ODESolver
# Assume that self.t[0] corresponds to self.U0
self.u[0] = self.U0

# Time loop
for k in range(n-1):
    self.k = k
    self.u[k+1] = self.advance()
    if terminate(self.u, self.t, self.k+1):
        break  # terminate loop over k
return self.u[:k+2], self.t[:k+2]


class RungeKutta4(ODESolver):
    def advance(self):
        u, f, k, t = self.u, self.f, self.k, self.t
        dt = t[k+1] - t[k]
        dt2 = dt/2.0
        K1 = dt*f(u[k], t[k])
        K2 = dt*f(u[k] + 0.5*K1, t[k] + dt2)
        K3 = dt*f(u[k] + 0.5*K2, t[k] + dt2)
        K4 = dt*f(u[k] + K3, t[k] + dt)
        u_new = u[k] + (1/6.0)*(K1 + 2*K2 + 2*K3 + K4)
        return u_new




clas SolverSIRD:
  def __init__(self,problem,T_tot,dt):
      self.T_tot, self.dt = T_tot, dt
      self.total_pop = problem.get_population()
      self.problem = problem

    def solve(self, method=RungeKutta4):
        T_tot = self.T_tot
        dt = self.dt
        solver = method(self.problem)
        #print(len(self.problem.initial_condition))
        solver.set_initial_condition(self.problem.initial_condition)
        t = np.linspace(0,T_tot,int(T_tot/dt)+1)
        u, t = solver.solve(t, self.terminate)
        self.problem.solution(u, t)

if __name__ == '__main__':
    U0_bjorvin = [7000,300,0,0] # S0,I0,R0,D0
    region = Region("Bjorgvin",*U0_bjorvin)
    problem = ProblemSIRD(region,6.5E-5, 0.1/4 , 0.9/4)
    sol = SolverSIRD(problem,63,1)


    def __call__(self, u, t):
      S = -self.alpha(t)*u[0]*u[1]
      I = u[1]*u[0]*self.alpha(t)-self.beta*u[1]-self.gamma*u[1]
      R = self.beta*u[1]
      D = self.gamma*u[1]
      return S,I,R,D