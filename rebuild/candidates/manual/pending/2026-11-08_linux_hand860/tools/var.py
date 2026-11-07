# var.py <base.cpp> <out.cpp> <old1> <new1> [<old2> <new2> ...]  -- exact-once string replacement variants ("ALL:" prefix = replace all)
import sys
t=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    o,n=a[i].replace('\\n','\n'),a[i+1].replace('\\n','\n')
    allm=o.startswith('ALL:')
    if allm: o=o[4:]
    c=t.count(o)
    if c==0 or (c!=1 and not allm): sys.exit(f"count {c} for {o!r}")
    t=t.replace(o,n)
open(sys.argv[2],'w').write(t)
