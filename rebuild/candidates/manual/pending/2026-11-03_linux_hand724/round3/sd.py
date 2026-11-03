# sd.py <q.py output>: structural diff -- stack offsets masked, branch targets masked
import sys,re,difflib
t=open(sys.argv[1]).read().split('\n')
def grab(start):
    out=[];on=False
    for l in t:
        if l.startswith(start): on=True; continue
        if on and l.startswith('---'): break
        if on:
            m=re.match(r'([0-9a-f]{8})\s+[0-9a-f]+\s+(.*)',l)
            if m: out.append((m.group(1),m.group(2)))
    return out
r=grab('--- retail'); b=grab('--- built')
def norm(s):
    s=re.sub(r'\[e[sb]p [+-] 0x[0-9a-f]+\]','[esp+N]',s); s=re.sub(r'\[esp\]','[esp+N]',s)
    s=re.sub(r'(j[a-z]+|call) 0x[0-9a-f]+',r'\1 X',s); s=re.sub(r'\[0x1[0-9a-f]{6}\]','[G]',s); s=re.sub(r'\[0\]','[G]',s); s=re.sub(r'0x1[0-9a-f]{6}\b','G',s); s=re.sub(r'push 0$','push G',s)
    return s
rn=[norm(x[1]) for x in r]; bn=[norm(x[1]) for x in b]
n=0
for op in difflib.SequenceMatcher(None,rn,bn,autojunk=False).get_opcodes():
    if op[0]=='equal': continue
    n+=max(op[2]-op[1],op[4]-op[3])
    if len(sys.argv)>2 and n<int(sys.argv[2]): 
        print('@@',op[0],'retail',r[op[1]][0] if op[1]<len(r) else '-', 'built',b[op[3]][0] if op[3]<len(b) else '-')
        for i in range(op[1],op[2]): print(' -',r[i][0],r[i][1])
        for i in range(op[3],op[4]): print(' +',b[i][0],b[i][1])
print('structdiff',n)
