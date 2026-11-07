import sys,re
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def calls(L):
    o=[]
    for l in L:
        p=l.split(None,2)
        if len(p)>2 and (p[2].startswith("call") or p[2].startswith("ret") or "jne" in p[2] and False):
            o.append(p[2])
    return o
import difflib
rc,bc=calls(r),calls(b)
for x in difflib.unified_diff(rc,bc,"retail","built",n=2,lineterm=""): print(x)
