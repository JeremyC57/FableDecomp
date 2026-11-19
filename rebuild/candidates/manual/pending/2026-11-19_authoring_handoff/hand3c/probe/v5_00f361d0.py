import itertools
def mk(gorder, vorder, inter, which):
    def fn(s):
        L=s.split("\n")
        G={l.split("gradient")[1][:3]:l for l in L if l.strip().startswith("C3DVector gradient")}
        V={l.split("value")[1][:3]:l for l in L if l.strip().startswith("float value")}
        keys0=["000","100","010","110"]; keys1=["001","101","011","111"]
        for grp in ((keys0,0),(keys1,1)):
            ks,gi=grp
            if which not in (gi,2): continue
            mp={"a":ks[0],"b":ks[1],"c":ks[2],"d":ks[3]}
            gl=[G[mp[c]] for c in gorder]; vl=[V[mp[c]] for c in vorder]
            first=min(L.index(G[k]) for k in ks)
            for k in ks: L.remove(G[k]); L.remove(V[k])
            new=sum(([g,v] for g,v in zip(gl,vl)),[]) if inter else gl+vl
            L=L[:first]+new+L[first:]
        return "\n".join(L)
    return fn
VARIANTS=[]
for go in ("abcd","acbd"):
    for vo in ("abcd","acbd"):
        for inter in (0,1):
            if inter and go!=vo: continue
            for which in (0,1,2):
                VARIANTS.append((f"g{go}_v{vo}_i{inter}_w{which}", mk(go,vo,inter,which)))
