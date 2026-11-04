# mkbind.py: ADD / CONFLICT / RETIRE / KEEP lines for reloc_bindings.tsv from the final (g2) objects; objdump -t symbol check
import sys,os,re,glob,struct,csv,json,subprocess,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
from common import *
import reloc_identity as RI
def nc(t): return re.sub(r'//[^\n]*','',t)
W=os.environ['S']+'/wave35d'; G=sys.argv[1]
cat=catalog(); E,F=pdb()
rows={l.split('\t')[1]:json.loads(l.rstrip('\n').split('\t')[2]) for l in open(W+'/all.tsv')}
passed=[l.split('\t')[0] for l in open(W+'/PASS.tsv') if re.match(r'^[0-9a-f]{8}\t',l)]
tbl=list(csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'))
bysym={}
for r in tbl: bysym.setdefault(r['symbol'],r)
out=['action\tsymbol\ttarget\tby_row\tnote']; chk=[]; newpairs={}; added=set()
for a in sorted(passed):
    d=rows[a]
    objs=sorted(glob.glob(W+'/vw_'+G+'/**/'+a+'.obj',recursive=True),key=os.path.getmtime)
    o=objs[-1]
    leaf=d.get('sym')
    st=subprocess.run(['objdump','-t',o],capture_output=True,text=True).stdout
    defined=[l.split()[-1] for l in st.splitlines() if re.search(r'\(sec\s+[1-9]',l) and '(scl   2)' in l]
    m=[x for x in defined if leaf and leaf in x] if leaf else []
    if not leaf:
        s=open(W+'/overlay/rebuild/src/compiled/'+d['new_src']).read(); mm=re.search(r'^// Leaf: (\S+)',s,re.M); leaf=mm.group(1) if mm else cat[a][1].split('::')[-1]
        m=[x for x in defined if leaf in x]
    chk.append((a,'DEFINES' if leaf in defined else ('SUBSTR:'+','.join(m) if m else 'MISSING'),leaf,'pdb' if leaf in E or leaf in F else 'nonpdb'))
    sym=leaf if leaf in defined else (m[0] if len(m)==1 else (max(m,key=len) if m else leaf))
    q=cat[a]; ln=int(q[2]); b=bytes.fromhex(q[3]); base=int(a,16)
    rel=RI.function_relocs('objdump',o,sym,ln)
    ps=set()
    for off,kind,s_ in (rel or []):
        if s_.startswith(RI.SKIP_PREFIXES): continue
        t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
        ps.add((s_,t))
    newpairs[a]=ps
    for s_,t in sorted(ps):
        if s_ in bysym:
            if int(bysym[s_]['target'],16)!=t: out.append('CONFLICT\t%s\t%08x\t%s\tbound to %s by %s'%(s_,t,a,bysym[s_]['target'],bysym[s_]['address']))
        elif s_ not in added:
            added.add(s_)
            if s_ in E or s_ in F: out.append('ADD\t%s\t%08x\t%s\tnaming wave 35d: emitted by the respelled row (exact PDB spelling: %s)'%(s_,t,a,'/'.join(x for x,y in (('Ego_r',E),('FableWin',F)) if s_ in y)))
            elif s_.startswith('??_7') and s_.endswith('@@6B@') and ('??1'+s_[4:-5]+'@@UAE@XZ' in E or '??1'+s_[4:-5]+'@@UAE@XZ' in F):
                out.append('ADD\t%s\t%08x\t%s\tnaming wave 35d: vtable of the PDB class (??1%s@@UAE@XZ in the PDB name sets), emitted by the respelled row (wave 35b precedent)'%(s_,t,a,s_[4:-5]))
            else: out.append('NOTE\t%s\t%08x\t%s\tnew relocation symbol, not an exact PDB spelling: not added'%(s_,t,a))
srcs=None; ms=set(passed)
olds={'rebuild/src/compiled/'+d['old_src'] for d in rows.values()}
for r in tbl:
    if r['address'] in ms and (r['symbol'],int(r['target'],16)) not in newpairs[r['address']]:
        sy=r['symbol']; body=sy[4:] if sy.startswith('??_') else (sy[3:] if sy.startswith('??') else sy[1:])
        ident=re.match(r'[A-Za-z_][A-Za-z_0-9]*',body).group(0)
        d_=rows[r['address']]
        olds_=nc(open(ROOT+'/rebuild/src/compiled/'+d_['old_src'],errors='replace').read()); news_=nc(open(W+'/overlay/rebuild/src/compiled/'+d_['new_src']).read())
        if not (ident in olds_ and ident not in news_): continue   # not a spelling this wave removed from the row
        if srcs is None:
            srcs={os.path.relpath(p,ROOT):nc(open(p,errors='replace').read()) for p in glob.glob(ROOT+'/rebuild/src/compiled/*/*/*.cpp')}
            for p in glob.glob(W+'/overlay/rebuild/src/compiled/*/*/*.cpp'): srcs[os.path.relpath(p,W+'/overlay')]=nc(open(p).read())
        users=[p for p,s in srcs.items() if ident in s and not (p in olds and not os.path.exists(W+'/overlay/'+p))]
        users=[p for p in users if not (p in olds and os.path.exists(W+'/overlay/'+p) and ident not in nc(open(W+'/overlay/'+p).read()))]
        if users: out.append('KEEP\t%s\t%s\t%s\tstill named by %d landed source(s) after the overlay (text search for %s), e.g. %s; the first binder %s no longer emits it'%(r['symbol'],r['target'],r['address'],len(users),ident,users[0],r['address']))
        else: out.append('RETIRE\t%s\t%s\t%s\tno landed source names %s after wave 35d (was bound by %s)'%(r['symbol'],r['target'],r['address'],ident,r['address']))
open(W+'/BINDINGS.tsv','w').write('\n'.join(out)+'\n')
open(W+'/logs/objdump_symbols.tsv','w').write('\n'.join('\t'.join(x) for x in chk)+'\n')
print(collections.Counter(l.split('\t')[0] for l in out[1:]), collections.Counter(x[1].split(':')[0] for x in chk))
