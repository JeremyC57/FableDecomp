# rep.py <file> <pairs.json>: apply [old, new] replacement pairs, each old string must occur exactly once (else nothing is written)
import sys,json
f=sys.argv[1]; s=open(f,encoding='utf-8').read(); P=json.load(open(sys.argv[2]))
for o,n in P:
    c=s.count(o)
    if c!=1: sys.exit('FAIL %s: %d occurrences of %r'%(f,c,o[:80]))
    s=s.replace(o,n)
open(f,'w',encoding='utf-8').write(s); print('OK',f.rsplit('/',1)[-1],len(P))
