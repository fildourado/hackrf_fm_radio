import numpy as np
import sys
import matplotlib.pyplot as plt
import math


def main():

    path = "/home/fdourado/github_projects/hackrf_fm_radio/"
    filename = "out.txt"

    filepath = path+filename

    file = open(filepath, "r")

    x = []
    t = []

    f = 1.0
    fs = 16e3
    N = 32000 / 2 # number of samples

    i = 0
    for line in file:
        t.append( i/fs )
        x.append(float(line))
        i += 1

    #for i in range(N):
    #    t.append( i/fs )
    #    x.append( math.sin(2*math.pi*f*(i/fs)) )


    plt.plot(t,x)
    plt.show()

if __name__ == "__main__":
    main()