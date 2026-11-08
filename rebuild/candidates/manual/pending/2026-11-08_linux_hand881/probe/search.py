import subprocess,sys,itertools,random,re
# search.py out.txt : enumerate block-1 wrap sets X<row>_<kgroup> (row 0..3, kgroup 1..3) with W5 none/a
base=['W3','W4','W7','G']
def run(tag,feats):
    r=subprocess.run(['./swr.sh',tag]+base+feats,capture_output=True,text=True).stdout.strip()
    c=subprocess.run(['python3','cs2.py','r_%s.out'%tag],capture_output=True,text=True).stdout.splitlines()
    return r,c
out=open(sys.argv[1],'a')
mode=sys.argv[2]; W5S=[x.split(',') if x else [] for x in sys.argv[3].split(':')]
if mode=='rowk':
    for rs in range(1,16):
        for ks in range(1,8):
            for w5 in W5S:
                feats=['X%d_%d'%(r,k) for r in range(4) if rs>>r&1 for k in (1,2,3) if ks>>(k-1)&1]+w5
                tag='s%d_%d_%s'%(rs,ks,''.join(w5))
                r,c=run(tag,feats)
                c0=c[1] if len(c)>1 else ''; c7=c[15] if len(c)>15 else ''
                ok0=c0.split()[2:3]==['+==++++=>=>$']; 
                out.write('%s rs=%d ks=%d w5=%s | %s | c0ok=%s c0=%s c7=%s\n'%(tag,rs,ks,w5,r.split(']')[1][:40],ok0,c0[4:30],c7[4:50])); out.flush()
