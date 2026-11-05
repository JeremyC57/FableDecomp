# handback.py W NAME [--queue LINE:EFFECT ...] [--labels FILE] : write the wave's hand-back directory $P/handback/NAME in the
# apply_w35c.py format from W (overlay/, gate.tsv, grades.tsv, grades_before.tsv, conscheck.tsv, rows.tsv, NOTES.txt, logs/).
#   overlay/      only the PASS rows' sources / tests, plus every header of W/overlay/rebuild/include
#   RENAMES.tsv   header only (no row changes its path or module; apply_w35c copies the overlay in place)
#   PASS.tsv      address, grade, log, family, introduced_warnings, inherited_warnings (vland WARN lines new to the row vs the
#                 landed text's grade; a new STANDIN_LEAF is a defect and the row is NOT passed)
#   DEFERRED.tsv  address, stage, reason   (gate / vland / consistency / rule failures)
#   CALLERS.tsv   caller, targets, status, source, note   (every relocation whose symbol changed, from the gate)
#   BINDINGS.tsv  ADD / RETIRE lines for rebuild/oracles/reloc_bindings.tsv (RETIRE when no index binder of the old spelling is left)
#   QUEUE_LINES.tsv  line, line_sha1_12, address, effect, old_binders_respelled, old_binders_left  (--queue 62:closed ...)
#   LABELS.tsv    copied from --labels (catalog form Class::Method, with evidence)
#   NOTES.txt, logs/
# A PASS row needs: gate PASS, vland grade MATCH/RELOCATION_MATCH[...] with behaviour PASS and the same grade as the landed text
# (grades_before.tsv), conscheck OK (or absent), no new STANDIN_LEAF warning.
import sys,os,re,shutil,collections,hashlib
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; NAME=sys.argv[2]; args=sys.argv[3:]; queue=[]; labels=None
while args:
    a=args.pop(0)
    if a=='--queue':
        while args and not args[0].startswith('--'): queue.append(args.pop(0))
    elif a=='--labels': labels=args.pop(0)
H=L.P+'/handback/'+NAME; shutil.rmtree(H,ignore_errors=True); os.makedirs(H+'/logs')
gate={r['address']:r for r in L.tsv(W+'/gate.tsv')}; gr={r['address']:r for r in L.tsv(W+'/grades.tsv')}
grb={r['address']:r for r in L.tsv(W+'/grades_before.tsv')}; cons={r['address']:r for r in L.tsv(W+'/conscheck.tsv')}
rules={r['address']:r for r in L.tsv(W+'/rows.tsv')} if os.path.exists(W+'/rows.tsv') else {}
ANON=re.compile(r'\?A0x[0-9a-f]{8}')
def warnset(s): return set(x.strip() for x in (s or '').split(' | ') if x.strip())
def base_grade(g): return re.sub(r'\[.*\]$','',g or '')
cands=sorted(set(gate)|set(rules)|L.wave_rows(W) if False else set(gate)|set(rules))
passed=[];deferred=[];callers=[]
for a in cands:
    g=gate.get(a); r=rules.get(a)
    if r and r.get('rule','').startswith('UNCHANGED'): deferred.append((a,'no_rule',r['rule'])); continue
    if not g: deferred.append((a,'gate','not gated')); continue
    if g['verdict']!='PASS':
        why=g['reason']; st='compile_fail' if why.startswith('COMPILE_FAIL') else 'bytes_differ' if why.startswith(('LENGTH','BYTES')) else 'targets_differ' if why.startswith('TARGETS') else 'gate'
        deferred.append((a,st,why[:300])); continue
    v=gr.get(a)
    if not v or not base_grade(v['grade']).startswith(('MATCH','RELOCATION_MATCH')) or v['behaviour']!='PASS':
        deferred.append((a,'grade','vland '+(v['grade']+' '+v['behaviour'] if v else 'not graded'))); continue
    b=grb.get(a)
    if b and base_grade(b['grade'])!=base_grade(v['grade']): deferred.append((a,'grade','grade %s differs from the landed text %s'%(v['grade'],b['grade']))); continue
    if a in cons and not cons[a]['verdict'].startswith('OK'): deferred.append((a,'consistency',cons[a]['why'][:300])); continue
    wa=warnset(v['warnings']); wb=warnset(b['warnings']) if b else wa
    intro=sorted(wa-wb); inh=sorted(wa&wb)
    if any(x.startswith('STANDIN_LEAF') for x in intro): deferred.append((a,'standin_leaf','new STANDIN_LEAF: '+' | '.join(intro)[:300])); continue
    passed.append((a,v['grade']+' PASS'+(' INHERITED_WARN' if inh else ''),v['log'],NAME,' | '.join(intro),' | '.join(inh)))
    for ch in filter(None,[x.strip() for x in g['changes'].split(' | ')]):
        m=re.match(r'^([0-9a-f]{8}): (\S+) -> (\S+)$',ch)
        if m and ANON.sub('?A',m.group(2))!=ANON.sub('?A',m.group(3)):
            callers.append((a,m.group(1),'RESPELLED_IN_OVERLAY (%s)'%(v['grade']+' PASS'),L.ent()[a]['Source'],'%s %s -> %s'%(m.group(1),m.group(2),m.group(3))))
