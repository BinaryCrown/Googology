import math
e=lambda l:(2*l[0]+1)*math.prod([2**i for i in l])
m=lambda l,i:m([int(k) for k in str(m(l,i-1))],i-1)if i else e(l)
x=m([3,5,7],5)
