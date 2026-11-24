import sys
f,a,b=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16)
per=int(sys.argv[4]) if len(sys.argv)>4 else 3
out=[]
for l in open(f):
    p=l.split(None,2)
    if len(p)<3: continue
    try: ad=int(p[0],16)
    except: continue
    if a<=ad<=b: out.append("%x %s"%(ad&0xfffff,p[2].strip().replace('dword ptr ','').replace('byte ptr ','b ')))
for i in range(0,len(out),per): print(" | ".join(out[i:i+per]))
