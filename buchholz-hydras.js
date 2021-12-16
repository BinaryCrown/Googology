function l(t){return t.length-1;}
function m(t){return t[l(t)]}
function s(t){t.splice(m(t),1);}
var n=1;

function e(t){
    if(m(t)==0){
        s(t);
        if(l(t)>1){
            for(let i=0;i<n;i++){t.push(t[l(t)-1]);}
        }
    }
    if(m(t)>0&&typeof t=="number"){
        for(let i=l(t); t[i]<m(t); i--){}
        var s;
        for(let j = i; j <= l(t); j++){s.push(t[j]);}
        var sa = s;
        sa[j]=sa[i]-1;
        sa[l(sa)]=0;
        s(t);
        for(let k = 0; k <= l(sa); k++){t.push(sa[k]);}
    }
    if(m(t)=="w"){t[l(t)]=n+1;}
    n += 1;
    return t;
    }

function b(n){
    i = ["+",0];
    for(let l = 0; l < n-2; l++){i.push("w")}
    while(l(i)>=0){i=e(i);}
    return n;
}

var k = b(b(b(3)));
