# census309.py <first> <last>: per ENTRIES line, classify every non-PDB table line (stand-in binder) by the scope it binds:
#   SAMECLS   same class / namespace scope as the PDB spelling (name, access, const or parameter fix: open it)
#   NOTPDB    the binder's class / scope token is in neither PDB name set (stand-in class / free stand-in: port, NF299 Q4)
#   WRONGCLS  a PDB class but not the PDB spelling's class (port, NF299 Q4)
#   FREE      a free function / data spelling where the PDB is a member (port)
# and flags stay-off (SO) / T3 / T3M witnesses. Entry verdict: CENSUS_PORT when no line is SAMECLS and none is stay-off. NF314.
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
G='/home/user/FableDecomp/ghidra_out/'
so=dict(l.split('\t') for l in open(A+'/work/stayoff_why.tsv').read().splitlines())
t3=set(open(A+'/work/t3rows.txt').read().split()); t3m=set(open(A+'/work/t3mention.txt').read().split())
names=set()
for f in ('egor_pdb_names.tsv','fablewin_pdb_names.tsv'):
    for l in open(G+f,encoding='utf-8',errors='replace'):
        p=l.rstrip('\n').split('\t')
        if len(p)>1: names.add(p[1])
scopes=set()
for s in names:
    m=re.match(r'\?+[^@]*@((?:[A-Za-z_]\w*@)+)@',s)
    if m:
        parts=m.group(1).rstrip('@').split('@'); scopes.update(parts)
def scope(s):
    if s.startswith('??$') or s.startswith('?$'): return None   # template instances: open by hand
    m=re.match(r'\?+(?:[0-9A-Z_]|[^@?]+)@((?:[A-Za-z_]\w*@)*)@',s) if s.startswith('?') else None
    if not m: return ('FREE',)
    return tuple(m.group(1).rstrip('@').split('@')) if m.group(1) else ('FREE',)
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n]; t=c[0]; P=c[6]; ps=scope(P) if P else None; out=[]; verdict='CENSUS_PORT'; hold=False
    for s,w in T.get(t,[]):
        if s==P: continue
        sc=scope(s); fl=('SO' if w in so else '')+('T3' if w in t3 else ('T3M' if w in t3m else ''))
        if sc is None: k='TMPL'
        elif sc==('FREE',): k='FREE' if ps!=('FREE',) else 'SAMECLS'
        elif ps and sc==ps: k='SAMECLS'
        elif any(x not in scopes for x in sc): k='NOTPDB'
        else: k='WRONGCLS'
        if k in ('SAMECLS','TMPL'): verdict='OPEN'
        if fl.startswith('SO') or fl=='T3': hold=True
        out.append('%s:%s%s'%(w,k,(':'+fl) if fl else ''))
    if not out: verdict='NO_SI_LINE'
    if hold and verdict=='CENSUS_PORT': verdict='CENSUS_PORT+HELD'
    print('%d\t%s\t%s\t%s'%(n,t,verdict,' '.join(out)))
