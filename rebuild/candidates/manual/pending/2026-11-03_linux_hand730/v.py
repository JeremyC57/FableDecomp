# v.py <base> <out> <old1> <new1> [<old2> <new2> ...]: apply replacements (each must occur exactly once)
import sys
s=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    assert s.count(a[i])==1, 'COUNT %d: %s'%(s.count(a[i]),a[i][:80])
    s=s.replace(a[i],a[i+1])
open(sys.argv[2],'w').write(s)
