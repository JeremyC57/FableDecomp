# Fable-module procedures with first source line, file, and local/param names -> TSV
import sys,struct,collections
sys.path.insert(0,sys.argv[2])
import pdb as P
p=P.PDB(sys.argv[1])
FD=('FableLib','BBBLibrary','FableEngine3D','FableParticleSystem','FableScripting','FableLibBase','FableMain','FableCommon','FableDefs','DataDefs')
out=open(sys.argv[3],'w')
for m in p.modules:
    o=m['obj'] or ''
    if not any('\\%s\\'%d in o for d in FD): continue
    sym,c13=p.module_streams(m)
    files,blocks=p.lines(c13)
    bl={(b['seg'],b['off']):b for b in blocks}
    cur=None
    def flush():
        if cur: out.write('\t'.join([cur[0],cur[1],str(cur[2]),cur[3],str(cur[4]),','.join(cur[5])])+'\n')
    for off,k,d in p.symbols(sym):
        if k in (0x1110,0x110f):
            flush()
            ln,=struct.unpack_from('<I',d,12); o2,sg=struct.unpack_from('<IH',d,28); nm=P.cstr(d,35)[0]
            b=bl.get((sg,o2)); f=b['file'] if b else '?'; first=min((x[1] for x in b['lines']),default=0) if b else 0
            cur=[nm,'G' if k==0x1110 else 'L',ln,f,first,[]]
        elif cur and k==0x1106: cur[5].append(P.cstr(d,6)[0])
        elif cur and k==0x1111: cur[5].append(P.cstr(d,10)[0])
    flush()
