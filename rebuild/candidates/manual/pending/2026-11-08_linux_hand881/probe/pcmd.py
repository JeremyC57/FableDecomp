import sys,re,difflib
# pcmd.py <out> : per-case instruction diff size (addresses/call targets masked); -v N prints case N diff
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def norm(L):
    out=[]
    for l in L:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=re.sub(r'0x[0-9a-f]{5,8}\b','A',p[2]); out.append((p[0],ins))
    return out
R=norm(r); B=norm(b)
def split(L):
    cs=[[]]
    for a,i in L:
        if re.search(r'mov e.x, dword ptr \[ebp \+ 0x14\]',i): cs.append([])
        cs[-1].append((a,i))
    return cs
RC=split(R); BC=split(B)
v=int(sys.argv[3]) if len(sys.argv)>3 and sys.argv[2]=='-v' else None
for n,(x,y) in enumerate(zip(RC,BC)):
    a=[i for _,i in x]; c=[i for _,i in y]
    sm=difflib.SequenceMatcher(None,a,c,autojunk=False)
    md=sum(max(i2-i1,j2-j1) for tag,i1,i2,j1,j2 in sm.get_opcodes() if tag!='equal')
    print('part',n,'md',md,'len',len(a),len(c))
    if v==n:
        for tag,i1,i2,j1,j2 in sm.get_opcodes():
            if tag=='equal': continue
            for k in range(max(i2-i1,j2-j1)):
                ra=x[i1+k] if i1+k<i2 else ('',''); ba=y[j1+k] if j1+k<j2 else ('','')
                print('  %s %-45s | %s %s'%(ra[0],ra[1][:45],ba[0],ba[1][:45]))
