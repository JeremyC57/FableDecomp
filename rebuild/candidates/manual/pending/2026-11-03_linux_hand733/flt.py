# flt.py <file.d>: show hunks whose -/+ lines differ beyond reloc immediates and esp displacements
import sys,re
def norm(l):
    l=l[3:]
    l=re.sub(r'^[0-9a-f]+\s+[0-9a-f]+\s+','',l)
    l=re.sub(r'\[esp \+ 0x[0-9a-f]+\]','[esp+X]',l)
    l=re.sub(r'0x[0-9a-f]{5,}','R',l); l=re.sub(r'^push 0$','push R',l); l=re.sub(r', 0$',', R',l)
    return l.strip()
blocks=open(sys.argv[1]).read().split('@@ ')
print(blocks[0].strip())
for b in blocks[1:]:
    lines=b.split('\n')
    m=[norm(l) for l in lines if l.startswith(' - ')]; p=[norm(l) for l in lines if l.startswith(' + ')]
    if m==p: continue
    print('@@ '+b.rstrip())
