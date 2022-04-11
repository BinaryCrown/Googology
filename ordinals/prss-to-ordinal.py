# Converts primitive sequences to ordinals

def basicToOrdinal(l: list) -> str:
    if len(l) == 0: return "0"
    
    if any([k == 0 for k in l[1:]]):
        for k in range(len(l)):
            if l[k] == 0: return basicToOrdinal(l[:(k+1)]) + "+" + basicToOrdinal(l[(k+1):])
    return "w^(%s)" % basicToOrdinal([k-1 for k in l[1:]])

def formatOrdinal(l: list) -> str:
    return basicToOrdinal(l).replace("w^(0)", "1").replace("w^(1)", "w").replace("w^(w)", "w^w")