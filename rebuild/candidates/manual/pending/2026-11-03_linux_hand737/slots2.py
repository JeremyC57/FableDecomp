# slots2.py <q.py output> <callee-pop table addr:n,...>: frame-slot map (offset from esp after prologue) per side, with callee-popped args
import sys,re,difflib
L=open(sys.argv[1]).read().split('\n')
tab={}
for kv in sys.argv[2].split(','):
    k,v=kv.split(':'); tab[k]=int(v)
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def norm(lines, retail):
    depth=None; out=[]; seen_body=False
    rows=[]
    for l in lines:
        p=l.split(None,2)
        if len(p)==3: rows.append((p[0],p[2]))
    # prologue: pushes until first non-push after 'sub esp' handled by counting pushes before first call
    depth=0; pro=True
    for a,ins in rows:
        n=re.sub(r'\[esp \+ 0x([0-9a-f]+)\]',lambda m:'[F%x]'%(int(m.group(1),16)-depth),ins)
        n=re.sub(r'\[esp\]','[F%x]'%(-depth),n)
        out.append((a,n))
        if ins.startswith('push') and not pro: depth+=4
        if ins.startswith('pop') or ins.startswith('ret'): pro=True
        if ins.startswith('call'):
            pro=False
            tgt=ins.split()[-1]
            if retail: key=tgt
            else: key=None
            depth-=4*tab.get(tgt if retail else '?',0)
        if pro and not ins.startswith('push') and not ins.startswith('sub esp') and not ins.startswith('mov') and not ins.startswith('test') and not ins.startswith('je') and not ins.startswith('pop'):
            pro=False
    return out
