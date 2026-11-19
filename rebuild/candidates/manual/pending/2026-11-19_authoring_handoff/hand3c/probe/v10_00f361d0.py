def mk(order):
    def fn(s):
        L=s.split("\n")
        G0=[l for l in L if "C3DVector gradient" in l and "+ 1)" not in l]; G1=[l for l in L if "C3DVector gradient" in l and "+ 1)" in l]
        V0=[l for l in L if l.strip().startswith("float value") and l.split("value")[1][2]=="0"]
        V1=[l for l in L if l.strip().startswith("float value") and l.split("value")[1][2]=="1"]
        Z0=[l for l in L if "float result_z0" in l]; Z1=[l for l in L if "float result_z1" in l]
        allb=G0+G1+V0+V1+Z0+Z1; first=min(L.index(l) for l in allb)
        L=[l for l in L if l not in allb]
        blk={"G0":G0,"G1":G1,"V0":V0,"V1":V1,"Z0":Z0,"Z1":Z1}
        L=L[:first]+sum((blk[k] for k in order),[])+L[first:]
        return "\n".join(L)
    return fn
ORD={"S1":["G0","G1","V0","V1","Z0","Z1"],"S2":["G0","V0","G1","V1","Z0","Z1"],"S4":["G0","G1","V0","Z0","V1","Z1"],"S5":["G0","V0","G1","Z0","V1","Z1"]}
VARIANTS=[(k,mk(v)) for k,v in ORD.items()]
