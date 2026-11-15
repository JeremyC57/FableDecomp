#!/usr/bin/env python3
"""mk.py <base.cpp> <out.cpp> <old> <new> [<old> <new> ...] : exact-substring replacements inside FindBestStartVertex only (count must be 1 unless old ends with '@all')"""
import sys
b,o=sys.argv[1:3]; t=open(b).read()
i=t.index('// Original source line 1303.'); head,body=t[:i],t[i:]
a=sys.argv[3:]
for k in range(0,len(a),2):
    old,new=a[k],a[k+1]; allf=False
    if old.endswith('@all'): old=old[:-4]; allf=True
    n=body.count(old)
    if n==0 or (n>1 and not allf): sys.exit('count %d for %r'%(n,old))
    body=body.replace(old,new)
open(o,'w').write(head+body)
