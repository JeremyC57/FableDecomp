# census303.py [lo] [hi]: NF299 ruling Q4 census of ENTRIES lines lo..hi (default 19..end). Per entry, every table spelling of the
# target other than the PDB spelling is demangled (tools/decomp_pipeline/naming/msvc_demangle.py) and its scope compared with the PDB
# spelling's scope:
#   SAME       same scope (only name / access / const / virtual / signature differ): in-place shaped, open by hand
#   FREE       a free function (no scope) standing for a member, or the reverse
#   STANDIN    the stand-in's innermost scope name is in neither PDB name set (made-up class / namespace)
#   WRONGCLASS the stand-in's scope is a real PDB scope, but another one
#   DINK       NF301: PDB spelling in the VC10 Dinkumware form (_Vector_val, _List_val, _Tree_val, ...)
# Entry verdict: census-port (every line FREE / STANDIN / WRONGCLASS), dink-equivalent (DINK), open (some SAME line), self-only (no stand-in
# table line). HDRFLAG marks an entry where a stand-in binder includes engine/<PDB class>.h and that header names the PDB identifier
# (Q4: open those). Witness flags: SO (stay-off), T3 (queue row), LK (lock held by another agent).
import re,sys,os
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as M
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF306'
S='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad'
R='/home/user/FableDecomp/'
tok=set()
for f in ('ghidra_out/egor_pdb_names.tsv','ghidra_out/fablewin_pdb_names.tsv'):
    t=open(R+f,encoding='utf-8',errors='replace').read()
    tok|=set(re.findall(r'@([A-Za-z_]\w*)@',t)); tok|=set(re.findall(r'[VUW]([A-Za-z_]\w*)@',t)); tok|=set(re.findall(r'\?\$([A-Za-z_]\w*)@',t))
T={}
for l in open(R+'rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
ps=open(R+'rebuild/build_candidates.ps1',encoding='utf-8').read()
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    m=re.search(r"\bSource = '([^']+)'",ps[i:ps.find("\n    }",i)]); return R+'rebuild/src/compiled/'+m.group(1) if m else None
so=set(open(A+'/work/stayoff.txt').read().split()); t3=set(open(A+'/work/t3rows.txt').read().split())
def lk(a):
    try: o=open(S+'/w22/nflocks/%s/owner'%a).read().strip()
    except Exception: return ''
    return '' if o in ('released','NF306') else o
DINK=('_Vector_val','_List_val','_Tree_val','_String_val','_Deque_val','_Container_base','_Iterator_base','_Tree_unchecked','_Vector_const_iterator','_List_const_iterator','_Tree_const_iterator')
def scope(s):
    d=M.demangle(s); c=d.get('cls') or ''
    return c
BI={'int','unsigned int','long','unsigned long','bool','char','unsigned char','signed char','short','unsigned short'}
def norm(x):
    x=x.replace(' const','').replace('const ','').replace('class ','').replace('struct ','').strip()
    return x
def near(a,b):
    if a==b: return True
    if a in BI and b in BI: return True
    na,nb=norm(a),norm(b)
    if na==nb: return True
    if na.rstrip('*&')==nb.rstrip('*&') and na.rstrip('*&')!='' : return True
    return False
def sig(s,P):
    d=M.demangle(s); e=M.demangle(P)
    if (d.get('ret'),d.get('params'))==(e.get('ret'),e.get('params')): return 'SIGEQ'
    pa=d.get('params') or []; pb=e.get('params') or []
    if len(pa)==len(pb) and all(near(x,y) for x,y in zip(pa,pb)) and near(d.get('ret') or '',e.get('ret') or ''): return 'NEAR'
    return 'TYPEDIFF'
def inner(c):
    c=M.strip_tmpl(c) if c else c
    parts=[p for p in re.split(r'::',c) if p]
    return parts
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
lo=int(sys.argv[1]) if len(sys.argv)>1 else 19; hi=int(sys.argv[2]) if len(sys.argv)>2 else len(L)-1
for n in range(lo,hi+1):
    c=L[n]; t=c[0]; P=c[6]; cp=scope(P); kinds=[]; det=[]; hdr=False
    if any(x in P for x in DINK): kinds.append('DINK')
    for s,w in T.get(t,[]):
        if s==P: continue
        cs=scope(s)
        if cs==cp: k='SAME_'+sig(s,P)
        elif not cs or not cp: k='FREE'
        else:
            pi=inner(cs); k='WRONGCLASS' if pi and all(p.split('<')[0] in tok or p.startswith('`') for p in pi) else 'STANDIN'
        fl=('SO' if w in so else '')+('T3' if w in t3 else '')+(('LK='+lk(w)) if lk(w) else '')
        # HDRFLAG: the witness includes engine/<PDB class>.h which names the PDB identifier
        pc=inner(cp)[-1] if inner(cp) else ''
        pid=(M.demangle(P).get('name') or '')
        f=src(w)
        if f and pc and os.path.exists(f):
            for h in re.findall(r'#include\s+"engine/(\w+)\.h"',open(f,encoding='utf-8',errors='replace').read()):
                if h==pc:
                    hp=R+'rebuild/include/engine/%s.h'%h
                    if os.path.exists(hp) and re.search(r'\b%s\b'%re.escape(pid),open(hp,encoding='utf-8',errors='replace').read()): hdr=True
        kinds.append(k); det.append('%s:%s%s:%s'%(k,w,(':'+fl) if fl else '',s[:90]))
    ks=set(kinds)
    if not det: v='self-only'
    elif 'DINK' in ks and not any(k.startswith('SAME') for k in ks): v='dink-equivalent'
    elif any(k in ('SAME_SIGEQ','SAME_NEAR') for k in ks): v='open'
    else: v='census-port'
    print('\t'.join([str(n),t,v,'HDRFLAG' if hdr else '-',P[:110],' | '.join(det)]))
