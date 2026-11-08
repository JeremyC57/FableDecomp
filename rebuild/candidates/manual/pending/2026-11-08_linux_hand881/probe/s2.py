import subprocess,sys,itertools
base=['W3','W4','W7','G']
out=open(sys.argv[1],'a')
R7='++++++++++++SSSS=GS>SG=GS>SSSSSI+I+I+I+SSSSSSSSSSSSSSSSS=GS>SG=GS>S$'
for m in range(64):
    feats=[f for i,f in enumerate(['W1k1','W1k2','W1k3','W5a','W1b5','W2n']) if m>>i&1]
    tag='z%d'%m
    r=subprocess.run(['./swr.sh',tag]+base+feats,capture_output=True,text=True).stdout.strip()
    c=subprocess.run(['python3','cs2.py','r_%s.out'%tag],capture_output=True,text=True).stdout.splitlines()
    c0=c[1].split()[2] if len(c)>1 else ''; c7=c[15].split()[2] if len(c)>15 else ''
    out.write('%s %s | %s | c0=%s %s c7=%s %s\n'%(tag,feats,r.split(']')[1][:28],c0,c0=='+==++++=>=>$',c7[:40],c7==R7)); out.flush()
