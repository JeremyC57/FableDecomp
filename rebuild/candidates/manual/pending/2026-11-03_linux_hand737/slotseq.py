# slotseq.py <q.py output>: aligned retail/built instructions that touch a frame slot, printed side by side where the slots differ
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n'); base=0x10
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def norm(lines):
    depth=0; out=[]
    for l in lines:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2]
        n=re.sub(r'\[esp \+ 0x([0-9a-f]+)\]',lambda m:'[F%x]'%(int(m.group(1),16)-depth+base),ins)
        n=re.sub(r'\[esp\]','[F%x]'%(base-depth),n)
        out.append((p[0],n))
        if ins.startswith('push'): depth+=4
        elif ins.startswith('pop'): depth-=4
        m=re.match(r'add esp, 0x([0-9a-f]+)',ins)
        if m and depth>0: depth-=int(m.group(1),16)
    return out
a=norm(L[i+1:j]); b=norm(L[j+1:])
sm=difflib.SequenceMatcher(None,[re.sub(r'\[F[0-9a-f]+\]','[F]',x[1]) for x in a],[re.sub(r'\[F[0-9a-f]+\]','[F]',x[1]) for x in b],autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal': continue
    for k in range(i2-i1):
        x=a[i1+k]; y=b[j1+k]
        if '[F' in x[1] and x[1]!=y[1]: print(f'{x[0]} {x[1]:45} | {y[1]}')
