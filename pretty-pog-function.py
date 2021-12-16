def encode(l):
    o = 2*l[0]+1
    for m in range(1,len(l)):
        o *= 2**l[m]
    return o

def multi_encode(l,i):
    if i==1:
        return encode(l)
    else:
        return multi_encode([int(k) for k in [char for char in str(multi_encode(l,i-1))]],i-1)

def PPF(k):
    return multi_encode([3,5,7],k+1)

print(PPF(5))
