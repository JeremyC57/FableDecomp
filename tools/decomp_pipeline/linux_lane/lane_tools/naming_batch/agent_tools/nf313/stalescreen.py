# stalescreen.py: for every skipped-port / skipped-held STATUS entry, each tracker stand-in spelling (symbol, witness): is the stand-in
# still in the witness's HEAD source? Member stand-ins: every scope token of the mangled name (class / namespace names, minus ?A0x hashes)
# must occur as a word in the source. Free (@@Y / @@Z) stand-ins: the leaf must occur, and the PDB scope must NOT qualify it
# (`<ns>::leaf` or an enclosing `namespace <ns>`). extern "C" stubs: the C name must occur. Prints OK / MISS / NOSRC per spelling.
import re,subprocess,sys
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
KW=set('class struct union enum const volatile bool char short int long float double void unsigned signed __int64 std allocator operator anonymous namespace'.split())
def idents(sym):   # NF313 v2: identifiers of the demangled stand-in (scopes, name, template args, parameter types); None if undemanglable
    try: d=MD.demangle(sym)
    except Exception: return None
    if not d.get('ok'): return None
    txt=' '.join(d['comps'])+' '+' '.join(d.get('params') or [])+' '+(d.get('ret') or '')
    return d, [w for w in dict.fromkeys(re.findall(r'[A-Za-z_]\w{2,}',txt)) if w not in KW and not w.startswith('A0x')]
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
E=open(A+'/ENTRIES.tsv').read().splitlines()
st={}
for l in open(A+'/STATUS.tsv').read().splitlines()[1:]:
    c=l.split('\t'); m=re.match(r'entry (\d+) ',c[4]); st[int(m.group(1))]=c[1]
def src(a):
    return subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
def scope_tokens(sym):
    m=re.match(r'\?{1,2}(?:\$?[A-Za-z_]\w*|[0-9A-Z_]{1,2})@(.*?)@@',sym)
    if not m: return None
    return [t for t in m.group(1).split('@') if t and not t.startswith('?A0x') and not t.startswith('?$') and not t.isdigit()]
for n in sorted(st):
    if not st[n].startswith('skipped'): continue
    c=E[n].split('\t'); pdb=c[6]
    pm=re.match(r'\?([A-Za-z_]\w*)@(.*?)@@',pdb); pscope=pm.group(2).split('@') if pm else []
    for part in c[11].split(' || '):
        m=re.match(r'(\S+) \(([^)]*)\)',part)
        if not m or m.group(1)==pdb: continue
        sym=m.group(1)
        for w in m.group(2).split(','):
            if w=='self' or not re.fullmatch(r'[0-9a-f]{8}',w): continue
            f=src(w)
            if not f: print('%d\t%s\tNOSRC\t%s'%(n,w,sym[:90])); continue
            t=open(f,encoding='utf-8',errors='replace').read()
            body='\n'.join(x for x in t.splitlines() if not x.lstrip().startswith('//'))
            if sym.startswith('@') or sym.startswith('_'):
                name=re.match(r'[@_]?(\w+?)(@\d+)?$',sym).group(1)
                ok=name in body; why='C name'
            else:
                r=idents(sym)
                if r is None: ok=False; why='undemangled'
                else:
                    d,ws=r; miss=[x for x in ws if not re.search(r'\b%s\b'%re.escape(x),body)]
                    leaf=d['comps'][-1].split('<')[0]
                    free=len(d['comps'])==1
                    qual=free and any(re.search(r'\b%s\s*::\s*%s\b'%(re.escape(s),re.escape(leaf)),body) or re.search(r'namespace\s+%s\b'%re.escape(s),body) for s in pscope if s)
                    ok=not miss and not qual; why=('free' if free else 'member')+(' missing '+','.join(miss) if miss else '')+(' PDB-scope present' if qual else '')
            print('%d\t%s\t%s\t%s\t%s'%(n,w,'OK' if ok else 'MISS',why,sym[:90]))
