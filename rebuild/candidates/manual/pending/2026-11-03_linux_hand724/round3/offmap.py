# offmap.py <q.py output>: pair retail/built instructions that differ only in [esp+X] displacement; print mapping retail->built with push depth unknown
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def ins(l):
    p=l.split(None,2); return p[2] if len(p)>2 else ''
A=[ins(l) for l in L[i+1:j]]; B=[ins(l) for l in L[j+1:] if l.strip()]
na=[re.sub(r'esp \+ 0x[0-9a-f]+','esp+N',x) for x in A]; nb=[re.sub(r'esp \+ 0x[0-9a-f]+','esp+N',x) for x in B]
na=[re.sub(r'0x[0-9a-f]{5,}','G',x) for x in na]; nb=[re.sub(r'0x[0-9a-f]{5,}','G',x) for x in nb]
sm=difflib.SequenceMatcher(None,na,nb,autojunk=False)
m={}
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal': continue
    for a,b in zip(range(i1,i2),range(j1,j2)):
        ra=re.findall(r'esp \+ (0x[0-9a-f]+)',A[a]); rb=re.findall(r'esp \+ (0x[0-9a-f]+)',B[b])
        for x,y in zip(ra,rb):
            if x!=y: m.setdefault((x,y),[]).append(a)
for (x,y),v in sorted(m.items(),key=lambda t:t[1][0]): print(f'retail {x} -> built {y}  at retail instr {v[:6]}')
