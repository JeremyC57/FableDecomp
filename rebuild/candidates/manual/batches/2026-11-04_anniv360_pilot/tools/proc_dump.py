# proc_dump.py <pdb> <pdbtools_dir> <names.txt> <out.txt>
# For each Fable-module procedure whose name is in names.txt: signature type, params/locals in
# declaration order with TYPE, location (PPC reg / frame offset), scope nesting (S_BLOCK32),
# and the line table (code offset -> source line) plus the distinct lines in source order.
import sys,struct
sys.path.insert(0,sys.argv[2])
import pdb as P
p=P.PDB(sys.argv[1]); tpi=p.tpi
want=set(l.strip() for l in open(sys.argv[3]) if l.strip())
FD=('FableLib','BBBLibrary','FableEngine3D','FableParticleSystem','FableScripting','FableLibBase','FableMain','FableCommon','FableDefs','DataDefs')
out=open(sys.argv[4],'w')
def tn(ti):
    try: return tpi.name(ti)
    except Exception as e: return 'ti_%x'%ti
for m in p.modules:
    o=m['obj'] or ''
    if not any('\\%s\\'%d in o for d in FD): continue
    sym,c13=p.module_streams(m)
    files,blocks=p.lines(c13)
    bl={(b['seg'],b['off']):b for b in blocks}
    depth=0; cur=None
    for off,k,d in p.symbols(sym):
        if k in (0x1110,0x110f):
            ln,=struct.unpack_from('<I',d,12); ti,o2,sg=struct.unpack_from('<IIH',d,24); nm=P.cstr(d,35)[0]
            if nm in want:
                cur=nm; depth=1
                b=bl.get((sg,o2))
                out.write('='*70+'\n%s  [%s] len=%d obj=%s\n  type: %s\n'%(nm,'G' if k==0x1110 else 'L',ln,o.split('\\')[-1],tn(ti)))
                cur=(nm,o2,sg,b)
            else: cur=None
            continue
        if cur is None: continue
        ind='  '*depth
        if k==0x1106:
            ti,r=struct.unpack_from('<IH',d,0); out.write('%sREG   %-28s %-40s %s\n'%(ind,P.cstr(d,6)[0],tn(ti),P.ppc_reg(r)))
        elif k==0x1111:
            of,ti,r=struct.unpack_from('<iIH',d,0); out.write('%sSTACK %-28s %-40s %s%+d\n'%(ind,P.cstr(d,10)[0],tn(ti),P.ppc_reg(r),of))
        elif k==0x110b:
            of,ti=struct.unpack_from('<iI',d,0); out.write('%sBPREL %-28s %-40s %+d\n'%(ind,P.cstr(d,8)[0],tn(ti),of))
        elif k==0x1103:
            _,_,ln,o3,sg=struct.unpack_from('<IIIIH',d,0); out.write('%s{ block +0x%x len 0x%x %s\n'%(ind,o3-cur[1],ln,P.cstr(d,18)[0])); depth+=1
        elif k==0x1105:
            pass
        elif k==0x6:
            depth-=1
            if depth<=0:
                b=cur[3]
                if b:
                    ls=b['lines']; out.write('  lines (%s):\n'%b['file'].split('\\')[-1])
                    out.write('   code order: '+' '.join('%x:%d'%(a,l) for a,l in ls)+'\n')
                    out.write('   distinct source lines: '+' '.join(str(x) for x in sorted(set(l for _,l in ls)))+'\n')
                cur=None
            else: out.write('  '*depth+'}\n')
        elif k in (0x1012,0x1019,0x113c,0x1102,0x1105,0x1107,0x1108,0x110c,0x110d): pass
        else: out.write('%s? sym %x\n'%(ind,k))
