# map every vtable (RTTI COL-preceded) -> class typedesc, subobject offset, slots
import sys, struct, json
sys.path.insert(0, sys.argv[0].rsplit('/',1)[0])
from pe import PE
p=PE()
rd=[s for s in p.secs if s[0]=='.rdata'][0]
out=open(sys.argv[1],'w')
out.write('vtable\tcol_off\ttypedesc\tslot\tfunc\n')
s_va=rd[1]; n=rd[4]
cols={}
for va in range(s_va, s_va+n-4, 4):
    col=p.u32(va)
    if col is None or not (rd[1]<=col<rd[1]+n): continue
    # COL: sig, offset, cdOffset, pTD, pCHD
    try:
        sig,off,cdo,ptd,pchd=struct.unpack_from('<5I',p.d,p.off(col))
    except Exception: continue
    if sig!=0: continue
    if p.sec(ptd) is None or p.sec(pchd) is None: continue
    try: name=p.cstr(ptd+8)
    except Exception: continue
    if not name.startswith('.?A'): continue
    vt=va+4
    k=0
    while True:
        f=p.u32(vt+4*k)
        if f is None or not p.is_text(f): break
        if k>0:
            # stop at next COL marker
            pass
        out.write('%08x\t%d\t%s\t%d\t%08x\n'%(vt,off,name,k,f)); k+=1
        nx=p.u32(vt+4*k)
        # next dword might be COL of next vtable: check
        if nx is not None and rd[1]<=nx<rd[1]+n:
            try:
                if struct.unpack_from('<I',p.d,p.off(nx))[0]==0 and p.u32(nx+12) and p.cstr(p.u32(nx+12)+8).startswith('.?A'): break
            except Exception: pass
