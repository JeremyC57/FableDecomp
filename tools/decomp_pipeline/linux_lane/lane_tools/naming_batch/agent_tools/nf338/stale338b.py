# stale338b.py (NF338): like stale338.py but on DEMANGLED text: the identifiers a wrong spelling has that the PDB spelling lacks
# (stand-in classes / scopes / types / method names). STANDIN_LEFT = one of them is still in the witness's landed source (code, not comments);
# NONE_LEFT = none is (the binder may have been respelled since: compile it to prove); SIG_ONLY = no extra identifier (access / const / cv /
# pointer-v-reference / basic-type difference: needs the object).
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
sys.path.insert(0, R + '/tools/decomp_pipeline/naming'); import msvc_demangle as M
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2].lower().replace('0x', '')))
KW = set('void char signed unsigned short int long float double bool const volatile class struct union enum operator std __int64 __cdecl __stdcall __fastcall __thiscall public private protected virtual static'.split())
def ids(s):
    try: p = M.pretty(s)
    except Exception: p = s
    return set(x for x in re.findall(r'[A-Za-z_][A-Za-z0-9_]*', p) if x not in KW), p
E = nf.entries()
out = open(A + '/work/stale2.tsv', 'w')
for i, e in enumerate(E, 1):
    t, pdb = e[0], e[6]; ip, pp = ids(pdb)
    for s, w in T.get(t, []):
        if s == pdb: continue
        iw, pw = ids(s); extra = sorted(iw - ip)
        src = nf.src_of(w)
        if not src or not src.exists(): st, left = 'NOSRC', extra
        else:
            txt = re.sub(r'//[^\n]*', '', src.read_text(encoding='utf-8', errors='replace'))
            left = [x for x in extra if re.search(r'\b%s\b' % re.escape(x), txt)]
            st = 'SIG_ONLY' if not extra else ('STANDIN_LEFT' if left else 'NONE_LEFT')
        out.write('%d\t%s\t%s\t%s\t%s\t%s\n' % (i, t, w, st, ','.join(left if st == 'STANDIN_LEFT' else extra), pw[:160]))
