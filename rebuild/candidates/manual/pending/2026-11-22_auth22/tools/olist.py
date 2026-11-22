import sys,re
src=open(sys.argv[1]).read().split('\n'); t=open(sys.argv[2]).read()
m=re.search(r"PlayAllSoundEvents@CTCSoundPlayer.*?\n\s+(.*?)\s+total",t,re.S)
sz=dict((int(a),int(b)) for a,b in (x.split(':') for x in m.group(1).split()))
lo,hi=int(sys.argv[3]),int(sys.argv[4])
for n in range(lo,hi+1):
    s=src[n-1].strip()
    if not s or s in('{','}'): 
        if sz.get(n,0)==0: continue
    print("%4d %4s  %s"%(n, sz.get(n,''), s[:150]))
