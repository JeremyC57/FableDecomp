"""lib440.py: shared loaders for NF440 (CCountedPointer<T> uncalled-copy naming)."""
import os, re, json, collections, bisect
R = '/home/user/FableDecomp'
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF440'
P = lambda *a: os.path.join(R, *a)

def norm(a): return a.strip().lower().replace('0x', '').zfill(8)

def tsv(p, skip=1):
    return [l.rstrip('\n').split('\t') for l in open(p, encoding='utf-8', errors='replace').read().splitlines()[skip:] if l.strip()]

_c = {}
def catalog():
    if 'cat' not in _c:
        d = {}
        for f in tsv(P('rebuild/oracles/auto-re-candidates.tsv')):
            if len(f) >= 3: d[norm(f[0])] = (f[1], int(f[2]) if f[2].isdigit() else 0)
        _c['cat'] = d; _c['keys'] = sorted(d)
    return _c['cat'], _c['keys']

def rec():
    if 'rec' not in _c: _c['rec'] = json.load(open(W + '/status/rec.json'))
    return _c['rec']

def pdbsets():
    if 'pdb' not in _c:
        w, e = set(), set()
        for f in tsv(P('ghidra_out/fablewin_pdb_names.tsv'), 0):
            if len(f) > 1: w.add(f[1].strip())
        for f in tsv(P('ghidra_out/egor_pdb_names.tsv'), 0):
            if len(f) > 1: e.add(f[1].strip())
        _c['pdb'] = (w, e)
    return _c['pdb']

def bindings():
    if 'bind' not in _c:
        s2a, a2s = collections.defaultdict(set), collections.defaultdict(set)
        for f in tsv(P('rebuild/oracles/reloc_bindings.tsv')):
            if len(f) > 1: s2a[f[0]].add(norm(f[1])); a2s[norm(f[1])].add(f[0])
        selfs = {}
        for f in tsv(P('rebuild/oracles/self_symbols.tsv')):
            if len(f) > 1 and f[1]: selfs[norm(f[0])] = f[1]; s2a[f[1]].add(norm(f[0])); a2s[norm(f[0])].add(f[1])
        _c['bind'] = (s2a, a2s, selfs)
    return _c['bind']

# ---- CCountedPointer member names ----
KIND_RX = {
    'D': re.compile(r'^\?\?1\?\$CCountedPointer@(.+)@@QAE@XZ$'),
    'RD': re.compile(r'^\?ReleaseData@\?\$CCountedPointer@(.+)@@QAEXXZ$'),
    'SH': re.compile(r'^\?ShareData@\?\$CCountedPointer@(.+)@@QAEXP.*PAVCCPPointerInfo@@@Z$'),
}

def member_T(n):
    """(kind, mangled T arg incl. trailing '@' of the template arg list) or None. T arg = text between '?$CCountedPointer@' and the
    class-name terminator '@@' of the instantiated class: for simple class args 'VCFoo@NBar@@' -> 'VCFoo@NBar@'."""
    for k, rx in KIND_RX.items():
        m = rx.match(n)
        if m: return k, m.group(1)
    return None

def demangle_T(m):
    """mangled template arg (e.g. 'VCDataBank@NGameText@' or '$$CBVCFontBank@') -> C++ spelling, or None for template args."""
    const = ''
    if m.startswith('$$CB'): const = 'const '; m = m[4:]
    if not m or m[0] not in 'VU' or '?$' in m or '$' in m: return None
    parts = [p for p in m[1:].split('@') if p]
    if not parts or any(not re.match(r'^[A-Za-z_]\w*$', p) for p in parts): return None
    return const + '::'.join(reversed(parts))

def name_of(kind, M):
    if kind == 'D': return '??1?$CCountedPointer@%s@@QAE@XZ' % M
    if kind == 'RD': return '?ReleaseData@?$CCountedPointer@%s@@QAEXXZ' % M
    raise ValueError(kind)

def sh_name(M):
    # ShareData(T* data, CCPPointerInfo* info): param T* spelled PAV<T-without-trailing-@>@ (back-ref to arg not used by MSVC for PAV)
    t = M
    cv = 'PA'
    if t.startswith('$$CB'): cv = 'PB'; t = t[4:]
    return '?ShareData@?$CCountedPointer@%s@@QAEX%s%s@PAVCCPPointerInfo@@@Z' % (M, cv, t)

def pool():
    """every PDB CCountedPointer D / RD / SH name, with mangled T -> kind -> name"""
    if 'pool' not in _c:
        w, e = pdbsets(); byT = collections.defaultdict(dict)
        for n in w | e:
            x = member_T(n)
            if x: byT[x[1]][x[0]] = n
        _c['pool'] = byT
    return _c['pool']

def layouts():
    if 'lay' not in _c:
        cls = collections.defaultdict(list)
        for f in tsv(P('ghidra_out/struct_layouts_egor.tsv'), 0):
            if f[0] == '@STRUCT' or len(f) < 5: continue
            cls[f[0]].append((int(f[2]) if f[2].isdigit() else -1, f[3], f[4]))
        _c['lay'] = cls
    return _c['lay']

CP_RX = re.compile(r'CCountedPointer<((?:const )?[A-Za-z_][\w:]*)>')

def scope_of(n):
    """class scope (C++ spelling) of a mangled member function / ctor / dtor name, or None."""
    if not n or not n.startswith('?'): return None
    m = re.match(r'^\?\?(?:[0-9]|_[GE]|_D)([^?$].*?)@@', n)
    if m: parts = m.group(1).split('@')
    else:
        m = re.match(r'^\?[A-Za-z_]\w*@([^?$].*?)@@', n)
        if not m: return None
        parts = m.group(1).split('@')
    if any(not re.match(r'^[A-Za-z_]\w*$', p) for p in parts): return None
    return '::'.join(reversed(parts))
