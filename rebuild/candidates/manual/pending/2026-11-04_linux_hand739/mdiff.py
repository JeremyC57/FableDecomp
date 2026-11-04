import sys,re,difflib
t=open(sys.argv[1]).read().split('\n')
r=[];b=[];cur=None
for l in t:
    if l.startswith('--- retail'): cur=r; continue
    if l.startswith('--- built'): cur=b; continue
    m=re.match(r'^([0-9a-f]{8})\s+[0-9a-f]+\s+(.*)$',l)
    if m and cur is not None: cur.append((m.group(1),m.group(2)))
def norm(s):
    s=re.sub(r'0x[0-9a-f]{5,}','ADDR',s); s=re.sub(r'\[0\]','[ADDR]',s)
    s=re.sub(r'(call|j[a-z]+) \S+',r'\1 X',s)
    return s
ra=[norm(x[1]) for x in r]; ba=[norm(x[1]) for x in b]
sm=difflib.SequenceMatcher(None,ra,ba,autojunk=False)
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal':
        print(f'  == {i2-i1} same (retail {r[i1][0]}..)'); continue
    for k in range(i1,i2): print(f'  - {r[k][0]} {r[k][1]}')
    for k in range(j1,j2): print(f'  + {b[k][0]} {b[k][1]}')
