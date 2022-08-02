import matplotlib.pyplot as plt
import numpy as np

version1=[13.73974706, 12.89455324, 11.23848511]
version2=[13.80479479, 15.69397719, 22.07871751]

block_size= [ 16, 32, 64]

plt.plot(block_size, version1, label= "GKDD-version1")
plt.plot(block_size, version2, label= "GKDD-version2")

plt.xticks([16, 32, 64])
plt.xlabel("Block Size")
plt.ylabel("Compression Ratio")
plt.title('gas dataset')
plt.legend(loc="upper left")
plt.show()