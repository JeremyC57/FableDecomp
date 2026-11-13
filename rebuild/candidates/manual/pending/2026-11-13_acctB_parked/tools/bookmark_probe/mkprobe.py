# mkprobe.py <src_hdr_dir> <dst_dir> : copy overlay; add PROBE_<Setter> macro at the start of every one-line FORCEINLINE setter body
import sys,shutil,os,re
src,dst=sys.argv[1],sys.argv[2]
if os.path.exists(dst): shutil.rmtree(dst)
shutil.copytree(src,dst)
p=dst+'/engine/CRenderStateManager.h'
if not os.path.exists(p): shutil.copy('/home/user/FableDecomp/rebuild/include/engine/CRenderStateManager.h',p)
s=open(p).read(); names=set()
def rep(m):
    names.add(m.group(2)); return m.group(1)+'{ PROBE_'+m.group(2)+' SetRenderState('
s=re.sub(r'(FABLE_FORCEINLINE void (Set\w+)\([^)]*\) )\{ SetRenderState\(',rep,s)
hdr=''.join(f'#ifndef PROBE_{n}\n#define PROBE_{n}\n#endif\n' for n in sorted(names))
s=hdr+s
open(p,'w').write(s); print(len(names),'setters')
