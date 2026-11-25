#!/usr/bin/env python3
"""gen423.py <out_prefix>  (rows listed in work/skip_rows.txt are skipped; set SKIP=none to ignore that file): NF423 T1 caller-respell generator over EVERY T1 target of the tracker (tracker/OPEN.tsv).

For each T1 target t with PDB spelling P, every reloc_bindings line (s, t, w) with s != P is a wrong spelling. Shapes the generator
writes (everything else goes to RESIDUE.tsv with a reason code):
  CPFREE  s = ?NAME@@YIXPAX@Z (free __fastcall void(void*) stand-in), P = ??1 / ?ReleaseData / ?DeleteData of CCountedPointer<T>,
          T a plain class (optionally in N-prefixed namespaces). Calls `NAME(e);` -> `reinterpret_cast<CCountedPointer<T>*>(e)->
          CCountedPointer<T>::~CCountedPointer();` (DTOR_DECL_ONLY) / `...->ReleaseData();` (RELEASEDATA_DECL_ONLY); a deleter
          address `NAME` used as a value -> `&CCountedPointer<T>::DeleteData` (DELETEDATA_DECL_ONLY, the NF284 hybrid).
  DTFREE  s = ?NAME@@YIXPAX@Z, P = ??1C@(ns@)*@[QU]AE@XZ (public dtor of a plain class, namespaces only): calls `NAME(e);` ->
          `reinterpret_cast<ns::C*>(e)->ns::C::~C();` with a bodiless declaration `class C { public: [virtual] ~C(); };`.
Caller rows: every landed source (rebuild/src/compiled) that declares NAME and calls it, not only the table witness; each row's retail
call targets (E8 scan of the catalog bytes) must contain t, else residue. Row screens: lock free or NF423 (gen423 does NOT take locks: mkbatch423.py does), T3_PORT_QUEUE col 2/3,
reserved list (via reserved_check later), rows another shape already claimed. Writes <out_prefix>_rows/<a>.cpp, <out_prefix>_PLAN.tsv
(address, shape, old, new, target), RESIDUE.tsv (target, wrong, witness_or_row, reason)."""
import sys, os, re, csv, collections, struct, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'; A = str(nf.A); S = str(nf.S)
csv.field_size_limit(10**9)
outp = sys.argv[1]
SKIP = set(open(A + '/work/skip_rows.txt').read().split()) if os.path.exists(A + '/work/skip_rows.txt') and os.environ.get('SKIP') != 'none' else set()
op = [r for r in csv.DictReader(open(A + '/tracker/OPEN.tsv'), delimiter='\t') if r['tier'] == 'T1']
tab = collections.defaultdict(list)
for r in csv.DictReader(open(R + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'): tab[r['target']].append((r['symbol'], r['address']))
t3 = collections.defaultdict(list)
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t')
    for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3])): t3[x.lower()].append(q[0])
