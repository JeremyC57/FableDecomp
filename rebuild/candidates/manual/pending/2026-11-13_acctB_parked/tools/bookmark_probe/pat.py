import sys,re
def blocks(lines):
    out=[]
    for i,l in enumerate(lines):
        m=re.search(r'mov (e\w\w), dword ptr \[e\w\w \+ 0x2814\]',l)
        if not m: continue
        M=m.group(1); prev=lines[i-1]; nxt=lines[i+1] if i+1<len(lines) else ''
        b1=re.search(r'mov (e\w\w), dword ptr \[e\w\w \+ 8\]',prev); b2=re.search(r'mov (e\w\w), dword ptr \[e\w\w \+ 8\]',nxt)
        if b1: o='S'; B=b1.group(1)
        elif b2: o='C'; B=b2.group(1)
        else: out.append('???'); continue
        t='?'; r='?'
        for l2 in lines[i+1:i+6]:
            m2=re.search(r'\btest (e\w\w), (e\w\w)',l2)
            if m2 and t=='?': t='m' if m2.group(1)==M else 'b'
            m3=re.search(r'\bor (e\w\w), (e\w\w)',l2)
            if m3 and r=='?': r='m' if m3.group(1)==M else 'b'
        out.append(o+t+r)
    return out
L=open(sys.argv[1]).read().split('\n')
if len(sys.argv)>2:
    k=[k for k,l in enumerate(L) if l.startswith('--- built')]
    if not k: print(''); sys.exit(0)
    L=L[k[0]+1:]
print(' '.join(blocks(L)))
