# var.py <base.cpp> <out.cpp> <old1> <new1> [<old2> <new2> ...]  -- exact-once string replacement variants
import sys
t=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    o,n=a[i].replace('\\n','\n'),a[i+1].replace('\\n','\n')
    c=t.count(o)
    if c!=1: sys.exit(f"count {c} for {o!r}")
    t=t.replace(o,n)
open(sys.argv[2],'w').write(t)
