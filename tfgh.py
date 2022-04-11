class Term:
    def __init__(self, t: int, v: list) -> None:
        self.t = t
        self.v = v
    
    def isfinite(self) -> bool:
        if self.t == 0: return True
        if self.t == 1: return True
        if self.t == 2: return False
        if self.t == 3: return self.v[1].isfinite() and self.v[2].isfinite()
    
    def __eq__(self, o: object) -> bool:
        return self.t == o.t and self.v == o.v
    
    def __le__(self, o: object) -> bool:
        if self == o: return True
        else: return self < o
    
    def __lt__(self, o: object) -> bool:
        if self.t == 0: return o.t != 0
        elif o.t == 0: return False
        elif self.t == 1:
            if o.t == 1: return False
            if o.t == 2: return True
            else: return o.v[2].t != 0
        elif self.t == 2:
            if o.t == 1: return False
            if o.t == 2: return False
            else: return not o.isfinite() and self != o
        elif self.t == 3:
            if o.t == 1: return (self.v[0].t != 0 and self.v[1].t != 0) or (self.v[0].t == 0 and self.v[1].t == 0)
            if o.t == 2: return self.isfinite()
            else: pass