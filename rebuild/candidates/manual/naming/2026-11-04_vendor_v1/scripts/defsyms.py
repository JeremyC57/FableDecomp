# defsyms.py <out.tsv> [ccdir] : the symbol each compiled row (cc/<addr>/x.obj) defines for its graded block
import sys,os,subprocess
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,ROOT+'/tools/decomp_pipeline')
import reloc_identity as RI
from concurrent.futures import ThreadPoolExecutor
out=sys.argv[1]; cc=sys.argv[2] if len(sys.argv)>2 else W+'/cc'
cat=catalog()
def one(a):
    o=cc+'/'+a+'/x.obj'
    if not os.path.exists(o) or a not in cat: return (a,'NOOBJ','')
    txt=open(cc+'/'+a+'/x.cpp',errors='replace').read(); leaf=leaf_of(txt)
    ln=int(cat[a][2]); d=subprocess.run(['objdump','-dr','-w',o],capture_output=True,text=True).stdout
    bl=[b for b in RI._blocks(d)]
    c=[b[2] for b in bl if b[3]==ln]
    lm=[x for x in c if leaf and leaf in x]
    if len(lm)==1: return (a,'OK',lm[0])
    if len(c)==1: return (a,'OK',c[0])
    if c: return (a,'AMBIG',c[0])
    pick=[]
    if not pick:
        c2=[b[2] for b in bl if b[1]==0 and not b[2].startswith('$L')]
        pick=[x for x in c2 if leaf and leaf in x] or (c2 if len(c2)==1 else [])
        return (a,'MERGED' if pick else 'NONE',pick[0] if pick else '')
    return (a,'OK' if len(pick)==1 else 'AMBIG',pick[0])
with ThreadPoolExecutor(8) as ex: R=list(ex.map(one,sorted(os.listdir(cc)) if True else []))
open(out,'w').write(''.join('\t'.join(r)+'\n' for r in R if r[0][:2]=='00'))
import collections; print(collections.Counter(r[1] for r in R))
