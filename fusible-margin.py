# You are given two fuses. Each one burns for exactly one minute, but not uniformly, so one cannot 
# predict exactly how much of the fuse will be left after a given amount of time. You are allowed to 
# light one or more unlit ends of any fuse, but only at time t = 0 or when a fuse burns out completely. 
# How do you measure 45 seconds? 

# The solution is to light both ends of one fuse and one end of the other fuse at the same time. When
# the first fuse burns out completely, 30 seconds have passed — then light the remaining end of the 
# other fuse. Forty-five seconds will have passed when this fuse burns out.

# From this problem, a fast-growing function arises. This program is a python implementation.

import math
from fractions import Fraction

def denom(x):
    n = Fraction(str(x))
    return(n.denominator)

def f(x):
    if x < 0: return 0
    else:
        minv = math.inf
        b = x
        while b > (x - 1/2):
            a = f((2*x) - (b + 1))
            b = f((2*x) - (a + 1))
            if (a+b+1) < minv:
                minv = (a+b+1)/2
            b = b-1/denom(b)

def m(x):
    return f(x)-x

def m_1(x):
    return -(math.log(m(x),2))
