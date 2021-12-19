"""

ASM (Avalanche Sequence Method) is one of the algorithms of the DEWDROP suite.

It uses three hardcoded arrays (x, y, z) containing some numbers.

"""

import random

x = [2, 3, 4, 6, 7, 8, 10, 11, 12, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134]
y = [1, 5, 9, 13, 15, 16, 17, 19, 20, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 97]
z = [14, 18, 21, 22, 23, 24, 25, 26, 27]
k = input("Message: ")
m = [ord(str(c)) for c in k]
r = random.randint(0, 8)
c = [chr(ord(str(l)) * x[y[z[r]]]) for l in m]
print("The message is: " + str(m))
print("The random index is: " + str(r))
print("The ciphertext is: " + str(c))

"""
v = c // x[y[z[r]]]
print("After decryption, we get: " + str(v))
"""
