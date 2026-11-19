import itertools
def blocks(s):
    L=s.split("\n")
    get=lambda pref: [l for l in L if l.strip().startswith(pref)]
    return {"A":get("float v0_"),"B":get("float v1_"),"C":[l for l in L if l.strip().startswith("float t") and "SCurve" in l],"D":get("unsigned long b")}
def mk(order):
    def fn(s):
        bl=blocks(s); L=s.split("\n")
        allb=sum(bl.values(),[])
        first=min(L.index(l) for l in allb)
        L=[l for l in L if l not in allb]
        new=sum((bl[k] for k in order),[])
        L=L[:first]+new+L[first:]
        return "\n".join(L)
    return fn
orders=[o for o in itertools.permutations("ABCD") if o.index("A")<o.index("B") and o.index("A")<o.index("C")]
VARIANTS=[("".join(o),mk(o)) for o in orders]
