#!/usr/bin/env python3
"""assign440.py: one global name assignment for the open D53 / D44 / SH67 CCountedPointer<T> copies (ruling R-NF428-1 relaxed by the
owner best-guess rule, 2026-11-25). Every name is a FableWin.pdb CCountedPointer<T> member name that no retail address binds or defines.

Tiers (best first):
  T1 BOUND   the binding table already binds a PDB D / RD / SH name at this address (a landed caller): take it (not a guess).
  T2 TU      the copy sits in a retail TU segment whose FableWin module instantiates the family (gate.json 'same' segments); the
             module's pointee types in FableWin order are aligned monotonically with the segment's copies in address order, with the
             already-named copies as anchors; T2 only where every alignment consistent with the anchors gives this copy one T.
             (Calibration: tc_village's 7 named copies follow FableWin's T order exactly.)
  T3 NEIGH   a unique T from the functions around the run (pred.json: PREV/NEXT names / layouts, adjacent named member), inside the
             module's set when the segment has one.
Member (~ vs ReleaseData) for D copies: forced when the other is taken; a T with two copies gets ReleaseData on the earlier and
~ on the later (named pairs: RD first 5 of 6); otherwise the shape's base rate (named copies: D53 ~ 23 / RD 5; D44 RD 29 / ~ 7). Each symbol is used once: higher tier wins; a loser takes the other member if free, else HOLD.
-> evidence/assign.tsv: address shape status tier T_mangled member symbol basis note"""
import sys, json, re, collections, itertools
sys.path.insert(0, __import__('os').path.dirname(__file__))
from lib440 import *

cat, keys = catalog(); r = rec(); pl = pool(); s2a, a2s, selfs = bindings()
occ = json.load(open(W + '/evidence/occ428.json')); gate = json.load(open(W + '/evidence/gate.json'))
pred = json.load(open(W + '/evidence/pred.json'))
w_pdb, e_pdb = pdbsets(); allpdb = w_pdb | e_pdb
extra_taken = set(l.strip() for l in open(W + '/evidence/extra_taken.txt')) if __import__('os').path.exists(W + '/evidence/extra_taken.txt') else set()

fam = {}
for f in tsv(P('rebuild/candidates/manual/pending/2026-11-25_nf428_handback/FAMILY_TABLE.tsv')):
    fam[f[0]] = f
blocked = {f[0]: f[1] for f in tsv(W + '/evidence/blocked.tsv', 0)}

def kind_of(a): return 'SH' if occ[a][0] == 'SH67' else 'D'
def is_open(a):
    sl = r.get(a, {}).get('self_leaf') or ''
    return 'CUnprovenPointee_' in sl and a in fam and fam[a][3] == 'HOLD'

def known(a):
    """(kind, M) of a copy whose name is a PDB CCountedPointer member (bound / self / pdb_spelling)"""
    for n in sorted(a2s.get(a, ())) + [r.get(a, {}).get('pdb_spelling') or '']:
        if n in allpdb:
            x = member_T(n)
            if x: return x
    return None

# names taken anywhere (bound or defined at some retail address) -- by address
taken = {}
for n, addrs in s2a.items():
    if n in allpdb and member_T(n): taken[n] = set(addrs)
for n in extra_taken: taken.setdefault(n, set()).add('pending')

# ---------- T2: TU alignment ----------
fw_order = collections.defaultdict(dict)  # module -> M -> first index
for f in tsv(P('ghidra_out/fablewin_obj_functions.tsv'), 0):
    if len(f) < 7: continue
    x = member_T(f[6])
    if x and x[1] in pl:
        k = 'SH' if x[0] == 'SH' else 'D'
        d = fw_order[(f[0], k)]
        i = int(f[1]) if f[1].isdigit() else 10 ** 9
        d[x[1]] = min(d.get(x[1], i), i)

