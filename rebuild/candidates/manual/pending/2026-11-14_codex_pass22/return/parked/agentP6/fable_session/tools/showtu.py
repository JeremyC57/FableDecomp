import json,sys
t=json.load(open(sys.argv[1])); n=0
f=lambda l:' '.join(hex(x) if isinstance(x,int) else x for x in l)
for e in t['events']:
    if e[0]=='t':
        n+=1; print('%4d %-8s f%-3d A[%s] B[%s]'%(n,e[1],e[2],f(e[3]),f(e[4])))
