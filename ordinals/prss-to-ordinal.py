# Converts primitive sequences to ordinals

def lexicoless(l: list, r: list) -> bool:
    if l == []: return r != []
    if r == []: return False
    else: return l[0] < r[0] and (l[0] == r[0] or lexicoless(l[1:], r[1:]))

def isstd(l: list) -> bool:
    if len(l) != 0:
        if l[0] != 0:
            return False
        for k in range(len(l)):
            if not type(k) is int: raise "Invalid."
            if k < len(l)-1 and l[k+1]-l[k] > 1: return False
            for j in range(k,len(l)):
                if l[j] == l[k] and lexicoless(l[k:],l[j:]): return False
        return True
    return True

def basicToOrdinal(l: list) -> str:
    if not isstd(l):
        raise "Not standard."
    
    if len(l) == 0: return "0"
    
    if any([k == 0 for k in l[1:]]):
        for k in range(len(l)):
            if l[k] == 0: return basicToOrdinal(l[:(k+1)]) + "+" + basicToOrdinal(l[(k+1):])
    return "w^(%s)" % basicToOrdinal([k-1 for k in l[1:]])

def formatOrdinal(l: list) -> str:
    return basicToOrdinal(l).replace("w^(0)", "1").replace("w^(1)", "w").replace("w^(w)", "w^w")