import matplotlib.pyplot as plt
import numpy as np
import sys
import pandas as pd

n = sys.argv[1]
V = sys.argv[2]
omega = sys.argv[4]

if V == "V0":
    rho_max = 1
else:
    rho_max = float(sys.argv[3])

if V == "V2":
    filename = "_n_" + n + "_" + V + "_w_%.6f.txt" %(float(omega))
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

n_eigval = 3 #Number of eigenvectors to plot
h = rho_max/(n+1)
x = np.asarray([i*h for i in range(n)])
plt.clf()
for i in range(n_eigval):
    plt.plot(x,num_eigvec[num_sort[i]], label="Num eigenvalue = %.4f" %(num_eigval[num_sort[i]]))
    plt.plot(x,arma_eigvec[arma_sort[i]], label="Arma eigenvalue = %.4f" %(arma_eigval[arma_sort[i]]))

plt.xlabel("rho")
plt.ylabel("u(rho)")
plt.grid()
plt.legend()

if V == "V2":
    plt.title("Eigenvectors for %s, omega=%.3f" %(V, float(omega)))
    #plt.savefig("eigplot_%s_n_%i_w_%.3f.PNG" %(V,n, float(omega)))
    #plt.savefig("eigplot_%s_n_%i_w_%.3f.PDF" %(V,n, float(omega)))
    plt.savefig("plots/%s/eigplot_%s_n_%i_w_%.3f.PDF" %(V,V,n, float(omega)))
else:
    plt.title("Eigenvectors for %s" %(V))
    #plt.savefig("eigplots_%s_n_%i.PNG" %(V,n))
    #plt.savefig("eigplots_%s_n_%i.PDF" %(V,n))
    plt.savefig("plots/%s/eigplot_%s_n_%i_w_%.3f.PDF" %(V,V,n, float(omega)))

plt.clf()
#-------------
#Plot u^2
for i in range(n_eigval):
    plt.plot(x,num_eigvec[num_sort[i]]**2, label="Num eigenvalue = %.4f" %(num_eigval[num_sort[i]]))

plt.xlabel("rho")
plt.ylabel("u(rho)^2")
plt.legend()

if V == "V2":
    plt.title("Squared Eigenvectors for %s, omega=%.3f" %(V, float(omega)))
    #plt.savefig("plots/%s/Sqr_eigplot_%s_n_%i_w_%.3f.PNG" %(V,V,n, float(omega)))
    plt.savefig("plots/%s/Sqr_eigplot_%s_n_%i_w_%.3f.PDF" %(V,V,n, float(omega)))
else:
    plt.title("Squared Eigenvectors for %s" %(V))
    plt.savefig("plots/%s/Sqr_eigplots_%s_n_%i.PNG" %(V,V,n))
    plt.savefig("plots/%s/Sqr_eigplots_%s_n_%i.PDF" %(V,V,n))


def time_results():
    """
    Reads TimeTable.csv and plots the CPU run time.
    Unncomment writeTimeResults(omega) in main.cpp to
    produce the TimeTable.csv file.
    """

    with open("results/V0/TimeTable.csv", 'r') as f:
        lines = f.readlines()
        size = int(len(lines)/10)
        t_arma = np.zeros(size)
        t_class = np.zeros(size)

        for line in lines:
            data = line.split(",")
            t_arma[int(int(data[0])/10)-1] += float(data[1])
            t_class[int(int(data[0])/10)-1] += float(data[2])
    t_arma /= 10
    t_class /= 10
    N = np.linspace(10,n,size)
    z = np.polyfit(N[4:], t_class[4:], 2)
    plt.clf()
    plt.plot(N[4:], z[0]*N[4:]**2+z[1]*N[4:]+z[2], ':', label="2nd degree fit")
    plt.plot(N, t_arma, label='Armadillo')
    plt.plot(N, t_class, 'o', label='Jacobi Algorithm')
    plt.legend()
    plt.xlabel("n")
    plt.ylabel("Time [s]")
    plt.title("Run times for eigenpair solvers")
    plt.grid()

    plt.savefig("plots/V0/times_plot_V0.pdf")




def analyicalEigenvalues():
    #Calculating analytical eigenvalues for V0 and V1
    ana_eigvalV0 = np.zeros(n)
    ana_eigvalV1 = np.zeros(n)
    ana_eigvalV2 = np.zeros(3)
    ana_eigvalV2[0] = 0.6259*2 #omega = 1/4
    ana_eigvalV2[1] = 0.1750*2 #omega = 1/20
    ana_eigvalV2[2] = 0.0822*2 #omega = 1/54.7386

    h = 1/(n+1)
    d = 2/(h**2)
    a = -1/(h**2)
    print("\n")
    for i in range(n):
        ana_eigvalV0[i] = d + 2*a*np.cos((i+1)*np.pi/(n+1))
        ana_eigvalV1[i] = 3.0 + 4*i
    print("Analytic eigenvalues: V0\n", ana_eigvalV0)
    print("Analytic eigenvalues: V1 \n", ana_eigvalV1)
    print("Analytic eigenvalues: V2 \n", ana_eigvalV2)

def plotTimeV0vsN():
    """
    Reads TimeTable.csv and the number of iterations vs n.
    Unncomment writeTimeResults(omega) in main.cpp to
    produce the TimeTable.csv file.
    """
    df = pd.read_csv("results/V0/TimeTable.csv", names = ['n','timeA', 'timeC', 'ite'])
    df = df.drop_duplicates(subset='n', keep='first')
    n1 = df['n'].to_numpy()
    ite = df['ite'].to_numpy()
    size = int(len(n1)/10)
    n_values = np.linspace(10, n, size)

    z = np.polyfit(n1, ite, 2)

    for i in range(n_values.size):
        n_values[i] = z[0]*n_values[i]**2 #- 1.55*n_values[i]

    plt.clf()
    plt.plot(n1, ite/1000, label = f"Iterations")
    #plt.plot(n_values, label = "func") #to see how the single x affected the curve, which is basically none
    plt.plot(n1, z[0]*n1**2/1000, ':', label= "2nd degree fit, %.3fnˆ2" %(z[0]))
    plt.legend()
    plt.title("Nr of Iterations vs N")
    plt.ylabel("Iterations/1000")
    plt.xlabel("n-value")
    plt.grid()
    plt.savefig("plots/V0/iterations.pdf", dpi = 200)
    plt.show()

plotTimeV0vsN()
#time_results()
