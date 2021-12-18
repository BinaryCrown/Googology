def e(l):
    o=2*l[0]+1
    for m in range(1,len(l)):o*=2**l[m]
    return o
def m(l,i):
    if i:return multi_encode([int(k) for k in [char for char in str(multi_encode(l,i-1))]],i-1)
    else:return e(l)
def PPF(k):return m([3,5,7],k)
print(PPF(5))
