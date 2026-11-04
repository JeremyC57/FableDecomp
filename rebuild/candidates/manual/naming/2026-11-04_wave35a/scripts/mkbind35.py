# mkbind35.py: ADD / RETIRE / CONFLICT lines for reloc_bindings.tsv from the final recheck objects (wave 33 mkbind.py logic); objdump -t symbol check
import sys,os,re,json,glob,struct,csv,collections,subprocess
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline')
from lib35 import *
import reloc_identity as RI
cat=catalog()
rows={l.split('\t')[1]:json.loads(l.rstrip('\n').split('\t')[2]) for l in open(W+'/all_ok.tsv')}
passed=set(l.split('\t')[2] for l in open(W+'/final/pass.tsv') if l.startswith('PASS'))
tbl=list(csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'))
bysym={}
for r in tbl: bysym.setdefault(r['symbol'],r)
out=['action\tsymbol\ttarget\tby_row\tnote']; symchk=[]; added=set()
for a in sorted(passed):
    d=rows[a]
    objs=sorted(glob.glob(W+'/vw_final_*/*/*/'+a+'/'+a+'.obj'),key=os.path.getmtime)
    if not objs: symchk.append((a,'NO_OBJ')); continue
    o=objs[-1]
    st=subprocess.run(['objdump','-t',o],capture_output=True,text=True).stdout
    defined=[l.split()[-1] for l in st.splitlines() if re.search(r'\(sec\s+[1-9]',l) and '(scl   2)' in l]
    symchk.append((a,'DEFINES' if d['sym'] in defined else 'MISSING',d['sym']))
    ln=int(cat[a][2]); b=bytes.fromhex(cat[a][3]); base=int(a,16)
    rel=RI.function_relocs('objdump',o,d['sym'],ln)
    if rel is None: symchk.append((a,'NO_RELOCS_BLOCK')); continue
    for off,kind,sym in rel:
        if sym.startswith(RI.SKIP_PREFIXES): continue
        t=(base+off+4+struct.unpack_from('<i',b,off)[0])&0xffffffff if kind in ('REL32','DISP32') else struct.unpack_from('<I',b,off)[0]
        if sym in bysym:
            if int(bysym[sym]['target'],16)!=t: out.append('CONFLICT\t%s\t%08x\t%s\tbound to %s by %s'%(sym,t,a,bysym[sym]['target'],bysym[sym]['address']))
        elif sym not in added:
            out.append('ADD\t%s\t%08x\t%s\tnaming wave 35: emitted by the respelled row'%(sym,t,a)); added.add(sym)
for r in tbl:
    if r['address'] in passed: out.append('CHECK\t%s\t%s\t%s\tbinding first-bound by a respelled row'%(r['symbol'],r['target'],r['address']))
open(W+'/BINDINGS.tsv','w').write('\n'.join(out)+'\n')
open(W+'/logs/objdump_symbols.tsv','w').write('\n'.join('\t'.join(x) for x in symchk)+'\n')
print(collections.Counter(l.split('\t')[0] for l in out[1:]), collections.Counter(x[1] for x in symchk))