t2 = {}; t2note = {}; segcand = {}
for key, g in gate.items():
    mod, k = g['obj'], g['kind']
    order = sorted(fw_order.get((mod, k), {}).items(), key=lambda kv: kv[1])
    Ts = [m for m, i in order]
    addrs = sorted(g['addrs'])
    for a in addrs: segcand[a] = set(Ts)
    if not Ts: continue
    n, m = len(addrs), len(Ts)
    anchors = {}
    for i, a in enumerate(addrs):
        kn = known(a)
        if kn: anchors[i] = kn[1]
    rep = 2 if (n > m and k == 'D') else 1
    ok = lambda i, j: (i not in anchors) or Ts[j] == anchors[i]
    f = [collections.defaultdict(int) for _ in range(n)]; b = [collections.defaultdict(int) for _ in range(n)]
    for j in range(m):
        if ok(0, j): f[0][(j, 1)] = 1
    for i in range(n - 1):
        for (j, c), v in f[i].items():
            for jj in range(j + 1, m):
                if ok(i + 1, jj): f[i + 1][(jj, 1)] += v
            if c + 1 <= rep and ok(i + 1, j): f[i + 1][(j, c + 1)] += v
    for j in range(m):
        for c in range(1, rep + 1):
            if ok(n - 1, j): b[n - 1][(j, c)] = 1
    for i in range(n - 2, -1, -1):
        for j in range(m):
            for c in range(1, rep + 1):
                if not ok(i, j): continue
                v = sum(b[i + 1].get((jj, 1), 0) for jj in range(j + 1, m))
                if c + 1 <= rep: v += b[i + 1].get((j, c + 1), 0)
                b[i][(j, c)] = v
    total = sum(v * b[n - 1].get(s, 0) for s, v in f[n - 1].items())
    if not total:
        for a in addrs: t2note[a] = 'TU %s: no alignment fits the anchors' % mod.split('\\')[-1]
        continue
    for i, a in enumerate(addrs):
        N = collections.defaultdict(int)
        for s, v in f[i].items(): N[s] += v * b[i].get(s, 0)
        opts = {Ts[j] for (j, c), v in N.items() if v}
        if len(opts) == 1:
            T = next(iter(opts)); j = Ts.index(T); pos = None
            if rep == 2:
                if N.get((j, 2), 0) == total: pos = 'D'
                elif i + 1 < n and f[i].get((j, 1), 0) * b[i + 1].get((j, 2), 0) == total: pos = 'RD'
            t2[a] = (T, pos)
            t2note[a] = 'TU %s (%s: %d copies v %d FableWin types, %d named anchors, %d alignment(s))' % (mod.split('\\')[-1], g['st'], n, m, len(anchors), total)
        else:
            t2note[a] = 'TU %s: %d possible types' % (mod.split('\\')[-1], len(opts))
            segcand[a] = opts

# ---------- T3: neighbours ----------
t3 = {}; t3note = {}
for a, v in pred.items():
    s = v['src']; Ts = set()
    for srcn in ('NB_MEMBER', 'NEXT_NAME', 'NEXT_LAYOUT', 'PREV_NAME', 'PREV_LAYOUT'):
        if srcn in s:
            c = set(s[srcn])
            if a in segcand and segcand[a]: c &= segcand[a]
            if len(c) == 1: Ts = c; t3note[a] = srcn + ' ' + (v['next_name'] if srcn.startswith('NEXT') else v['prev_name'] if srcn.startswith('PREV') else '')[:90]; break
    if Ts: t3[a] = (Ts.pop(), None)
    elif a in segcand and len(segcand[a]) == 1:
        t3[a] = (next(iter(segcand[a])), None); t3note[a] = 'TU module single type'

# ---------- assignment ----------
want = []
for a in sorted(occ):
    if not is_open(a) or blocked.get(a, 'free') != 'free': continue
    k = kind_of(a)
    for n in sorted(a2s.get(a, ())):
        if n in allpdb and member_T(n):
            want.append((1, a, member_T(n)[1], 'SH' if member_T(n)[0] == 'SH' else member_T(n)[0], 'bound by a landed caller: ' + n[:60])); break
    else:
        if a in t2: want.append((2, a, t2[a][0], t2[a][1], t2note[a]))
        elif a in t3: want.append((3, a, t3[a][0], t3[a][1], t3note[a]))
        else: want.append((9, a, None, None, t2note.get(a) or 'no candidate type (no TU module set, no named neighbour)'))

def sym(k, M, mem):
    d = pl.get(M, {})
    if k == 'SH': return d.get('SH')
    return d.get('D' if mem == 'D' else 'RD')

