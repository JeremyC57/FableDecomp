#!/usr/bin/env python3
"""cmpq.py <q.py output> [-v] : status line + count of differing instruction lines with relocated operands masked."""
import sys,re
t=open(sys.argv[1]).read()
st=t.splitlines()[0] if t else ''
if '--- built' not in t: print(st); sys.exit()
a=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]
b=t.split('--- built')[1].splitlines()[1:]
def norm(l,base_lo,base_hi):
    p=l.split(None,2); 
    if len(p)<3: return l
    ins=p[2]
    ins=re.sub(r'\[(0x)?[0-9a-f]+\]','[A]',ins)
    ins=re.sub(r'\[(e\w\w) \+ (0x)?[0-9a-f]{5,}\]',r'[\1+A]',ins)
    def rep(m):
        v=int(m.group(0),16)
        return m.group(0) if base_lo<=v<base_hi else 'A'
    if ins.startswith(('call','jmp')): ins=re.sub(r'0x[0-9a-f]+',rep,ins)
    ins=re.sub(r'0x[0-9a-f]{6,}','A',ins)
    if ins.startswith(('push','mov')): ins=re.sub(r', (0x)?[0-9a-f]+$',lambda m: ', A' if m.group(0)!=', 0' else m.group(0),ins) if False else ins
    return ins
lo=int(a[0].split()[0],16); hi=lo+0x100000
n=0; out=[]
for i,(x,y) in enumerate(zip(a,b)):
    if norm(x,lo,lo+len(a)*16)!=norm(y,lo,lo+len(a)*16):
        n+=1; out.append(f"{i:4} {x[:75]:77}| {y[34:80]}")
n+=abs(len(a)-len(b))
print(st, 'difflines=%d'%n, 'ninsn=%d/%d'%(len(a),len(b)))
if '-v' in sys.argv: print('\n'.join(out[:80]))
