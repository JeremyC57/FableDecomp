# mkbind.py <pass.tsv> <out BINDINGS.tsv> <out reloc_bindings.after.tsv>
import sys, os, re, json, glob, struct, subprocess, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
from common import *
import reloc_identity as RI
W=os.environ['S']+'/wave33'
cat=catalog()
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip()]
mine=set(r[1] for r in rows)
gen={}
for fam in set(r[0] for r in rows):
    for l in open(W+'/'+fam+'/gen.tsv'):
        a=l.rstrip('\n').split('\t')
        if a[1]=='OK': gen[(fam,a[0])]=json.loads(a[2])
tbl=[]; bysym={}
for r in csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    tbl.append(r); bysym.setdefault(r['symbol'],r)
newpairs={}  # addr -> set((sym,target))
for fam,a,g in rows:
    objs=glob.glob(W+'/'+fam+'/rk_*/work/*/*/'+a+'/'+a+'.obj')+glob.glob(W+'/'+fam+'/rk_*/*/work/*/*/'+a+'/'+a+'.obj')+glob.glob(W+'/final/rk_*/work/*/*/'+a+'/'+a+'.obj')
    objs=sorted(objs,key=os.path.getmtime)
    assert objs, a
    q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3]); base=int(a,16)
    rel=RI.function_relocs('objdump',objs[-1],gen[(fam,a)]['sym'],ln)
    assert rel is not None, a
    ps=set()
    for off,kind,sym in rel:
        if sym.startswith(RI.SKIP_PREFIXES): continue
        if kind in ('REL32','DISP32'): t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff
        else: t=struct.unpack_from('<I',b,off)[0]
        ps.add((sym,t))
    newpairs[a]=ps
out=['action\tsymbol\ttarget\tby_row\tnote']
after=[dict(r) for r in tbl]
syms_after={r['symbol'] for r in after}
# ADD / CONFLICT
for a,ps in sorted(newpairs.items()):
    for sym,t in sorted(ps):
        if sym in bysym:
            if int(bysym[sym]['target'],16)!=t: out.append('CONFLICT\t%s\t%08x\t%s\tbound to %s by %s'%(sym,t,a,bysym[sym]['target'],bysym[sym]['address']))
        elif sym not in syms_after:
            out.append('ADD\t%s\t%08x\t%s\tnaming wave 33: emitted by the respelled row'%(sym,t,a)); after.append(dict(symbol=sym,target='%08x'%t,address=a)); syms_after.add(sym)
# RETIRE / KEEP: bindings first-bound by a respelled row that the row no longer emits
srcs=None
for r in tbl:
    if r['address'] in mine and (r['symbol'],int(r['target'],16)) not in newpairs[r['address']]:
        m=re.match(r'^\?{1,2}([0-9A-Z_]?)([A-Za-z_][A-Za-z_0-9]*)@',r['symbol'])
        ident=m.group(2) if m else r['symbol']
        if srcs is None:
            srcs={}
            for p in glob.glob(ROOT+'/rebuild/src/compiled/*/*/*.cpp'):
                srcs[p]=open(p,errors='replace').read()
        users=[p for p,s in srcs.items() if ident in s and not any(p.endswith('_'+x+'.cpp') for x in mine)]
        if users:
            alt=None
            out.append('KEEP\t%s\t%s\t%s\tstill named by %d unedited landed source(s) (text search for %s), e.g. %s; the first binder %s no longer emits it'%(r['symbol'],r['target'],r['address'],len(users),ident,os.path.relpath(users[0],ROOT),r['address']))
        else:
            out.append('RETIRE\t%s\t%s\t%s\tno landed source names %s after wave 33 (was bound by %s)'%(r['symbol'],r['target'],r['address'],ident,r['address']))
            after=[x for x in after if x['symbol']!=r['symbol']]
open(sys.argv[2],'w').write('\n'.join(out)+'\n')
with open(sys.argv[3],'w') as f:
    f.write('symbol\ttarget\taddress\n')
    for r in after: f.write('%s\t%s\t%s\n'%(r['symbol'],r['target'],r['address']))
import collections
print(collections.Counter(l.split('\t')[0] for l in out[1:]))
