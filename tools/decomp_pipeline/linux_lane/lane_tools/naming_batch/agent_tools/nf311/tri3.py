# tri3.py <from> <to>: per ENTRIES line (1 = first entry), each stand-in spelling with every witness binder opened: does the binder
# still name the stand-in identifier / scope tokens in code (STALE = names none of them), does it define the stand-in's scope token
# locally, and the first binder lines naming the identifier.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
R='/home/user/FableDecomp/'
ps=open(R+'rebuild/build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:i+700]); return R+'rebuild/src/compiled/'+m.group(1) if m else None
def toks(sym):
    if sym.startswith('@'): return [sym[1:].split('@')[0]]
    head=re.sub(r'\?\$','',sym).split('@@')[0]
    return [x.lstrip('?0123456789') for x in head.split('@') if x and not x.startswith('$')][:3]
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n].split('\t'); t,pdb,sp=c[0],c[6],c[11]
    print('#%d %s %s :: %s'%(n,t,c[4],pdb[:160]))
    for part in sp.split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m: continue
        sym,wit=m.group(1),m.group(2)
        if sym==pdb: print('   = PDB wit=%s'%wit[:70]); continue
        tk=[x for x in toks(sym) if len(x)>2]
        print('   S %s  wit=%s'%(sym[:170],wit[:60]))
        for w in wit.split(','):
            if w in ('self','...') or len(w)!=8: continue
            f=src(w)
            if not f: print('      %s: no landed source'%w); continue
            txt=open(f,encoding='utf-8',errors='replace').read()
            code='\n'.join(l.split('//')[0] for l in txt.splitlines())
            have=[x for x in tk if re.search(r'\b%s\b'%re.escape(x),code)]
            defs=[x for x in tk if re.search(r'\b(struct|class|namespace)\s+%s\b'%re.escape(x),code)]
            print('      %s %s lines=%d %s localdef=%s'%(w,f.split('/')[-1][:64],txt.count('\n'),'STALE' if tk and not have else 'names='+','.join(have),','.join(defs) or '-'))
            ident=tk[0] if tk else ''
            for l in [l.strip() for l in code.splitlines() if ident and re.search(r'\b%s\b'%re.escape(ident),l)][:2]: print('         | %s'%l[:170])
