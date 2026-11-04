import sys,struct,collections
sys.path.insert(0,sys.argv[2])
import pdb as P
p=P.PDB(sys.argv[1]); t=p.tpi
FD=('FableLib','BBBLibrary','FableEngine3D','FableParticleSystem','FableScripting')
c=collections.Counter(); blocks=0
for m in p.modules:
    if not any('\\%s\\'%d in (m['obj'] or '') for d in FD): continue
    sym,_=p.module_streams(m); cur=None
    def fin():
        if cur is None: return
        reg,rr,npar=cur
        c['procs']+=1; c['reg_total']+=reg; c['regrel_total']+=rr
        if npar is not None:
            c['reg_le_params']+= reg<=npar
            c['reg_gt_params']+= reg>npar
    for off,k,d in p.symbols(sym):
        if k in (0x1110,0x110f):
            fin(); ti,=struct.unpack_from('<I',d,24); k2,dd=t.rec(ti); npar=None
            if k2==0x1009: npar=struct.unpack_from('<H',dd,16)[0]+1   # LF_MFUNCTION parmcount (+this)
            elif k2==0x1008: npar=struct.unpack_from('<H',dd,6)[0]    # LF_PROCEDURE
            cur=[0,0,npar]
        elif k==0x1103: blocks+=1
        elif cur and k==0x1106: cur[0]+=1
        elif cur and k==0x1111: cur[1]+=1
    fin()
print(dict(c),'blocks',blocks)
