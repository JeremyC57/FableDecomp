# ld.py <grade-output> : straight line diff (retail vs ours) of q.py's two listings, addresses and relocation operands masked
import sys,re,difflib
L=open(sys.argv[1]).read().splitlines()
starts=[i for i,l in enumerate(L) if re.match(r'^[0-9a-f]{8}  ',l)]
# the two listings start where the address equals the first listing's first address
first=L[starts[0]].split()[0]; idx=[i for i in starts if L[i].split()[0]==first]
a0=idx[0]; b0=idx[1] if len(idx)>1 else None
def grab(i):
    out=[]
    while i<len(L) and re.match(r'^[0-9a-f]{8}  ',L[i]):
        ins=L[i].split(None,2)[2] if len(L[i].split(None,2))>2 else ''
        ins=re.sub(r'0x[0-9a-f]{5,}','REL',ins); ins=re.sub(r'\[0\]','[REL]',ins); ins=re.sub(r'(call|j[a-z]+) +\S+',r'\1 T',ins)
        out.append(re.sub(r'\s+',' ',ins).strip()); i+=1
    return out
A=grab(a0); B=grab(b0)
ops=[o for o in difflib.SequenceMatcher(None,A,B,autojunk=False).get_opcodes() if o[0]!='equal']
n=sum(max(i2-i1,j2-j1) for _,i1,i2,j1,j2 in ops)
print("lines %d v %d, differing (masked) %d"%(len(A),len(B),n))
for t,i1,i2,j1,j2 in ops[:int(sys.argv[2]) if len(sys.argv)>2 else 12]:
    print("@%d %s"%(i1,t)); [print("  < "+x) for x in A[i1:i2]]; [print("  > "+x) for x in B[j1:j2]]
