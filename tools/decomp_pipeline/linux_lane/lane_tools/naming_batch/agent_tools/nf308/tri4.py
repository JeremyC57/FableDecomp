# tri4.py <from> <to>: like tri3.py, but per (entry, stand-in, witness binder) prints one line with a verdict:
#   IDMISSING  = the stand-in's identifier token (first token) is absent from the binder's comment-stripped code (stale candidate)
#   ANYMISSING = some other token absent
#   NOTOKENS   = the spelling has no usable stand-in token (template / operator spelling: judge by objbind)
#   LIVE       = all tokens present
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF308'
R='/home/user/FableDecomp/'
ps=open(R+'rebuild/build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    j=ps.find("[pscustomobject]",i); blk=ps[i:j if j>0 else i+700]   # NF308: bounded to the row's own block
    m=re.search(r"\bSource = '([^']+)'",blk); return R+'rebuild/src/compiled/'+m.group(1) if m else None
def toks(sym):
    if sym.startswith('@'): return [sym[1:].split('@')[0]]
    head=re.sub(r'\?\$','',sym).split('@@')[0]
    return [x.lstrip('?0123456789') for x in head.split('@') if x and not x.startswith('$')][:3]
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n].split('\t'); t,pdb,sp=c[0],c[6],c[11]
    for part in sp.split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m: continue
        sym,wit=m.group(1),m.group(2)
        if sym==pdb: continue
        tk=[x for x in toks(sym) if len(x)>2]
        for w in wit.split(','):
            if w in ('self','...') or len(w)!=8: continue
            f=src(w)
            if not f: print('#%d\t%s\t%s\tNOSRC\t%s'%(n,t,w,sym[:120])); continue
            txt=open(f,encoding='utf-8',errors='replace').read()
            code='\n'.join(l.split('//')[0] for l in txt.splitlines())
            have=[x for x in tk if re.search(r'\b%s\b'%re.escape(x),code)]
            if not tk: v='NOTOKENS'
            elif tk[0] not in have: v='IDMISSING'
            elif len(have)<len(tk): v='ANYMISSING'
            else: v='LIVE'
            print('#%d\t%s\t%s\t%s\t%s\tmissing=%s'%(n,t,w,v,sym[:120],','.join(x for x in tk if x not in have)))
