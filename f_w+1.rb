f=->n{n.times{eval("n.times{"*n+"n+=1"+"}"*n)};n};f.call(64)
