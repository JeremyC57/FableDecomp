#!/usr/bin/env python3
# setstatus.py <addr> <grade> <bytes> <test> <grades_used> <notes>
import sys, os
W=os.environ.get("W", os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
a,g,b,t,n,notes=sys.argv[1:7]
L=open(f"{W}/STATUS.tsv").read().splitlines()
out=[]
for l in L:
    if l.split("\t")[0]==a: l="\t".join([a,g,b,t,n,notes])
    out.append(l)
open(f"{W}/STATUS.tsv","w").write("\n".join(out)+"\n")
