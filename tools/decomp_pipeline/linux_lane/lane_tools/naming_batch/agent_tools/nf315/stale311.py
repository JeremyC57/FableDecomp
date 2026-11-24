# stale311.py (NF315 copy of stalescreen.py, driven by work/census.txt: FULL table spellings): for every skipped-port / skipped-held STATUS entry, each tracker stand-in spelling (symbol, witness): is the stand-in
# still in the witness's HEAD source? Member stand-ins: every scope token of the mangled name (class / namespace names, minus ?A0x hashes)
# must occur as a word in the source. Free (@@Y / @@Z) stand-ins: the leaf must occur, and the PDB scope must NOT qualify it
# (`<ns>::leaf` or an enclosing `namespace <ns>`). extern "C" stubs: the C name must occur. Prints OK / MISS / NOSRC per spelling.
import re,subprocess,sys
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
KW=set('class struct union enum const volatile bool char short int long float double void unsigned signed __int64 std allocator operator anonymous namespace'.split())
def idents(sym):   # NF315 v2: identifiers of the demangled stand-in (scopes, name, template args, parameter types); None if undemanglable
    try: d=MD.demangle(sym)
    except Exception: return None
    if not d.get('ok'): return None
    txt=' '.join(d['comps'])+' '+' '.join(d.get('params') or [])+' '+(d.get('ret') or '')
    return d, [w for w in dict.fromkeys(re.findall(r'[A-Za-z_]\w{2,}',txt)) if w not in KW and not w.startswith('A0x')]
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
E=open(A+'/ENTRIES.tsv').read().splitlines()
def src(a):
    return subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
def scope_tokens(sym):
    m=re.match(r'\?{1,2}(?:\$?[A-Za-z_]\w*|[0-9A-Z_]{1,2})@(.*?)@@',sym)
    if not m: return None
    return [t for t in m.group(1).split('@') if t and not t.startswith('?A0x') and not t.startswith('?$') and not t.isdigit()]

import sys
lines=[l.rstrip('\n').split('\t') for l in open(A+'/work/census.txt') if l.startswith('#')]
full={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: full.setdefault((r[1].lower(),r[2].lower()),[]).append(r[0])
for c in lines:
    n=int(c[0][1:]); t=c[1]; e=E[n].split('\t'); pdb=e[6]
    pm=re.match(r'\?([A-Za-z_]\w*)@(.*?)@@',pdb); pscope=pm.group(2).split('@') if pm else []
    for w in c[2].split(','):
        if not re.fullmatch(r'[0-9a-f]{8}',w): continue
        for sym in full.get((t,w),[]):
            if sym==pdb or not sym.startswith(c[5][:60]): continue
            f=src(w)
            if not f: print('%d\t%s\tNOSRC\t%s'%(n,w,sym[:90])); continue
            tx=open(f,encoding='utf-8',errors='replace').read()
            body='\n'.join(x for x in tx.splitlines() if not x.lstrip().startswith('//'))
            if sym.startswith('@') or sym.startswith('_'):
                name=re.match(r'[@_]?(\w+?)(@\d+)?$',sym).group(1); ok=name in body; why='C name'
            else:
                r=idents(sym)
                if r is None: ok=False; why='undemangled'
                else:
                    d,ws=r; miss=[x for x in ws if not re.search(r'\b%s\b'%re.escape(x),body)]
                    leaf=d['comps'][-1].split('<')[0]; free=len(d['comps'])==1
                    qual=free and any(re.search(r'\b%s\s*::\s*%s\b'%(re.escape(s),re.escape(leaf)),body) or re.search(r'namespace\s+%s\b'%re.escape(s),body) for s in pscope if s)
                    ok=not miss and not qual; why=('free' if free else 'member')+(' missing '+','.join(miss) if miss else '')+(' PDB-scope present' if qual else '')
            print('%d\t%s\t%s\t%s\t%s'%(n,w,'OK' if ok else 'MISS',why,sym[:90]))
