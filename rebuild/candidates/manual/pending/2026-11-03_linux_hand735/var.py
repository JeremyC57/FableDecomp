# var.py <base.cpp> <out.cpp> <old1> <new1> [<old2> <new2> ...]: replace all occurrences (each must occur)
import sys
s=open(sys.argv[1]).read()
a=sys.argv[3:]
for i in range(0,len(a),2):
    o=a[i].replace('\\n','\n'); n=a[i+1].replace('\\n','\n')
    if o not in s: sys.exit('MISSING: '+o)
    s=s.replace(o,n)
open(sys.argv[2],'w').write(s)
