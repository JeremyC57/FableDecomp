# rv.py <base> <out> <pairsfile>: apply OLD\n===\nNEW\n%%%\n blocks in order
import sys
s=open(sys.argv[1]).read()
blocks=open(sys.argv[3]).read().split('\n%%%\n')
for b in blocks:
    if not b.strip(): continue
    o,n=b.split('\n===\n')
    assert o in s,'MISSING:'+o
    s=s.replace(o,n,1)
open(sys.argv[2],'w').write(s)
