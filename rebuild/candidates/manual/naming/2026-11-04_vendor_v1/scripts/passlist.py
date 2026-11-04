# passlist.py : PASS.tsv / DEFERRED (grade failures) from logs/recheck_*.log; warnings split into inherited (the landed source's own
# object has the same warning: logs from reloc_identity.check on cc/<addr>/x.obj) and introduced (a symbol this wave made the row emit)
import sys,os,glob
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,ROOT+'/tools/decomp_pipeline'); os.chdir(ROOT)
import reloc_identity as RI
cat=catalog(); rows={r['address']:r for r in json.load(open(W+'/rows.json'))}
res={}; warns=collections.defaultdict(list)
for f in sorted(glob.glob(W+'/logs/recheck_*.log')):
    for l in open(f,errors='replace'):
        m=re.match(r'^([0-9a-f]{8})\s+WARN (.*)$',l.rstrip())
        if m: warns[m.group(1)].append(m.group(2)); continue
        m=re.match(r'^([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER\S*|SRC_FAIL|OBJDUMP_ERR|\S+)\s+(\S+)\s',l)
        if m and m.group(1) in rows: res[m.group(1)]=(m.group(2),m.group(3),os.path.basename(f))
def base_warns(a):
    o=W+'/cc/'+a+'/x.obj'
    if not os.path.exists(o): return set()
    lf=leaf_of(open(W+'/cc/'+a+'/x.cpp',errors='replace').read())
    retail=bytes.fromhex(cat[a][3].strip())
    try: h,w,p=RI.check(ROOT,'objdump',o,lf,a,retail)
    except Exception as ex: return set()
    return set(w)|set(h)
out=[]; bad=[]
for a,r in sorted(rows.items()):
    if a not in res: bad.append((a,'NOT_GRADED')); continue
    par,beh,log=res[a]
    if par not in ('MATCH','RELOCATION_MATCH') or beh!='PASS': bad.append((a,'GRADE %s %s (%s)'%(par,beh,log))); continue
    bw=base_warns(a)
    ent_=ps1_entries() if 'ENT' not in globals() else ENT
    globals()['ENT']=ent_
    osrc=open(ROOT+'/rebuild/src/compiled/'+ent_[a]['Source'],errors='replace').read()
    def inherited(w):
        if w in bw: return True
        m=re.match(r'SHARED_HEADER_AVAILABLE (\w+)',w)
        return bool(m and re.search(r'\b(struct|class)\s+%s\b'%m.group(1),osrc))
    inh=[w for w in warns[a] if inherited(w)]; new=[w for w in warns[a] if not inherited(w)]
    g='%s PASS'%par
    if new: g+=' INTRODUCED_WARN'
    elif inh: g+=' INHERITED_WARN'
    out.append((a,g,'logs/'+log,r['fam'],' | '.join(new),' | '.join(inh)))
with open(W+'/PASS.tsv','w') as o:
    o.write('address\tgrade\tlog\tfamily\tintroduced_warnings\tinherited_warnings\n')
    for x in out: o.write('\t'.join(x)+'\n')
open(W+'/logs/grade_fail.tsv','w').write(''.join('%s\t%s\n'%x for x in bad))
print(collections.Counter(x[1] for x in out), len(bad))
