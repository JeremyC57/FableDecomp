# zs.py <src.cpp> [flags...]: VC7.1 syntax-only compile (cl /nologo /Zs) with the verifier's INCLUDE (overlay = FABLE_HDR_OVERLAY);
# prints ZS_OK or the tail of the compiler output. Run in the background under timeout 900 (ruling D1 precheck for toolchain-header failures).
import sys,os
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V
e=V.env(); src=sys.argv[1]
r=V.cl(['/nologo','/Zs','/EHsc']+sys.argv[2:]+[V.wpath(src) if V.WINE else src],e)
t=(r.stdout or '')+(r.stderr or '')
print('ZS_OK' if r.returncode==0 else 'ZS_FAIL rc=%s\n%s'%(r.returncode,t[-1500:]))
