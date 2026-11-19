# cls.py <g_file> : per fcos site in the built listing, classify the GetValue combine: S (sin-factored: Cc then +Cb), C (cos-factored: Cc then +Ca)
import sys,re
t=open(sys.argv[1]).read().split("--- built")
for name,part in (("retail",t[0]),("built",t[1] if len(t)>1 else "")):
    L=[l for l in part.split("\n") if re.match(r"^[0-9a-f]{8} ",l)]
    out=[]
    for i,l in enumerate(L):
        if " fcos" in l:
            k="?"
            for j in range(i,min(i+25,len(L))):
                if re.search(r"fmul\s+dword ptr \[\w+ \+ 8\]",L[j]):
                    nx=L[j+1]
                    k="S" if re.search(r"fadd\s+dword ptr \[\w+ \+ 4\]",nx) else ("C" if re.search(r"fadd\s+dword ptr \[\w+\]",nx) else "?")
                    break
            out.append(l.split()[0]+":"+k)
    print(name," ".join(out))
