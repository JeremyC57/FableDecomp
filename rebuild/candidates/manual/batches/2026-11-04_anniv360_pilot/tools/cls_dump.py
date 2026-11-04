# cls_dump.py <pdb> <pdbtools_dir> Class [Class...] : members + methods (with types) of a 360 class
import sys,struct
sys.path.insert(0,sys.argv[2])
import pdb as P
p=P.PDB(sys.argv[1]); t=p.tpi
for c in sys.argv[3:]:
    ti=t.resolve(c)
    if ti is None: print('NO',c); continue
    tag=t.tag(ti); print('==',c,'ti=%x'%ti,'size',t.size(ti))
    k,d=t.rec(ti)
    fl=struct.unpack_from('<I',d,4)[0]
    for f in t.fields(fl):
        try: ty=t.name(f['type'])
        except Exception: ty='?'
        print('  %-8s %-6s %-30s %s'%(f['kind'],'' if f['off'] is None else hex(f['off']),f['name'],ty))
