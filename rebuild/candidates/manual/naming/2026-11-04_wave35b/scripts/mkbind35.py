# mkbind35.py <PASS.tsv|DEFERRED.tsv> <out BINDINGS.tsv>: wave 33 mkbind.py logic over the final wave 35b objects
import sys,os,re,glob,struct,csv,json
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
from common import catalog, ROOT
import reloc_identity as RI
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
cat=catalog()
mine=[l.split('\t')[0] for l in open(sys.argv[1]) if re.match(r'^[0-9a-f]{8}\t',l)]
allmine={l.split('\t')[0] for l in open(W+'/final/all.tsv')}
leaf={l.split('\t')[0]:l.split('\t')[2] for l in open(W+'/final/all.tsv')}
oldsym={}
for f in 'tstruct3 lvlalloc lvltstruct frameloop plainctor requal lvlctor entctor minent man'.split():
    for l in open(W+'/gen/'+f+'/gen.tsv'):
        a=l.rstrip('\n').split('\t')
        if a[1]=='OK': oldsym[a[0]]=json.loads(a[2]).get('old_sym')
tbl=list(csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'))
bysym={r['symbol']:r for r in tbl}
newpairs={}
for a in mine:
    objs=sorted(glob.glob(W+'/final/rk_part_*/work/*/*/'+a+'/'+a+'.obj')+glob.glob(W+'/fix3/rk_part_*/work/*/*/'+a+'/'+a+'.obj'),key=os.path.getmtime)
    assert objs,a
    q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3]); base=int(a,16)
    rel=RI.function_relocs('objdump',objs[-1],leaf[a],ln); assert rel is not None,a
    ps=set()
    for off,kind,sym in rel:
        if sym.startswith(RI.SKIP_PREFIXES): continue
        t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
        ps.add((sym,t))
    newpairs[a]=ps
out=['action\tsymbol\ttarget\tby_row\tnote']; added=set()
for a,ps in sorted(newpairs.items()):
    for sym,t in sorted(ps):
        if sym in bysym:
            if int(bysym[sym]['target'],16)!=t: out.append('CONFLICT\t%s\t%08x\t%s\tbound to %s by %s'%(sym,t,a,bysym[sym]['target'],bysym[sym]['address']))
        elif sym not in added:
            out.append('ADD\t%s\t%08x\t%s\tnaming wave 35b: emitted by the respelled row'%(sym,t,a)); added.add(sym)
srcs=None; ms=set(mine)
for r in tbl:
    if r['address'] in ms and (r['symbol'],int(r['target'],16)) not in newpairs[r['address']]:
        m=re.match(r'^\?{1,2}([0-9A-Z_]?)([A-Za-z_][A-Za-z_0-9]*)@',r['symbol']); ident=m.group(2) if m else r['symbol']
        if srcs is None: srcs={p:open(p,errors='replace').read() for p in glob.glob(ROOT+'/rebuild/src/compiled/*/*/*.cpp')}
        users=[p for p,s in srcs.items() if ident in s and not any(p.endswith('_'+x+'.cpp') for x in allmine)]
        if users: out.append('KEEP\t%s\t%s\t%s\tstill named by %d unedited landed source(s) (text search for %s), e.g. %s; the first binder %s no longer emits it'%(r['symbol'],r['target'],r['address'],len(users),ident,os.path.relpath(users[0],ROOT),r['address']))
        else: out.append('RETIRE\t%s\t%s\t%s\tno landed source names %s after wave 35b (was bound by %s)'%(r['symbol'],r['target'],r['address'],ident,r['address']))
# a binding that targets a row of mine under its OLD (now undefined) spelling
for r in tbl:
    if r['target'] in allmine and r['symbol']==oldsym.get(r['target']) and r['symbol']!=leaf[r['target']]:
        out.append('NOTE\t%s\t%s\t%s\tbinding of the old spelling of row %s (now defined as %s)'%(r['symbol'],r['target'],r['address'],r['target'],leaf[r['target']]))
open(sys.argv[2],'w').write('\n'.join(out)+'\n')
import collections; print(collections.Counter(l.split('\t')[0] for l in out[1:]))
