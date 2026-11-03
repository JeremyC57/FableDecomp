# slotmap.py <q.py output> [base]: map retail frame slots to built frame slots over aligned instructions (offsets relative to the esp after the prologue pushes)
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n'); base=int(sys.argv[2],16) if len(sys.argv)>2 else 0x10
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def norm(lines):
    depth=0; out=[]
    for l in lines:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2]
        n=re.sub(r'\[esp \+ 0x([0-9a-f]+)\]',lambda m:'[F%x]'%(int(m.group(1),16)-depth+base),ins)
        n=re.sub(r'\[esp\]','[F%x]'%(base-depth),n)
        out.append(n)
        if ins.startswith('push'): depth+=4
        elif ins.startswith('pop'): depth-=4
        m=re.match(r'add esp, 0x([0-9a-f]+)',ins)
        if m and depth>0: depth-=int(m.group(1),16)
    return out
a=norm(L[i+1:j]); b=norm(L[j+1:])
sm=difflib.SequenceMatcher(None,[re.sub(r'\[F[0-9a-f]+\]','[F]',x) for x in a],[re.sub(r'\[F[0-9a-f]+\]','[F]',x) for x in b],autojunk=False)
mp={}
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal': continue
    for k in range(i2-i1):
        for x,y in zip(re.findall(r'\[F([0-9a-f]+)\]',a[i1+k]),re.findall(r'\[F([0-9a-f]+)\]',b[j1+k])):
            mp.setdefault(x,{}).setdefault(y,0); mp[x][y]+=1
print(' '.join(f"{x}->{'/'.join(sorted(mp[x],key=lambda q:-mp[x][q]))}" for x in sorted(mp,key=lambda s:int(s,16)) if list(mp[x])!=[x]))
