from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

op_text = open('op2.txt','r')

op_vals = op_text.readlines()

for x in range(0,len(op_vals)):
    op_vals[x] = op_vals[x][:-1]

vals = []

mins = []

for i in range(len(op_vals)):
    mins.append(op_vals[i].split('|')[0].split(','))
    maxs = op_vals[i].split('|')[1].split(',')
    mins[i] = mins[i]+maxs

x = []
y = []
z = []

# mins[-1][-1] = '4'

for j in mins:
    print(j)
    x.append(float(j[0]))
    y.append(float(j[1]))
    z.append(float(j[2]))
    
    x.append(float(j[0]))
    y.append(float(j[4]))
    z.append(float(j[2]))

    x.append(float(j[3]))
    y.append(float(j[4]))
    z.append(float(j[2]))

    x.append(float(j[0]))
    y.append(float(j[4]))
    z.append(float(j[5]))

    x.append(float(j[0]))
    y.append(float(j[4]))
    z.append(float(j[2]))

    x.append(float(j[0]))
    y.append(float(j[4]))
    z.append(float(j[5]))

    x.append(float(j[3]))
    y.append(float(j[4]))
    z.append(float(j[5]))

    x.append(float(j[3]))
    y.append(float(j[1]))
    z.append(float(j[3]))



fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, c='r',s=0.1)

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()