import os,struct,bisect,sys
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
opt=struct.unpack_from("<H",exe,pe+20)[0]
o=pe+24+opt; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8)
text=exe[ro:ro+rs]; base=0x400000+va
tgt=int(sys.argv[1],16)
hits=[]
i=text.find(b'\xe8')
while i>=0 and i<len(text)-5:
    d=struct.unpack_from("<i",text,i+1)[0]
    if base+i+5+d==tgt: hits.append(base+i)
    i=text.find(b'\xe8',i+1)
funcs=[]
for r in open('rebuild/manifest/functions.tsv'):
    try: funcs.append((int(r.split('\t')[0],16), r.split('\t')[1], r.split('\t')[2]))
    except: pass
funcs.sort(); fa=[f[0] for f in funcs]
from collections import Counter
c=Counter()
for h in hits:
    k=bisect.bisect_right(fa,h)-1; c[funcs[k]]+=1
for f,n in sorted(c.items()): print(hex(f[0]),f[2]+'::'+f[1],n)
