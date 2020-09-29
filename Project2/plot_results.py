import matplotlib.pyplot as plt
import numpy as np
import sys
n = sys.argv[1]
V = sys.argv[2]
omega = sys.argv[4]

if V == "V1":
    rho_max = 1
else:
    rho_max = float(sys.argv[3])

if V == "V2":
    filename = "_n_" + n + "_" + V + "_w_" + omega + ".txt"
else:
    filename = "_n_" + n + "_" + V + ".txt"

n = int(n)


def read_results(filename): #Read result files
    file = open(filename, 'r')
    eigval = np.asarray(file.readline().split(),dtype=float)
    eigvec = np.zeros((n,n))

    lines = file.readlines()
    i = 0
    for line in lines:
        vals = line.split()
        for j in range(n):
            eigvec[j][i] = float(vals[j])
        i += 1

    sorted = np.argsort(eigval)
    return eigval, eigvec, sorted

#Fetching results
num_eigval, num_eigvec, num_sort = read_results("numerical" + filename)
arma_eigval, arma_eigvec, arma_sort = read_results("armadillo" + filename)

n_eigval = 1 #Number of eigenvectors to plot
h = rho_max/(n+1)
x = np.asarray([i*h for i in range(n)])
for i in range(n_eigval):
    plt.plot(x,num_eigvec[num_sort[i]], label="Num eigenvalue = %.4f" %(num_eigval[num_sort[i]]))
    plt.plot(x,arma_eigvec[arma_sort[i]], label="Arma eigenvalue = %.4f" %(arma_eigval[arma_sort[i]]))

plt.legend()
if V == "V2":
    plt.title("Eigenvectors for %s, %.2f" %(V, omega))
    plt.savefig("eigplot_%s_n_%i_w_%.2f.PNG" %(V,n, omega))
else:
    plt.title("Eigenvectors for %s" %(V))
    plt.savefig("eigplots_%s_n_%i_plot.PNG" %(V,n))

plt.show()