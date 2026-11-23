# census311.py <from> <to>: NF299 Q4 census per (entry, stand-in, witness). Tokens = identifiers of the demangled stand-in (class / namespace
# path, template arguments, parameter types); MADEUP = tokens found in neither PDB name set (ghidra_out/egor_pdb_names.tsv /
# fablewin_pdb_names.tsv, any mangled-name token); WRONGCLASS = the stand-in's owning class differs from the PDB spelling's; FREE = a
# free function / C stub for a member (or the reverse); HASH = an anonymous-namespace ?A0x hash (TU-path dependent, GOTCHAS 357/471/501);
# else the mdiff2 class. Prints one line per stand-in plus the witness off-limits reasons; the verdict per entry is `port (census)` when
# every stand-in is MADEUP / WRONGCLASS / FREE / HASH / SIG, otherwise OPEN (read by hand).
import re,sys,collections
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
sys.path.insert(0,A+'/tools'); 
tok=set()
for p in ('/home/user/FableDecomp/ghidra_out/egor_pdb_names.tsv','/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv'):
    for l in open(p,encoding='utf-8',errors='replace'):
        tok.update(re.findall(r'[A-Za-z_]\w+',l.split('\t')[-1]))
why=collections.defaultdict(set)
for l in open(A+'/work/offwhy.tsv'):
    a,r=l.split(); why[a].add(r)
KW=set('class struct union enum const volatile bool char short int long float double void unsigned signed __int64 std allocator operator anonymous namespace __thiscall __fastcall __cdecl __stdcall'.split())
def dm(s):
    try: d=MD.demangle(s)
    except Exception: return None
    return d if d.get('ok') else None
def idents(d):
    txt=' '.join(d['comps'])+' '+' '.join(d.get('params') or [])+' '+(d.get('ret') or '')
    return [w for w in dict.fromkeys(re.findall(r'[A-Za-z_]\w+',txt)) if w not in KW]
L=open(A+'/ENTRIES.tsv').read().splitlines()
# NF315 v2: the ENTRIES spellings column is cut at 160 characters, so take the FULL stand-in spellings from reloc_bindings.tsv (target match)
T=collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T[r[1].lower()].append((r[0],r[2]))
lo,hi=int(sys.argv[1]),int(sys.argv[2]); skip=set(int(x) for x in sys.argv[3].split(',')) if len(sys.argv)>3 else set()
for n in range(lo,hi+1):
    if n in skip: continue
    c=L[n].split('\t'); t,pdb,sp=c[0],c[6],c[11]; P=dm(pdb); tags=[]
    for s,w in T.get(t,[]):
        if s==pdb: continue
        if '?A0x' in s: v='HASH'
        elif s.startswith('@') or s.startswith('_') or re.match(r'\?\w+@@Y',s) and P and P['comps'][:-1]: v='FREE'
        else:
            d=dm(s)
            if not d: v='UNDEM'
            else:
                mu=[x for x in idents(d) if x not in tok]
                if mu: v='MADEUP:'+','.join(mu[:3])
                elif P and d['comps'][:-1] and P['comps'][:-1] and d['comps'][-2].split('<')[0]!=P['comps'][-2].split('<')[0]: v='WRONGCLASS:'+d['comps'][-2][:40]
                elif P and (d.get('params') or [])!=(P.get('params') or []): v='SIG'
                else: v='OPEN'
        wo=','.join(sorted(set(r for x in w.split(',') for r in why.get(x,())))) or '-'
        tags.append(v.split(':')[0]); print('#%d\t%s\t%s\t%s\tW:%s\t%s'%(n,t,w,v,wo,s[:100]))
    ver='port' if tags and all(x in ('MADEUP','WRONGCLASS','FREE','HASH','SIG') for x in tags) else 'OPEN'
    print('=%d\t%s\t%s\t%s'%(n,t,ver,'/'.join(sorted(set(tags)))))
