# census.py: for every new symbol in RENAMES.tsv section 2 (column 3), list the landed sources that DEFINE `Class::Method(` (or a free `Method(`
# with a body) for that name: a same-TU helper under the new name with another body would be a conflict (GOTCHAS nf265).
import re,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309/'
R='/home/user/FableDecomp/rebuild/src/compiled'
names=set()
for l in open(A+'RENAMES.tsv').read().splitlines():
    f=l.split('\t')
    if len(f)!=4 or l.startswith('#') or not f[0].startswith('0x'): continue
    m=re.match(r'\?+\$?([A-Za-z_]\w*)@(?:([A-Za-z_]\w*)@)?',f[2]) if not f[2].startswith(('??9','??0','??1','??4')) else None   # NF309: template ??$ names parsed, operators / ctors skipped (checked by hand)
    if m: names.add((m.group(1),m.group(2) or '',f[3]))
src={}
for root,_,fs in os.walk(R):
    for x in fs:
        if x.endswith('.cpp'): src[os.path.join(root,x)]=None
hits={}
for p in src:
    t=open(p,encoding='utf-8',errors='replace').read()
    for meth,cls,tgt in names:
        if meth not in t: continue
        code='\n'.join(l.split('//')[0] for l in t.splitlines())
        rx=(r'\b%s\s*::\s*%s\s*(<[^;{]*>)?\s*\([^;{]*\)\s*(const\s*)?\{'%(re.escape(cls),re.escape(meth))) if cls else (r'^[^\n;]*\b%s\s*\([^;{]*\)\s*\{'%re.escape(meth))
        if re.search(rx,code,re.M): hits.setdefault((meth,cls,tgt),[]).append(p.rsplit('/',1)[-1])
for (meth,cls,tgt) in sorted(names):
    h=hits.get((meth,cls,tgt),[])
    print('%s\t%s::%s\t%s'%(tgt,cls,meth,' '.join(h) if h else '-'))
