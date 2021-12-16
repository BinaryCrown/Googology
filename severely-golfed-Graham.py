a=lambda b,c:3if b<2else(a(a(b-1,c),c-1)if c else 3*b)
G=lambda k:a(3,G(k-1))if k else 4
print(G(64))
