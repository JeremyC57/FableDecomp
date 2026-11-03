# slots.py <q.py output>: per side, stack slot uses normalised to the frame base (esp offset minus pushes since the prologue)
import sys,re
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
def scan(lines):
    depth=0; out={}
    for l in lines:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2]
        for m in re.finditer(r'\[esp \+ 0x([0-9a-f]+)\]',ins):
            off=int(m.group(1),16)-depth
            out.setdefault(off,[]).append(p[0][-4:]+':'+ins[:40])
        if ins.startswith('push'): depth+=4
        elif ins.startswith('pop'): depth-=4
        m=re.match(r'add esp, 0x([0-9a-f]+)',ins)
        if m and depth>0: depth-=int(m.group(1),16)
        if ins.startswith('call') and 'ret' in ins: pass
    return out
import collections
a=scan(L[i+1:j]); b=scan(L[j+1:])
for name,d in (('retail',a),('built',b)):
    print(name, ' '.join(f'{k:x}:{len(v)}' for k,v in sorted(d.items())))
