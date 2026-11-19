import itertools
def paren_keys(keys, inner=False):
    def fn(s):
        L=s.split("\n")
        for i,l in enumerate(L):
            for key in keys:
                if l.strip().startswith(key):
                    a,b=l.split(" = ",1); L[i]=a+" = ("+b[:-1]+");"
        s="\n".join(L)
        if inner: s=s.replace("Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty)","Interp((Interp(value000, value100, tx)), (Interp(value010, value110, tx)), ty)")
        return s
    return fn
vk=["float value000","float value100","float value010","float value110"]
VARIANTS=[]
for n in range(1,5):
    for combo in itertools.combinations(vk,n):
        for inner in (0,1):
            VARIANTS.append(("_".join(c[-3:] for c in combo)+f"_in{inner}", paren_keys(combo,inner)))
for extra in (["float result_z0"],["float result_z0","float value000"]):
    for inner in (0,1): VARIANTS.append(("+".join(e.split()[-1] for e in extra)+f"_in{inner}", paren_keys(extra,inner)))
