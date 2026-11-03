# var.py <base> <out> <old> <new> [<old> <new>]... : apply exact replacements (each must occur)
import sys
s=open(sys.argv[1]).read(); a=sys.argv[3:]
for i in range(0,len(a),2):
    assert a[i] in s, 'MISSING: '+a[i]
    s=s.replace(a[i],a[i+1],1)
open(sys.argv[2],'w').write(s)
