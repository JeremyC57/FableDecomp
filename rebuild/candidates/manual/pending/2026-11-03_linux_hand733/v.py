# v.py <base> <out> <old1> <new1> [<old2> <new2> ...]  (\n escapes allowed)
import sys
s=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    o=a[i].encode().decode('unicode_escape'); n=a[i+1].encode().decode('unicode_escape')
    assert o in s, 'MISSING: '+o
    s=s.replace(o,n,1)
open(sys.argv[2],'w').write(s)
