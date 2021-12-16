def iterate(f,a,b):
    if b == 1:
        return f(a)
    else:
        return iterate(f,f(a),b-1)

def sequence(a,b,c):
    return iterate(b,a+(a%(c+1)),a+(a%(c+1))) % (c+iterate(b,a,a)+1)

x = lambda n:n**2

for k in range(10000):
    print(sequence(2,x,k))
