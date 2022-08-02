import numpy as np
import matplotlib.pyplot as plt

barWidth = 0.15
CA = [2.74, 5.19, 10.75, 12.47]
SZ = [4.39, 11.00, 24.79, 23.99]
version2=[7.03, 38.77, 100.93, 34.94]
LFZip=[9.13, 12.71, 28.77, 17.98 ]

br1 = np.arange(len(CA))
br2 = [x + barWidth for x in br1]
br3 = [x + barWidth for x in br2]
br4 = [x + barWidth for x in br3]

plt.bar(br1, CA, color ='g', width = barWidth,
                edgecolor ='grey', label ='CA')
plt.bar(br2, SZ, color ='violet', width = barWidth,
        edgecolor ='grey', label ='SZ')
plt.bar(br3, version2, color ='cornflowerblue', width = barWidth,
        edgecolor ='grey', label ='version2')
plt.bar(br4, LFZip, color ='goldenrod', width = barWidth,
        edgecolor ='grey', label ='LFZip')


plt.xlabel('Datasets', fontweight='bold', fontsize=15)
plt.ylabel('Compression Ratio', fontweight='bold', fontsize=15)
plt.xticks([r + barWidth for r in range(len(CA))],
           ["BVP", "ACM", "gys", "gactive"])

plt.legend()
plt.show()