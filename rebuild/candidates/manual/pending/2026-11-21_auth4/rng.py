import sys,re
t=open(sys.argv[1]).read(); lo=int(sys.argv[2],16); hi=int(sys.argv[3],16)
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def g(L):
    o=[]
    for l in L:
        p=l.split(None,2)
        if len(p)==3:
            try: a=int(p[0],16)
            except: continue
            if lo<=a<=hi: o.append("%s %s"%(p[0][-4:],p[2]))
    return o
R=g(r);B=g(b)
for i in range(max(len(R),len(B))):
    print("%-48s | %s"%(R[i] if i<len(R) else "", B[i] if i<len(B) else ""))
