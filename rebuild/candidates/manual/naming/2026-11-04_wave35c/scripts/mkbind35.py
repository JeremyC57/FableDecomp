# mkbind35.py: BINDINGS.tsv for the wave 35c rows from the final graded objects (vw_*), against rebuild/oracles/reloc_bindings.tsv
import sys,os,json,re,glob,csv,collections,io,contextlib
ROOT='/home/user/FableDecomp'
sys.path.insert(0,ROOT+'/tools/decomp_pipeline')
os.environ.setdefault('FABLE_OBJDUMP','objdump')
import verify_and_land as V, reloc_identity as RI
W=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
tbl={}; byrow=collections.defaultdict(set)
for r in csv.DictReader(open(ROOT+'/rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    tbl.setdefault(r['symbol'],(r['target'],r['address'])); byrow[r['address']].add((r['symbol'],r['target']))
fams={'sp':['vw_sp_part_00','vw_sp_part_01'],'man':['vw_final_man'],'cal':['vw_tv1','vw_final_cal']}
oracle={}
for f in ['sp/rk_part_00','sp/rk_part_01','man/pay_final_man','cal/pay_final_cal']:
    p=os.path.join(W,f,'oracle.tsv')
    if not os.path.exists(p): continue
    for r in csv.DictReader(open(p),delimiter='\t'): oracle[r['address']]=bytes.fromhex(r['bytes'])
adds=[];rets=[];newall=collections.defaultdict(set)
for fam,vws in fams.items():
    for l in open(os.path.join(W,fam,'gen.tsv')):
        a,st,js=l.rstrip('\n').split('\t'); d=json.loads(js)
        obj=None
        for vw in vws:
            g=glob.glob(os.path.join(W,vw,'*','*',a,a+'.obj'))
            if g: obj=g[0]; break
        if not obj or a not in oracle: print('skip',a,fam,file=sys.stderr); continue
        src=open(os.path.join(W,fam,'src',d['new_src'])).read()
        m=re.search(r'^//\s*Leaf:\s*(\S+)',src,re.M); leaf=m.group(1) if m else d['sym']
        with contextlib.redirect_stderr(io.StringIO()):
            built,_,_=V.obj_text(obj,leaf)
        hard,warn,pairs=RI.check(ROOT,'objdump',obj,leaf,a,oracle[a],built=built)
        new={(s,'%08x'%t) for _,_,s,t in pairs}
        for s,t in new: newall[s].add(t)
        for s,t in sorted(new):
            if s not in tbl: adds.append(['ADD',s,t,a,'naming wave 35c: emitted by the respelled row'])
        newt=collections.defaultdict(set)
        for s,t in new: newt[t].add(s)
        for s,t in sorted(byrow.get(a,())):
            if (s,t) not in new and t in newt:
                rets.append(['RETIRE',s,t,a,'naming wave 35c: row now binds %s (RETIRE-candidate: drop only if no other landed row still emits it)'%','.join(sorted(newt[t]))])
# a RETIRE whose symbol is still emitted by another wave-35c row is KEEP
rets=[r for r in rets if r[1] not in newall]
out=['action\tsymbol\ttarget\tby_row\tnote']+['\t'.join(r) for r in adds+rets]
open(os.path.join(W,'BINDINGS.tsv'),'w').write('\n'.join(out)+'\n')
print(len(adds),'ADD',len(rets),'RETIRE')
