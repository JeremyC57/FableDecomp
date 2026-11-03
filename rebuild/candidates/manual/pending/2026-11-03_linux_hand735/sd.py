# sd.py <q.py output>: structural diff (ebp offsets and constants normalised)
import re,sys,difflib
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
A=[l for l in L[i+1:j] if l.strip()]; B=[l for l in L[j+1:] if l.strip()]
def n(l):
    p=l.split(None,2)
    s=p[2] if len(p)>2 else ''
    s=re.sub(r'\[ebp [+-] 0x[0-9a-f]+\]','[ebp+X]',s); s=re.sub(r'0x[0-9a-f]+','N',s); return s
a=[n(x) for x in A]; b=[n(x) for x in B]
sm=difflib.SequenceMatcher(None,a,b,autojunk=False); tot=0
for t,i1,i2,j1,j2 in sm.get_opcodes():
    if t=='equal': continue
    tot+=max(i2-i1,j2-j1)
    print('@@',t,i1,i2,j1,j2)
    for k in range(i1,i2): print(' - '+A[k][:90])
    for k in range(j1,j2): print(' + '+B[k][:90])
print('TOTAL',tot)
