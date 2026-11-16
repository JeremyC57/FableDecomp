import sys,re,difflib
t=open(sys.argv[1],errors='replace').read()
A=[x for x in t.split('--- retail')[1].split('--- built')[0].splitlines()[1:] if len(x.split(None,2))==3]
B=[x for x in t.split('--- built')[1].splitlines()[1:] if len(x.split(None,2))==3]
def nn(x):
    x=x.split(None,2)[2]; x=re.sub(r'\[(0x)?[0-9a-f]+\]','[A]',x); x=re.sub(r'0x[0-9a-f]{6,}','A',x)
    if x.startswith(('call','jmp','j')): x=re.sub(r'0x[0-9a-f]+','T',x)
    return x
raw='-raw' in sys.argv
sm=difflib.SequenceMatcher(None,[nn(x) for x in A],[nn(x) for x in B],autojunk=False)
print(t.splitlines()[0])
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal' and not raw: continue
    if op=='equal':
        for i,j in zip(range(i1,i2),range(j1,j2)):
            if A[i].split(None,2)[1:]!=B[j].split(None,2)[1:]: print('  R %s | %s'%(A[i][:70],B[j].split(None,1)[1][:60]))
        continue
    for i in range(i1,i2): print('  - '+A[i][:90])
    for j in range(j1,j2): print('  + '+B[j][:90])
    print('  --')