pa=set(x[0] for x in passed)
# overlay: passing rows' files + headers
for d,_,fl in os.walk(W+'/overlay'):
    for f in fl:
        rel=os.path.relpath(os.path.join(d,f),W+'/overlay'); keep=rel.startswith('rebuild/include/')
        if not keep:
            a=None
            for x in pa:
                if rel in (L.src_rel(x),L.tst_rel(x)): a=x; break
            keep=a is not None
        if keep: os.makedirs(os.path.dirname(H+'/overlay/'+rel),exist_ok=True); shutil.copyfile(os.path.join(d,f),H+'/overlay/'+rel)
open(H+'/RENAMES.tsv','w').write('address\told_source\tnew_source\told_test\tnew_test\tnew_module\n')
with open(H+'/PASS.tsv','w') as o:
    o.write('address\tgrade\tlog\tfamily\tintroduced_warnings\tinherited_warnings\n')
    for p in passed: o.write('\t'.join(p)+'\n')
with open(H+'/DEFERRED.tsv','w') as o:
    o.write('address\tstage\treason\n')
    for d_ in deferred: o.write('\t'.join(d_)+'\n')
with open(H+'/CALLERS.tsv','w') as o:
    o.write('caller\ttargets\tstatus\tsource\tnote\n')
    for c in callers: o.write('\t'.join(c)+'\n')
# bindings: ADD new spellings absent from the index at that target; RETIRE old spellings with no binder left
T,cal=L.index_binds(); resp=collections.defaultdict(set); newsp=collections.defaultdict(set)
for a,t,st,src,note in callers:
    m=re.match(r'^(\S+) (\S+) -> (\S+)$',note); resp[(t,m.group(2))].add(a); newsp[(t,m.group(3))].add(a)
with open(H+'/BINDINGS.tsv','w') as o:
    o.write('action\tsymbol\ttarget\tby_row\tnote\n')
    for (t,s),rows in sorted(newsp.items()):
        if s not in T.get(t,{}): o.write('\t'.join(['ADD',s,t,sorted(rows)[0],NAME+' wave: the decided spelling the respelled rows emit'])+'\n')
    for (t,s),rows in sorted(resp.items()):
        leftover=cal.get((t,s),set())-rows
        if not leftover: o.write('\t'.join(['RETIRE',s,t,sorted(rows)[0],NAME+' wave: no landed binder left'])+'\n')
        else: o.write('\t'.join(['KEEP',s,t,sorted(rows)[0],NAME+' wave: %d binder(s) still bind it: %s'%(len(leftover),','.join(sorted(leftover)[:6]))])+'\n')
# queue lines
Q=open(L.ROOT+'/rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv','rb').read().decode('utf-8'); nl='\r\n' if '\r\n' in Q else '\n'; QL=Q.split(nl)
with open(H+'/QUEUE_LINES.tsv','w') as o:
    o.write('line\tline_sha1_12\taddress\teffect\told_binders_respelled\told_binders_left\n')
    for q in queue:
        ln,eff=q.split(':',1); l=QL[int(ln)-1]; f=l.split('\t'); addr=f[1].lower().replace('0x','') if len(f)>1 else ''
        n_resp=sum(len(v) for (t,s),v in resp.items() if t==addr); n_left=sum(len(cal.get((t,s),set())-v) for (t,s),v in resp.items() if t==addr)
        o.write('\t'.join([ln,L.sha12(l),addr,eff,str(n_resp),str(n_left)])+'\n')
if labels: shutil.copyfile(labels,H+'/LABELS.tsv')
else: open(H+'/LABELS.tsv','w').write('address\tname\tevidence\n')
if os.path.exists(W+'/NOTES.txt'): shutil.copyfile(W+'/NOTES.txt',H+'/NOTES.txt')
for f in os.listdir(W+'/logs') if os.path.isdir(W+'/logs') else []: shutil.copyfile(W+'/logs/'+f,H+'/logs/'+f)
for f in ('gate.tsv','grades.tsv','grades_before.tsv','conscheck.tsv','targets_after.tsv','rows.tsv'):
    if os.path.exists(W+'/'+f): shutil.copyfile(W+'/'+f,H+'/logs/'+f)
print('handback',H,'PASS',len(passed),'DEFERRED',len(deferred),collections.Counter(d[1] for d in deferred),'callers',len(callers))
