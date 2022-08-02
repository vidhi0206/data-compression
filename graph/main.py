import matplotlib.pyplot as plt
import numpy as np

BVP = [2.80, 2.80, 2.80]
EDA = [12.59, 12.56, 12.20]
ACM = [5.44, 5.42, 5.42]
GYS = [7.82, 7.85, 7.82]
GAS = [13.74, 13.84, 13.63]
GACTIVE= [4.17, 4.18, 4.14]

TAU = [5, 7, 9]

plt.plot(TAU, BVP, label= "BVP")
plt.plot(TAU, EDA, label= "EDA")
plt.plot(TAU, ACM, label= "ACM")
plt.plot(TAU, GYS, label= "GYS")
plt.plot(TAU, GAS, label= "GAS")
plt.plot(TAU, GACTIVE, label= "GACTIVE")
plt.xticks(np.arange(5, 9+1, 2.0))
plt.xlabel("Tau")
plt.ylabel("Compression Ratio")
plt.title('Block Size = 16')
plt.legend(loc="lower right")
plt.show()
