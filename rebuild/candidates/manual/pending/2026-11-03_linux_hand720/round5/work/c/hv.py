# hv.py <tag> <src> old new [old new ...] : CDefPointer.h variant probe on a source
import sys,subprocess,shutil,os
D='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agent720d'
tag=sys.argv[1]; src=sys.argv[2]; pairs=sys.argv[3:]
addr=os.environ.get('ADDR','005c3180'); leaf=os.environ.get('LEAF','GetActiveProjectileWeaponDamage')
base=open('/home/user/FableDecomp/rebuild/include/engine/CDefPointer.h').read()
for k in range(0,len(pairs),2):
    assert pairs[k] in base, 'missing: '+pairs[k][:70]
    base=base.replace(pairs[k],pairs[k+1])
hd=f'{D}/work/hv_{tag}'
if os.path.exists(hd): shutil.rmtree(hd)
os.makedirs(hd+'/hdr')
subprocess.run(f'cp -r {D}/hdr/* {hd}/hdr/',shell=True)
open(hd+'/hdr/engine/CDefPointer.h','w').write(base)
out=f'{hd}/o.txt'
r=subprocess.run(f'bash {D}/work/p.sh {hd} {addr} {src} {leaf} {out}',shell=True,capture_output=True,text=True)
r2=subprocess.run(['python3',f'{D}/work/b/pr/dl.py',out],capture_output=True,text=True)
t=open(out).read()
print(tag, t.splitlines()[0].split()[2] if t else '?', 'dl', (r2.stdout.splitlines() or ['?'])[0])
if 'error' in t.lower(): print('\n'.join([l for l in t.splitlines() if 'error' in l.lower()][:3]))
