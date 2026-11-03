# dq2.py <q.py output> [nostack]: diff with relocations masked; with 'nostack' also mask esp offsets
import re,difflib,sys
L=open(sys.argv[1]).read().split('\n'); ns=len(sys.argv)>2
print(L[0])
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def norm(l):
    p=l.split(None,2)
    if len(p)<3: return ''
    s=p[2]
    s=re.sub(r'\[0x[0-9a-f]{5,}\]','[G]',s); s=re.sub(r'\[0\]','[G]',s)
    s=re.sub(r'^(call|jmp|j\w+) 0x[0-9a-f]+$',r'\1 L',s)
    s=re.sub(r'(dword ptr \[e.. [+-] 0x[0-9a-f]+\]), 0x[0-9a-f]{6,}',r'\1, IMM',s)
    s=re.sub(r'^(push|mov e..,) (0x[0-9a-f]{6,}|0)$',r'\1 IMM',s)
    if ns: s=re.sub(r'\[esp \+ 0x[0-9a-f]+\]','[esp+X]',s)
    return s
a=[norm(l) for l in L[i+1:j]]; b=[norm(l) for l in L[j+1:] if l.strip()]
A=L[i+1:j]; B=[l for l in L[j+1:] if l.strip()]
sm=difflib.SequenceMatcher(None,a,b,autojunk=False); n=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    n+=max(i2-i1,j2-j1)
    print('@@ retail %d-%d built %d-%d'%(i1,i2,j1,j2))
    for k in range(max(0,i1-2),i1): print('   '+A[k])
    for k in range(i1,i2): print(' - '+A[k])
    for k in range(j1,j2): print(' + '+B[k])
    for k in range(i2,min(len(A),i2+2)): print('   '+A[k])
print('DIFFLINES',n)
