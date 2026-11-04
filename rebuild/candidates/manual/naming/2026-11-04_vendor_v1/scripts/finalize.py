# finalize.py: final PASS / DEFERRED / CALLERS / BINDINGS from PASS.tsv (graded) + consistency checks; prunes the overlay
import sys,os,shutil
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
ent=ps1_entries(); cat=catalog()
rows={r['address']:r for r in json.load(open(W+'/rows.json'))}
P=[l.rstrip('\n').split('\t') for l in list(open(W+'/PASS.tsv'))[1:]]
win={l.split('\t')[0]:l.split('\t')[1] for l in list(open(W+'/DECISIONS_V1.tsv'))[1:]}
stale=dict(l.rstrip('\n').split('\t',1) for l in open(W+'/logs/stale_decision.tsv'))
defs0={l.split('\t')[0]:l.rstrip('\n').split('\t')[2] for l in open(W+'/logs/defs_base.tsv')}
defs1={l.split('\t')[0]:l.rstrip('\n').split('\t')[2] for l in open(W+'/logs/defs_new.tsv')}
for a,s in defs1.items():
    if s!=defs0.get(a) and win.get(a)!=s: stale[a]='own symbol %s, decision %s'%(s,win.get(a))
def scan(f):
    d=collections.defaultdict(list)
    for l in open(f):
        q=l.rstrip('\n').split('\t')
        if q[1]=='OK' and q[2]: d[q[0]].append((q[2],q[3]))
    return d
B=scan(W+'/logs/scan_base.tsv'); N=scan(W+'/logs/scan_new.tsv')
JUNK=re.compile(r'CAIState|CV_|CQ_|NScript')
keep=[]; defer=[]
for q in P:
    a,g,log,fam,new,inh=q
    newsyms=[s1 for (s0,t0),(s1,t1) in zip(B.get(a,[]),N.get(a,[])) if s0!=s1]+([defs1[a]] if a in defs1 and defs1[a]!=defs0.get(a) else [])
    if a in stale: defer.append((a,'decision changed after grading (one address, one name / junk filter): '+stale[a])); continue
    if any(JUNK.search(s) for s in newsyms): defer.append((a,'respell adopts a winner with engine vocabulary: '+', '.join(s for s in newsyms if JUNK.search(s)))); continue
    if fam=='strm':
        if new and not all('ResetMediaTime@CRendererPosPassThru' in w for w in new.split(' | ')): defer.append((a,'introduced warning: '+new)); continue
        keep.append((a,g.replace(' INTRODUCED_WARN',' LABEL_PENDING') if new else g,log,fam,new,inh)); continue
    if new:
        defer.append((a,'introduced warning on a decided placeholder spelling (the catalog label is propagated; strict rule: no RELOC_NAME/CONFLICT): '+new)); continue
    keep.append(tuple(q))
for l in open(W+'/logs/grade_fail.tsv'):
    a,r=l.rstrip('\n').split('\t'); defer.append((a,'grade failed: '+r))
K={k[0] for k in keep}
# prune overlay to kept rows
for a,r in rows.items():
    if a in K: continue
    for p in (W+'/overlay/rebuild/src/compiled/'+r['src'],W+'/overlay/rebuild/tests/'+r['test']):
        if os.path.exists(p): os.remove(p)
ren=[l for l in open(W+'/RENAMES.tsv')]
open(W+'/RENAMES.tsv','w').write(ren[0]+''.join(l for l in ren[1:] if l.split('\t')[0] in K))
with open(W+'/PASS.tsv','w') as o:
    o.write('address\tgrade\tlog\tfamily\tintroduced_warnings\tinherited_warnings\n')
    for k in keep: o.write('\t'.join(k)+'\n')
# respell-level deferrals (not graded): the respell pipeline's rejects
rs=[l.rstrip('\n').split('\t') for l in open(W+'/rs_t7/result.tsv')]
with open(W+'/DEFERRED.tsv','w') as o:
    o.write('address\tstage\treason\n')
    for a,r in defer: o.write('%s\tgraded\t%s\n'%(a,r))
    for q in rs:
        if q[1]=='OK': continue
        o.write('%s\trespell\t%s: %s %s\n'%(q[0],q[1],q[2][:300],q[3][:300] if len(q)>3 else ''))
# CALLERS
with open(W+'/CALLERS.tsv','w') as o:
    o.write('caller\ttargets\tstatus\tsource\tnote\n')
    for k in keep:
        a=k[0]; r=rows[a]
        ch=sorted(set(t1 for (s0,t0),(s1,t1) in zip(B.get(a,[]),N.get(a,[])) if s0!=s1))
        if r['fam']=='auto' and ch:
            o.write('%s\t%s\tRESPELLED_IN_OVERLAY (%s)\t%s\tcallee spellings unified to the DECISIONS_V1 winner\n'%(a,','.join(ch),k[1],r['src']))
# BINDINGS
tbl=collections.defaultdict(set); first={}
for b in bindings():
    tbl[b['symbol']].add(b['target'].lower()); first.setdefault(b['symbol'],b['address'])
after=collections.defaultdict(set)
for a,ps in B.items():
    src=N if a in K and a in N else B
    for s,t in src[a]: after[s].add(t)
for a,s in defs0.items():
    s2=defs1.get(a,s) if a in K else s
    if s2: after[s2].add(a)
out=['action\tsymbol\ttarget\tby_row\tnote']; seen=set()
for k in keep:
    a=k[0]
    pairs=[(s1,t1) for (s0,t0),(s1,t1) in zip(B.get(a,[]),N.get(a,[])) if s0!=s1]
    if a in defs1 and defs1[a]!=defs0.get(a): pairs.append((defs1[a],a))
    if rows[a]['fam']=='strm': pairs.append((rows[a]['leaf'],a))
    for s,t in pairs:
        if (s,t) in seen: continue
        seen.add((s,t))
        if s in tbl:
            if t not in tbl[s]: out.append('CONFLICT\t%s\t%s\t%s\tbound to %s'%(s,t,a,','.join(sorted(tbl[s]))))
        else: out.append('ADD\t%s\t%s\t%s\tvendor naming wave V1: %s spelling of 0x%s emitted by the respelled row'%(s,t,a,'proven' if t in win and any(l.startswith(t+'\t') and '\tproven\t' in l for l in open(W+'/DECISIONS_V1.tsv')) else 'decided placeholder',t.upper()))
for l in list(open(W+'/DECISIONS_V1.tsv'))[1:]:
    q=l.rstrip('\n').split('\t'); t=q[0]
    for x in (q[5].split(' | ') if len(q)>5 and q[5] else []):
        s=re.sub(r'\[[^\]]*\]$','',x)
        if s in tbl and t in tbl[s] and t not in after.get(s,set()):
            out.append('RETIRE\t%s\t%s\t%s\tno landed row emits it for 0x%s after the overlay (losing spelling of the DECISIONS_V1 winner %s)'%(s,t,first.get(s,''),t.upper(),q[1]))
open(W+'/BINDINGS.tsv','w').write('\n'.join(out)+'\n')
print('keep',len(keep),collections.Counter(k[1] for k in keep)); print('defer graded',len(defer)); print(collections.Counter(l.split('\t')[0] for l in out[1:]))
