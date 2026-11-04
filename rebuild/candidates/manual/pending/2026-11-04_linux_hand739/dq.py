# dq.py <q.py output> [ctx]: masked side-by-side diff (relocations/call targets/jump targets masked)
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n'); ctx=int(sys.argv[2]) if len(sys.argv)>2 else 2
print(L[0])
try:
    i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
except IndexError:
    print('\n'.join(L[:40])); sys.exit()
def norm(l):
    l=re.sub(r"(push |mov e[a-z][a-z], |mov dword ptr \[[^]]*\], )(0|0x[0-9a-f]{6,})$", r"\1IMM", l)
    p=l.split(None,2)
    if len(p)<3: return ''
    s=p[2]
    s=re.sub(r'\[0x[0-9a-f]{5,}\]','[G]',s); s=re.sub(r'\[0\]','[G]',s)
    s=re.sub(r'^(call|jmp|j\w+) 0x[0-9a-f]+$',r'\1 L',s)
    s=re.sub(r'(dword ptr \[e.. [+-] 0x[0-9a-f]+\]), 0x[0-9a-f]{6,}',r'\1, IMM',s)
    s=re.sub(r'^(push|mov e..,) 0x[0-9a-f]{6,}$',r'\1 IMM',s)
    return s
a=[norm(l) for l in L[i+1:j]]; b=[norm(l) for l in L[j+1:] if l.strip()]
A=[l for l in L[i+1:j]]; B=[l for l in L[j+1:] if l.strip()]
sm=difflib.SequenceMatcher(None,a,b,autojunk=False)
n=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    n+=max(i2-i1,j2-j1)
    print(f'@@ retail {i1}-{i2} built {j1}-{j2}')
    for k in range(max(0,i1-ctx),i1): print('   '+A[k])
    for k in range(i1,i2): print(' - '+A[k])
    for k in range(j1,j2): print(' + '+B[k])
    for k in range(i2,min(len(A),i2+ctx)): print('   '+A[k])
print('DIFFLINES',n,'retail_instr',len(a),'built_instr',len(b))
