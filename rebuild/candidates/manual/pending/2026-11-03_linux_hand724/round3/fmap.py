# fmap.py <q.py output> <base_pushes_regs...>: list F-relative (frame after sub esp) offsets used by retail and built, paired.
# Push depth: callee-saved pushes counted cumulatively; argument pushes reset after each call (callee cleans).
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def ins(l):
    p=l.split(None,2); return p[2] if len(p)>2 else ''
def track(X):
    out=[]; saved=0; args=0; seen_sub=False
    for x in X:
        d=(saved+args)*4
        r=[int(v,16)-d for v in re.findall(r'esp \+ (0x[0-9a-f]+)',x)]
        if re.search(r'\[esp\]',x): r.append(0-d)
        out.append(r)
        if x.startswith('push e') and x.split()[1] in ('ebx','esi','edi','ebp') and args==0: saved+=1
        elif x.startswith('push'): args+=1
        elif x.startswith('pop'): saved-=1
        elif x.startswith('call'): args=0
        elif x.startswith('add esp'): args-=int(x.split(',')[1],16)//4 if args else 0
    return out
A=[ins(l) for l in L[i+1:j]]; B=[ins(l) for l in L[j+1:] if l.strip()]
na=[re.sub(r'esp \+ 0x[0-9a-f]+','esp+N',x) for x in A]; nb=[re.sub(r'esp \+ 0x[0-9a-f]+','esp+N',x) for x in B]
na=[re.sub(r'0x[0-9a-f]{5,}','G',x) for x in na]; nb=[re.sub(r'0x[0-9a-f]{5,}','G',x) for x in nb]
ta=track(A); tb=track(B)
sm=difflib.SequenceMatcher(None,na,nb,autojunk=False)
m={}
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal': continue
    for a,b in zip(range(i1,i2),range(j1,j2)):
        for x,y in zip(ta[a],tb[b]):
            m.setdefault(x,{}).setdefault(y,[]).append(a)
for x in sorted(m):
    if x>=0x130: continue
    s=' '.join(f'{y:#x}{"*" if y!=x else ""}(@{v[0]}..{len(v)})' for y,v in m[x].items())
    print(f'F+{x:#05x} -> {s}')
