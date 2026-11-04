# final_tables.py: PASS.tsv / DEFERRED.tsv from final/rk_part_*.log; moves RELOC_NAME rows' files to overlay_inherited/; copies logs
import re,glob,collections,os,shutil
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
def parse(files):
    grade={};warn=collections.defaultdict(list);logof={}
    for f in files:
        for l in open(f):
            m=re.match(r'^([0-9a-f]{8}) +(RELOCATION_MATCH|MATCH|DIFFER|SRC_FAIL|LINK_FAIL|TEST_FAIL)\s+(\S+)',l)
            if m: grade[m.group(1)]=m.group(2)+' '+m.group(3); logof[m.group(1)]=f
            m=re.match(r'^([0-9a-f]{8}) +WARN (\S+?):? (.*)',l)
            if m and m.group(2)!='SHARED_HEADER_AVAILABLE': warn[m.group(1)].append((m.group(2),m.group(3).strip()))
    return grade,warn,logof
G,Wn,L=parse(sorted(glob.glob(W+'/final/rk_part_0*.log')))
BG,BW,_=parse(sorted(glob.glob(W+'/base/rk_part_0*.log')))
def tl(w):  # target+label part, symbol-independent
    m=re.search(r'-> (\S+) (.*)',w); return m.groups() if m else w
os.makedirs(W+'/logs',exist_ok=True)
for f in glob.glob(W+'/final/rk_part_0*.log'): shutil.copy(f,W+'/logs/final_'+os.path.basename(f))
ren={l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(W+'/RENAMES.tsv') if not l.startswith('address')}
P=['address\tgrade\tlog'];D=['address\treason'];cnt=collections.Counter()
for a in sorted(G):
    g=G[a]; ws=Wn.get(a,[])
    bad=[w for k,w in ws if k in('RELOC_NAME',) or 'leaf' in k.lower() or 'CONFLICT' in k]
    other=[k+': '+w for k,w in ws if k not in('RELOC_NAME','RELOC_ALIAS')]
    if not g.endswith('PASS') or not g.startswith(('MATCH','RELOCATION_MATCH')):
        D.append('%s\tgrade %s'%(a,g)); cnt['grade']+=1; continue
    if other:
        D.append('%s\tunexpected warning: %s'%(a,'; '.join(other))); cnt['otherwarn']+=1; continue
    if bad:
        base=set(tl(w) for k,w in BW.get(a,[]) if k=='RELOC_NAME')
        new=[w for w in bad if tl(w) not in base]
        why='inherited callee RELOC_NAME' if not new else 'callee RELOC_NAME (not all in the base grade)'
        D.append('%s\t%s: %s; bytes %s, the row respelled byte-identically (overlay_inherited/), the callee spellings are pre-existing invented names in the landed source'%(a,why,' | '.join(sorted(set(bad))),g))
        cnt[why]+=1
        r=ren[a]
        for sub,rel in (('src/compiled',r[2]),('tests',r[4])):
            s=W+'/overlay/rebuild/'+sub+'/'+rel; d=W+'/overlay_inherited/rebuild/'+sub+'/'+rel
            os.makedirs(os.path.dirname(d),exist_ok=True); shutil.move(s,d)
        continue
    P.append('%s\t%s%s\tlogs/final_%s'%(a,g,(' (RELOC_ALIAS import thunk only: '+'; '.join(w.split(' ->')[0] for k,w in ws)+')') if ws else '',os.path.basename(L[a])))
    cnt['pass']+=1
open(W+'/PASS.tsv','w').write('\n'.join(P)+'\n'); open(W+'/DEFERRED.tsv','w').write('\n'.join(D)+'\n')
print(cnt)
