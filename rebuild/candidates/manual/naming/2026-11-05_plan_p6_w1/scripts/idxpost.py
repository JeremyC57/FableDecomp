# idxpost.py [OUTDIR] : MULTINAME.tsv and QUEUE_STATUS.tsv from the symbol index ($S/plan/idx INDEX.tsv / BIND.tsv), in the phase 1
# format (naming/2026-11-04_vendor_v2/index/, its README): p1's post.py is lost, this is the plan phase 6 rewrite.
#   MULTINAME.tsv: every retail target that landed code reaches under 2 or more distinct symbols (the callers' bind symbols plus the
#     definer's own symbol); scope vendor (0xC08AE0..0xCB8000) / fable; class ICF when every spelling is a real PDB symbol (Ego_r or
#     FableWin name set: byte-identical bodies folded by the linker, genuine) else conflict.
#   QUEUE_STATUS.tsv: every NAME_FIXES_QUEUE.tsv line not marked ` done]`: satisfied (the new symbol emitted or bound and the only
#     spelling at the target), partial (emitted/bound, other spellings remain: listed), open (neither), unparseable.
import sys,os,re,csv,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
csv.field_size_limit(1<<30)
OUT=sys.argv[1] if len(sys.argv)>1 else L.P+'/idxpost'; os.makedirs(OUT,exist_ok=True)
E,F=L.pdb(); PDB=E|F; cat=L.cat(); ent=L.ent()
idx={}
for r in csv.DictReader(open(L.IDX+'/INDEX.tsv'),delimiter='\t',quoting=csv.QUOTE_NONE): idx[r['address']]=r
binds=collections.defaultdict(lambda: collections.defaultdict(set))   # target -> symbol -> callers
for r in csv.DictReader(open(L.IDX+'/BIND.tsv'),delimiter='\t',quoting=csv.QUOTE_NONE): binds[r['target']][r['symbol']].add(r['caller'])
def src(a): return ent[a]['Source'] if a in ent else a
def scope(t): v=int(t,16); return 'vendor' if 0xC08AE0<=v<0xCB8000 else 'fable'
n_multi=collections.Counter()
with open(OUT+'/MULTINAME.tsv','w') as o:
    o.write('target\tscope\tclass\tn_symbols\tn_files\tdefiner_symbol\tsymbols (count: callers; def:<addr> = the definer)\tcatalog_name\tfiles\n')
    for t in sorted(set(binds)|set(idx)):
        syms={s:set(c) for s,c in binds.get(t,{}).items()}
        dsym=idx.get(t,{}).get('symbol') or ''
        if dsym: syms.setdefault(dsym,set()).add('def:'+t)
        if len(syms)<2: continue
        cls='ICF' if all(s in PDB for s in syms) else 'conflict'
        files=sorted({src(c) for cs in syms.values() for c in cs if not c.startswith('def:')}|({src(t)} if t in ent else set()))
        parts=[]
        for s,cs in sorted(syms.items(),key=lambda kv:(-len(kv[1]),kv[0])):
            cl=sorted(cs); parts.append('%s (%d: %s%s)'%(s,len(cl),' '.join(cl[:12]),' ...' if len(cl)>12 else ''))
        n_multi[(scope(t),cls)]+=1
        o.write('\t'.join([t,scope(t),cls,str(len(syms)),str(len(files)),dsym,' || '.join(parts),cat.get(t,['','',''])[2] if t in cat else '',' '.join(files[:40])])+'\n')
print('MULTINAME',dict(n_multi))
Q=L.ROOT+'/rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv'
raw=open(Q,'rb').read().decode('utf-8'); nl='\r\n' if '\r\n' in raw else '\n'; lines=raw.split(nl)
st=collections.Counter()
with open(OUT+'/QUEUE_STATUS.tsv','w') as o:
    o.write('line\tline_sha1_12\taddress\tnew_symbol\tstatus\thow\tstill_binding_old (symbol (definer/caller addr source))\tother_spellings_at_target (symbol (callers / definer))\n')
    for i,l in enumerate(lines,1):
        if not l.strip() or ' done]' in l: continue
        q=l.split('\t'); h=L.sha12(l)
        if len(q)<3: o.write('\t'.join([str(i),h,'','','unparseable','columns','',''])+'\n'); st['unparseable']+=1; continue
        ma=re.match(r'^\s*(?:0x)?([0-9A-Fa-f]{6,8})\s*$',q[1]); new=q[2].strip()
        if not ma: o.write('\t'.join([str(i),h,q[1][:20],new,'unparseable','address','',''])+'\n'); st['unparseable']+=1; continue
        a=ma.group(1).lower().zfill(8)
        if not re.match(r'^[?@_]',new) or ' ' in new: o.write('\t'.join([str(i),h,a,new,'unparseable','new symbol not mangled','',''])+'\n'); st['unparseable']+=1; continue
        dsym=idx.get(a,{}).get('symbol') or ''; emitted=(dsym==new); bound=new in binds.get(a,{})
        old=[x.strip() for x in (q[4] if len(q)>4 else '').split(';') if x.strip()]
        still=[]
        for s in old:
            s=s.strip()
            if not re.match(r'^[?@_]',s): continue
            who=[]
            if dsym==s: who.append('definer '+src(a))
            cs=binds.get(a,{}).get(s)
            if cs: who.append('caller '+','.join(sorted(cs)))
            if who: still.append('%s (%s)'%(s,'; '.join(who)))
        others=[]
        for s,cs in sorted(binds.get(a,{}).items(),key=lambda kv:-len(kv[1])):
            if s!=new: others.append('%s (caller %s)'%(s,','.join(sorted(cs)[:8])))
        if dsym and dsym!=new: others.append('%s (definer %s)'%(dsym,src(a)))
        how='+'.join(x for x,y in (('emitted',emitted),('bound',bound)) if y)
        status='satisfied' if (emitted or bound) and not others else ('partial' if (emitted or bound) else 'open')
        st[status]+=1
        o.write('\t'.join([str(i),h,a,new,status,how,' ; '.join(still),' ; '.join(others)])+'\n')
print('QUEUE_STATUS',dict(st))
