# sbs.py <q.py output> <from> <to>: side-by-side retail | built instruction listing (instruction index range)
import sys
L=open(sys.argv[1]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(L) if l.startswith('--- built')][0]
a=[l[10:].split(None,1)[1] if len(l)>34 else l for l in L[i+1:j]]; b=[l[10:].split(None,1)[1] if len(l)>34 else l for l in L[j+1:] if l.strip()]
f=int(sys.argv[2]); t=int(sys.argv[3])
for k in range(f,min(t,max(len(a),len(b)))):
    x=a[k].strip() if k<len(a) else ''; y=b[k].strip() if k<len(b) else ''
    print(f"{k:4} {'  ' if x==y else '!!'} {x[:60]:60} | {y[:60]}")
