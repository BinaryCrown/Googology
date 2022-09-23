from typing import Callable

class Notation:
    def __init__(self, terms: Callable, limterms: Callable, comparison: Callable, expansion: Callable, mv):
        self.terms = terms
        self.limterms = limterms
        self.comparison = comparison
        self.FSrule = expansion
        self.min = mv
        self.pred = lambda x: expansion(x, 0)
    
def HH(N: Notation, s, n: int) -> int:
    if not N.terms(s):
        raise "Not valid term w.r.t. notation"
    else:
        if s == N.min:
            return n
        if not N.limterms(s):
            return HH(N, N.pred(s), n+1)
        else:
            return HH(N, N.FSrule(s, n), n)

def isNA(t) -> bool:
    if isinstance(t, list):
        for i in t:
            if not isinstance(i,int):
                return False
        return True

def isLNA(t):
    return isNA(t) and t[-1] == 0 and len(t) > 1

def CNFcomparison(s, t):
    if not isNA(s) and isNA(t):
        raise "Attempting to compare non-CNF terms via CNF algorithm"
    else:
        if s == 0:
            return True
        else:
            if len(s) < len(t):
                return True
            if len(s) > len(t):
                return False
            if s[0] < t[0]:
                return True
            if s[0] == t[0]:
                return CNFcomparison(s[1:], t[1:])
            return False

def CNFexpansion(s, n):
    if s[-1] > 0:
        s[-1] -= 1
        return s
    if s[-1] == 0:
        ind = bit = None
        for i in range(1,len(s)+1):
            ind = len(s)-i
            bit = s[ind]
            if bit > 0:
                break
        if ind == None:
            return [0]
        s[ind] -= 1
        s[ind+1] += n
        return s

CNF = Notation(isNA, isLNA, CNFcomparison, CNFexpansion, [0])

print(HH(CNF, [1,0], 3))