# landed sources by address (file name suffix) and identifier index
files = {}
for f in glob.glob(R + '/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files.setdefault(m.group(1), f)
text = {a: open(f, encoding='utf-8', errors='replace').read() for a, f in files.items()}
cat = nf.catalog()
tok = collections.defaultdict(set)
for a, x in text.items():
    for w_ in set(re.findall(r'\w+', x)): tok[w_].add(a)
def calls(a):
    if a not in cat: return set()
    b = bytes.fromhex(cat[a]['bytes']); va = int(a, 16); out = set()
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9): out.add('%08x' % ((va + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff))
    for i in range(len(b) - 3): out.add('%08x' % struct.unpack_from('<I', b, i)[0])   # address-of (push imm / mov imm)
    return out
def lockfree(a):
    p = S + '/w22/nflocks/%s/owner' % a
    return (not os.path.exists(p)) or open(p).read().strip() in ('released', 'NF439')
def tname(m):   # mangled V<id>@<ns>@...@ (already split) -> C++ spelling + forward declaration, or None
    parts = m.split('@')
    if not parts or not re.match(r'^[A-Za-z_]\w*$', parts[0]): return None
    ns = parts[1:]
    if any(not re.match(r'^N[A-Z]\w*$', n) for n in ns): return None   # an outer that is not an N-namespace may be a class (nested): no forward decl
    full = '::'.join(list(reversed(ns)) + [parts[0]])
    fwd = 'class %s;' % parts[0]
    for n in ns: fwd = 'namespace %s { %s }' % (n, fwd)
    return full, fwd, parts[0], list(reversed(ns))
plan = []; res = []; claimed = {}
def residue(t, s, w, why): res.append((t, s, w, why))
for r in op:
    t = r['target']; P = r['pdb_spelling']
    for s, w in tab[t]:
        if s == P: continue
        m = re.match(r'^\?(\w+)@@YIXPAX@Z$', s)
        if not m: residue(t, s, w, 'shape: wrong spelling is not a free __fastcall void(void*) stand-in (method / typed / other: needs a per-row model edit)'); continue
        NAME = m.group(1)
        mc = re.match(r'^(\?\?1|\?ReleaseData@|\?DeleteData@)\?\$CCountedPointer@V(.+?)@@@@(QAE@XZ|QAEXXZ|SIXPAX@Z)$', P)
        md = re.match(r'^\?\?1(\w+)@((?:\w+@)*)@([QU])AE@XZ$', P)
        if mc:
            tn = tname(mc.group(2)); kind = {'??1': 'dtor', '?ReleaseData@': 'release', '?DeleteData@': 'delete'}[mc.group(1)]
            if not tn: residue(t, s, w, 'CPFREE: T is nested in a class or a template (no forward declaration without the outer class model)'); continue
        elif md:
            tn = tname(md.group(1) + '@' + md.group(2).rstrip('@')) if md.group(2) else tname(md.group(1)); kind = 'cdtor'
            if not tn: residue(t, s, w, 'DTFREE: class nested in a class (outer is not an N-namespace)'); continue
        else:
            residue(t, s, w, 'shape: free stand-in, PDB target is not a CCountedPointer release/deleter or a public plain destructor (%s)' % P[:60]); continue
        users = sorted(tok.get(NAME, ()))
        if not users: residue(t, s, w, 'no landed source names %s (stale table line)' % NAME); continue
        for a in users:
            x = text[a]
            why = None
            if a in SKIP: why = 'row in an earlier NF423 batch / held'
            elif a in claimed and claimed[a] != (t, s): why = 'row carries a second respell (one generated edit per row)'
            elif t not in calls(a): why = 'retail bytes of this row do not reference the target (the identifier binds another address here)'
            elif not lockfree(a): why = 'locked by another agent'
            elif a in t3: why = 'named in T3_PORT_QUEUE col 2/3 (%s)' % ','.join(t3[a][:3])
            elif kind != 'cdtor' and re.search(r'(template\s*<[^>]*>\s*(class|struct)\s+CCountedPointer\b|(class|struct)\s+CCountedPointer\b\s*[{:])', x): why = 'row has its own CCountedPointer model (port)'
            elif kind != 'cdtor' and 'FABLE_RETAIL_CCOUNTEDPOINTER_' in x: why = 'row already sets a CCountedPointer opt-in (one header configuration per TU)'
            elif kind == 'cdtor' and re.search(r'\b%s\b' % re.escape(tn[2]), x): why = 'row already models / names class %s' % tn[2]
            if why: residue(t, s, a, why); continue
            decl = re.search(r'^[ \t]*(extern[ \t]+)?void[ \t]+(__fastcall|FABLE_FASTCALL)[ \t]+%s[ \t]*\([^)]*\)[ \t]*;[^\n]*\n' % re.escape(NAME), x, re.M)
            if not decl: residue(t, s, a, 'no single-line `void __fastcall NAME(void*);` declaration'); continue
            body = x[:decl.start()] + x[decl.end():]
            n_calls = len(re.findall(r'\b%s\s*\(' % re.escape(NAME), body)); n_any = len(re.findall(r'\b%s\b' % re.escape(NAME), body))
            if kind == 'delete':
                if n_calls or n_any < 1: residue(t, s, a, 'deleter stand-in is called, not only stored'); continue
                body2 = re.sub(r'(?<![\w&])&?%s\b' % re.escape(NAME), '&CCountedPointer<%s>::DeleteData' % tn[0], body)
                macro = 'FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY'
            else:
                if n_calls != n_any or n_calls == 0: residue(t, s, a, 'stand-in used other than as a direct call'); continue
                def rep(mm):
                    e = mm.group(1)
                    if kind == 'dtor': return 'reinterpret_cast<CCountedPointer<%s>*>(%s)->CCountedPointer<%s>::~CCountedPointer();' % (tn[0], e, tn[0])
                    if kind == 'release': return 'reinterpret_cast<CCountedPointer<%s>*>(%s)->ReleaseData();' % (tn[0], e)
                    return 'reinterpret_cast<%s*>(%s)->%s::~%s();' % (tn[0], e, tn[0], tn[2])
                body2, k = re.subn(r'\b%s\s*\(([^;]*)\)\s*;' % re.escape(NAME), rep, body)
                if k != n_calls: residue(t, s, a, 'call statement not of the form NAME(expr);'); continue
                macro = {'dtor': 'FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY', 'release': 'FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY'}.get(kind)
            tgt = t.upper()
            if kind == 'cdtor':
                virt = 'virtual ' if md.group(3) == 'U' else ''
                d = 'class %s { public: %s~%s(); };' % (tn[2], virt, tn[2])
                for n in reversed(tn[3]): d = 'namespace %s { %s }' % (n, d)
                ins = ('// Naming (NF423 gen, 2026-11-25): the callee 0x%s now binds its PDB spelling %s (the T1 tracker target; the self row\'s\n'
                       '//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in %s.\n%s\n') % (tgt, P, s, d)
                new = x[:decl.start()] + ins + body2[decl.start():]
            else:
                ins = ('// Naming (NF423 gen, 2026-11-25): the callee 0x%s now binds its PDB spelling %s (the T1 tracker target; the self row\'s\n'
                       '//   Leaf / PDB name sets), through the shared engine/CCountedPointer.h under %s (declaration only); was the free stand-in %s.\n'
                       '#include "engine/CCountedPointer.h"\n%s\n') % (tgt, P, macro, s, tn[1])
                new = x[:decl.start()] + ins + body2[decl.start():]
                # the opt-in goes first in the TU, ahead of any header that may include CCountedPointer.h
                new = '#define %s\n' % macro + new if not new.startswith('#pragma') else re.sub(r'^((?:#pragma[^\n]*\n)+)', lambda mm: mm.group(1) + '#define %s\n' % macro, new, 1)
            claimed[a] = (t, s); plan.append((a, kind, s, P, t, new))
os.makedirs(outp + '_rows', exist_ok=True)
with open(outp + '_PLAN.tsv', 'w') as f:
    for a, k, s, P, t, new in plan:
        f.write('\t'.join([a, k, s, P, t]) + '\n'); open(outp + '_rows/%s.cpp' % a, 'w', encoding='utf-8').write(new)
with open(outp + '_RESIDUE.tsv', 'w') as f:
    f.write('target\twrong_symbol\twitness_or_row\treason\n')
    for x in res: f.write('\t'.join(x) + '\n')
print('targets', len(op), 'planned rows', len(plan), 'residue lines', len(res))
print(collections.Counter(k for _, k, *_ in plan))
print(collections.Counter(x[3].split(':')[0][:70] for x in res).most_common(25))