used = {}; out = {}
byT = collections.defaultdict(list)
for t, a, M, mem, note in want:
    if t == 1:   # bound by a landed caller: the name is the binding, nothing to choose
        s = next(n for n in sorted(a2s.get(a, ())) if n in allpdb and member_T(n))
        out[a] = (t, M, member_T(s)[0], s, note); used[s] = (a, t)
for t, a, M, mem, note in want:
    if M and t != 1: byT[(kind_of(a), M)].append((t, a, mem, note))
for (k, M), lst in byT.items():
    lst.sort(key=lambda x: (x[0], x[1]))
    if k == 'SH':
        for i, (t, a, mem, note) in enumerate(lst):
            s = sym('SH', M, None)
            if not s: out[a] = (t, M, None, None, 'HOLD: no PDB ShareData name for this type'); continue
            if s in taken and a not in taken[s]: out[a] = (t, M, 'SH', s, 'HOLD: %s already at %s' % ('name', ','.join(sorted(taken[s]))[:40])); continue
            if s in used and used[s][0] != a: out[a] = (t, M, 'SH', s, 'HOLD: collision with %s (tier %d)' % (used[s][0], used[s][1])); continue
            used[s] = (a, t); out[a] = (t, M, 'SH', s, note)
        continue
    # D: up to two names per T
    free = [m for m in ('RD', 'D') if sym('D', M, m) and not (sym('D', M, m) in taken) and sym('D', M, m) not in used]
    lst_best = lst[:]
    # forced / positional members first
    pick = {}
    # keep the best two by tier (then address); others HOLD
    keepers = lst_best[:len(free)]
    for t, a, mem, note in lst_best[len(free):]:
        out[a] = (t, M, None, None, 'HOLD: type %s has no free member name left (collision; kept %s)' % (demangle_T(M) or M, ','.join(x[1] for x in keepers)))
    if len(keepers) == 2:
        e, l = sorted(keepers, key=lambda x: x[1])
        pick[e[1]] = 'RD'; pick[l[1]] = 'D'
    elif len(keepers) == 1:
        t, a, mem, note = keepers[0]
        pref = mem or ('RD' if occ[a][0] == 'D44' else 'D')   # base rate of the named copies: D53 ~ 23 / RD 5, D44 RD 29 / ~ 7
        pick[a] = pref if pref in free else (free[0] if free else None)
    for t, a, mem, note in keepers:
        m = pick.get(a)
        if not m: out[a] = (t, M, None, None, 'HOLD: both member names of %s taken' % (demangle_T(M) or M)); continue
        s = sym('D', M, m)
        if len(keepers) == 2:
            o = [x[1] for x in keepers if x[1] != a][0]
            why = 'member: positional pair with 0x%s (ReleaseData on the earlier copy, ~ on the later)' % o
        elif mem and m == mem: why = 'member: positional in the TU alignment (two copies of T, ReleaseData first)'
        elif m == ('RD' if occ[a][0] == 'D44' else 'D'): why = 'member: base rate of the shape'
        else: why = 'member: forced (the other member name is taken)'
        used[s] = (a, t); out[a] = (t, M, m, s, note + '; ' + why)
for t, a, M, mem, note in want:
    if not M: out[a] = (t, None, None, None, 'HOLD: ' + note)

rows = []
C = collections.Counter()
for a in sorted(out):
    t, M, m, s, note = out[a]
    v = 'HOLD' if note.startswith('HOLD') or not s else 'NAME'
    C[(t, v)] += 1
    rows.append([a, occ[a][0], r.get(a, {}).get('status', ''), 'T%d' % t if t < 9 else '-', M or '', m or '', s or '', note])
open(W + '/evidence/assign.tsv', 'w').write('address\tshape\tstatus\ttier\tT\tmember\tsymbol\tnote\n' + ''.join('\t'.join(x) + '\n' for x in rows))
for k in sorted(C): print(k, C[k])
# sanity: one symbol one address
cnt = collections.Counter(x[6] for x in rows if x[6] and not x[7].startswith('HOLD'))
print('dup symbols', [k for k, v in cnt.items() if v > 1][:5])
