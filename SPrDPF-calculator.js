const zero = Symbol("");
const add = Symbol("");
const psi = Symbol("");
const _zero = new Term(zero, zero, zero);
const one = new Term(psi, 0, _zero);
const omega = new Term(psi, 0, one);

class Term {              // RT and RPT
  constructor(type, a, b) {
    this.type = type;
    this.a = a;
    this.b = b;
  }
  equal(lhs, rhs) {
    if(typeof(lhs) == "object" && typeof(rhs) == "object") {return ((lhs.type == rhs.type) && equal(lhs.a, rhs.a) && equal(lhs.b, rhs.b));}
    if(typeof(lhs) != "object" && typeof(rhs) != "object") {return (lhs == rhs);}
    else {return null;}
  }
  lessequal(lhs, rhs) {
    return (equal(lhs, rhs) || compare(lhs, rhs))
  }
  dollar(n) {
    start = _zero;
    for(let i = 0; i < n; i++) {
      start = new Term(add, start, one);
    }
    return start;
  }
  compare(lhs, rhs) {
    var s = lhs.type;
    var t = rhs.type;
    if(t == zero) {return false;}
    if(s == zero) {return (t != zero);}
    else {
      if(s == add) {
        if(t == add) {return (compare(lhs.a, rhs.a) || (equal(lhs.a, rhs.a) && compare(lhs.b, rhs.b)));}
        if(t == psi) {return compare(lhs.a, rhs);}
        else {return null;}
      }
      if(s == psi) {
        if(t == add) {return lessequal(lhs, rhs.a);}
        if(t == psi) {return (compare(lhs.a, rhs.a) || (equal(lhs.a, rhs.a) && compare(lhs.b, rhs.b)));}
        else {return null;}
      }
      else {return null;}
    }
  }
  dod(x) {
    if(x.type == zero) {return 0;}
    if(x.type == add) {return dod(x.b);}
    if(x.type == psi) {return dod(x.b)+1;}
    else {return null;}
  }
  cut(x,y) {
    if(y.type == zero) {return x;}
    else {
      if(x.type == add) {return cut(x.b,y);}
      if(x.type == psi) {return cut(x.b,y-1);}
      else {return null;}
    }
  }
  oplus(x,y,u) {
    if(x.type == zero) {return _zero;}
    if(x.type == add) {return new Term(add, oplus(x.a,y,u), oplus(x.b,y,u));}
    if(x.type == psi) {
      if(u < x.a) {return new Term(psi, x.a+t, oplus(x.b,t,u));}
      if(u >= x.a) {return x;}
      else {return null;}
    }
    else {return null;}
  }
  asd(x,y,u,v,w) {
    if(x.type == zero) {return _zero;}
    if(x.type == add) {return new Term(add, asd(x.a,y,u,v,w), asd(x.b,y,u,v,w));}
    if(x.type == psi) {
      if(v == 1) {return oplus(x,y+u,w);}
      else {return oplus(asd(x,y,u,v-1,w),y+u*v,w);}
    }
    else {return null;}
  }
  con(x,y) {
    if(x.type == zero) {return y;}
    if(x.type == add) {return new Term(add, x.a, con(x.b,t));}
    if(x.type == psi) {return new Term(psi, x.a, con(x.b,t));}
    else {return null;}
  }
  cod(x) {
    if(x.type == zero) {return _zero;}
    if(x.type == add) {return cod(x.b);}
    if(x.type == psi) {
      if(cod(x.b) == _zero) {return x;}
      if(cod(x.b) == one) {return omega;}
      else {
        if(compare(cod(x.b), x)) {return cod(x.b);}
        else {
          if(cod(cod(x.b).b) == _zero) {
            if(cod(x.b).a-x.a <= 1) {return omega;}
            else {return x;}
          }
          else {return cod(x.b);}
        }
      }
    }
    else {return null;}
  }
  dom(x) {
    if(x.type == zero) {return _zero;}
    if(x.type == add) {return dom(x.b);}
    if(x.type == psi) {
      if(dom(x.b) == _zero) {return x;}
      if(dom(x.b) == one) {return omega;}
      else {
        if(compare(dom(x.b), x)) {return dom(x.b);}
        else {
          if(dom(dom(x.b).b) == 0) {
            if(cod(x) == omega) {return omega;}
            else {return x;}
          }
          else {
            if(dom(x.b).a - x.a < cod(cod(x.b).b).a - cod(x.b).a) {return omega;}
            else {return x;}
          }
        }
      }
    }
    else {return null;}
  }
  static FS(x,y) {
    if(x.type == zero) {return _zero;}
    if(x.type == add) {
      if(FS(x.b,y) == _zero) {return x.a;}
      else {return new Term(add, x.a, FS(x.b,y));}
    }
    if(x.type == psi) {
      if(dom(x.b) == _zero) {return y;}
      if(dom(x.b) == one) {
        if(y == new Term(add, FS(y,0), one)) {return new Term(add, FS(x,FS(y,0)), FS(x,one));}
        else {return new Term(psi, x.a, FS(x.b, _zero));}
      }
      else {
        if(compare(dom(x.b),x)) {return new Term(psi, x.a, FS(x.b,y));}
        else {
          if(dom(dom(x.b).b) == _zero) {
            if(cod(x) == omega) {
              if(x.type == add && compare(x,omega)) {
                for(let i = 0; true; i++) {
                  if(x == dollar(i)) {break;}
                }
                if(FS(x,FS(y,0)).a == x.a) {return new Term(psi, x.a, FS(x.b, new Term(psi, FS(c,_zero), FS(x,FS(y,0)).b)));}
                else {return new Term(psi, x.a, FS(x.b, new Term(psi, FS(c,_zero),_zero)));}
              }
              else {return new Term(psi, x.a, FS(x.b, new Term(psi, FS(c,_zero),_zero)));}
            }
            else {return new Term(psi, x.a, FS(x.b, y));}
          }
          else {
            if(dom(x.a).a - x.a < cod(cod(b).b).a - cod(b).a) {
              if(x.type == add && compare(x,omega)) {
                for(let i = 0; true; i++) {
                  if(x == dollar(i)) {break;}
                }
                if(FS(x,FS(y,0)).a == x.a && FS(x,0).a == x.a) {return new Term(psi, a, con(FS(x,FS(y,0)).b, asd(cut(FS(x,0).b, dod(x) - dod(dom(x.b))), (cod(cod(x.b).b).a - x.a - 1) - (dom(x.b).a - x.a), (cod(cod(x.b).b).a - cod(x.b).a) - (dom(x.b).a - x.a) - 1, i, x.a)))}
                  else {return new Term(psi, a, FS(b, new Term(psi, FS(cod(cod(x.b).b).a,0), 0)));}
              }
              else {return new Term(psi, a, FS(b, new Term(psi, FS(cod(cod(x.b).b).a,0), 0)));}
            }
            else {return new Term(psi, a, FS(b, x))}
          }
        }
      }
    }
    else {return null;}
  }
}

function fgh(s,m,n) {
  if(m == 0) {return n;}
  if(m == 1) {
    if(dom(s) == _zero) {return n+1;}
    if(dom(s) == one) {return f(FS(s,0),n,n);}
    else {return f(FS(s,dollar(n)),1,n)}
  }
  else {return fgh(s,1,fgh(s,m-1,n))}
}

function F(n) {
  return fgh(new Term(psi, 0, new Term(psi, 1, new Term(psi, n, 0))),1,n);
}

function repeat(f,i,t) {
  if(t == 0) {return i;}
  else {return f(repeat(f,i,t-1));}
}

Console.log("The subspecies primitive difference psi number is: " + repeat(a => F(a), 10**100, 10**100));
