import json, os, re, collections
L='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/lc2/'; R='/home/user/FableDecomp/rebuild/include/'; B0=L+'b0/rebuild/include/'
items=json.load(open(L+'items.json'))
PIN=re.compile(r'FABLE_STATIC_ASSERT_AT\(\s*\d+\s*,\s*')
rd=lambda p: open(p,encoding='latin-1').read().replace('\r\n','\n') if os.path.exists(p) else ''
def toks(t, code_only=False):
    c=collections.Counter()
    for l in t.split('\n'):
        s=re.sub(r'\s+',' ',PIN.sub('FABLE_STATIC_ASSERT(',l.strip()))
        if code_only: s=re.sub(r'\s*//.*$','',s).strip()
        if s: c[s]+=1
    return c
trivial=re.compile(r'^(#\s*(endif|else)\b.*|[{}];?|public:|private:|protected:|\};|)$')
bal=0; dups=0; miss=0
for rel,v in sorted(items.items()):
    fin=rd(L+'fin/'+rel); head=rd(R+rel); base=rd(B0+rel)
    srcs=[rd(x[2] if x[0]=='new' else x[3]) for x in v]
    # balance
    d=0; ok=True
    for l in fin.split('\n'):
        s=l.strip()
        if re.match(r'#\s*if',s): d+=1
        elif re.match(r'#\s*endif',s): d-=1
        if d<0: ok=False
    if d or not ok: bal+=1; print('UNBALANCED',rel,d)
    # code lines beyond the most any single source has (HEAD or one folder) -- duplicated code
    ft=toks(fin,True); exp=toks(head,True)
    for s in srcs: exp|=toks(s,True)
    extra={k:c for k,c in (ft-exp).items() if not trivial.match(k) and not k.startswith('#')}
    if extra: dups+=1; print('EXTRA',rel,list(extra.items())[:4])
    # completeness: code lines each folder added over B0 must be in fin
    bt=toks(base,True)
    for x,s in zip(v,srcs):
        add=toks(s,True)-bt
        m={k:c for k,c in (add-ft).items() if not k.startswith('#') and not trivial.match(k)}
        if m: miss+=1; print('MISSING',rel,x[1][-10:],list(m.items())[:3])
print('unbalanced',bal,'extra',dups,'missing',miss)
