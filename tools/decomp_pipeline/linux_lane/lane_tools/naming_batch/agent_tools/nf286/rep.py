# rep.py <file> <old> <new> [<old> <new> ...]: exact single-occurrence replacements; the line count must not change
import sys
f=sys.argv[1]; t=open(f,encoding='utf-8',errors='surrogateescape').read(); n0=t.count('\n'); a=sys.argv[2:]
for o,n in zip(a[::2],a[1::2]):
    if t.count(o)!=1: print('FAIL count=%d for %r'%(t.count(o),o)); sys.exit(1)
    t=t.replace(o,n)
if t.count('\n')!=n0: print('FAIL line delta'); sys.exit(1)
open(f,'w',encoding='utf-8',errors='surrogateescape').write(t); print('OK',f)
