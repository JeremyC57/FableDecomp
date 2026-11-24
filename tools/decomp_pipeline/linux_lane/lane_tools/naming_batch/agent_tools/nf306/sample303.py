# sample303.py <census.tsv> <out>: the Q4 open sample. Picks every 10th census-port entry (by census order) plus every HDRFLAG entry, and for
# each of its stand-in table lines prints the witness row, the demangled stand-in, and the witness source lines (comment-stripped) that
# declare the stand-in's innermost scope (`class|struct|namespace <Scope>`) and that name the stand-in identifier: a reading aid for
# confirming by eye that the binder models the stand-in scope itself (port) rather than reaching the PDB member another way.
import re,sys
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as M
R='/home/user/FableDecomp/rebuild/'
ps=open(R+'build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'src/compiled/'+m.group(1) if m else None
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1])]
cp=[r for r in rows if r[2]=='census-port']
pick=[r for i,r in enumerate(cp) if i%10==0]+[r for r in cp if r[3]=='HDRFLAG' and r not in [x for i,x in enumerate(cp) if i%10==0]]
out=open(sys.argv[2],'w')
for r in pick:
    out.write('#%s %s %s %s\n'%(r[0],r[1],r[3],r[4][:100]))
    for d in r[5].split(' | ')[:3]:
        k,w,rest=d.split(':',2)
        s=rest.split(':')[-1] if rest[:2] in ('SO','T3','LK') else rest
        dm=M.demangle(s); cls=M.strip_tmpl(dm.get('cls') or '') ; inner=(cls.split('::')[-1] if cls else ''); name=dm.get('name') or ''
        f=src(w); out.write('  %s %s  %s\n'%(k,w,dm.get('qual')))
        if not f: out.write('     NOSRC\n'); continue
        L=[l.split('//')[0] for l in open(f,encoding='utf-8',errors='replace').read().split('\n')]
        hits=[]
        for i,l in enumerate(L):
            if inner and re.search(r'\b(class|struct|namespace)\s+%s\b'%re.escape(inner),l): hits.append('L%d: %s'%(i+1,l.strip()[:110]))
        nm=re.sub(r'[<>~]','',name.split('<')[0])
        for i,l in enumerate(L):
            if nm and re.search(r'\b%s\b'%re.escape(nm),l) and re.search(r'[;{]\s*$',l): hits.append('L%d: %s'%(i+1,l.strip()[:110])); break
        for h in hits[:3]: out.write('     %s\n'%h)
        if not hits: out.write('     (no declaring line found)\n')
out.close(); print(len(pick),'entries sampled')
