"""Vtable matcher v2: strings + mapped callees. Callee map seeded from RTTI vtables common to both binaries
(same mangled class + COL offset + slot count), slot i <-> slot i, then one level of positional call alignment.
Validation is leave-one-out: a callee mapping produced only by the vtable under test is ignored.
usage: vt_match2.py <work_dir>
"""
import sys, os, collections
W = sys.argv[1]
def rd(p, enc="latin-1"):
    return [l.rstrip("\n").split("\t") for l in open(os.path.join(W, p), encoding=enc)]
def load(prefix, rtti):
    vts = {f[0]: (f[2].split(",") if f[2] else []) for f in rd(prefix + "_vts.tsv")}
    fstr = {f[0]: set(f[1].split("\x1f")) for f in rd(prefix + "_vts.tsv.fstr")}
    fcall = {f[0]: f[1].split(",") for f in rd(prefix + "_vtc.tsv.fcall")}
    names = {f[0]: (f[3], int(f[2]), int(f[1])) for f in rd(rtti)}
    cnt = collections.Counter(s for v in vts.values() for s in set(v))
    own = {va: [s for s in sl if cnt[s] == 1] for va, sl in vts.items()}
    return vts, fstr, fcall, names, own
tv, tfs, tfc, tn, town = load("tlc", "tlc_rtti_vt.tsv")
av, afs, afc, an, aown = load("ann", "ann_rtti_vt.tsv")
# seeds
ta = collections.defaultdict(list); aa = collections.defaultdict(list)
for k, (n, o, s) in tn.items(): ta[(n, o)].append(k)
for k, (n, o, s) in an.items(): aa[(n, o)].append(k)
pairs = []  # (tvt, avt)
for key, tl in ta.items():
    al = aa.get(key, [])
    if len(tl) == 1 and len(al) == 1 and len(tv[tl[0]]) == len(av[al[0]]): pairs.append((tl[0], al[0]))
# function map with provenance
fmap = collections.defaultdict(lambda: collections.defaultdict(set))  # tf -> af -> {seed vt}
for t, a in pairs:
    for x, y in zip(tv[t], av[a]):
        fmap[x][y].add(t)
        cx, cy = tfc.get(x), afc.get(y)
        if cx and cy and len(cx) == len(cy):
            for p, q in zip(cx, cy): fmap[p][q].add(t)
def mapped(f, exclude):
    m = fmap.get(f)
    if not m: return None
    cands = [(y, s - {exclude}) for y, s in m.items()]
    cands = [(y, s) for y, s in cands if s]
    if len(cands) != 1: return None
    return cands[0][0]
def feats_t(va, exclude):
    out = set()
    for f in town[va]:
        out |= {"S:" + s for s in tfs.get(f, ())}
        for c in tfc.get(f, ()):
            y = mapped(c, exclude)
            if y: out.add("F:" + y)
    return out
afeat = {}
for va in av:
    st = set()
    for f in aown[va]:
        st |= {"S:" + s for s in afs.get(f, ())}
        st |= {"F:" + c for c in afc.get(f, ())}
    afeat[va] = st
inv = collections.defaultdict(set)
for va, st in afeat.items():
    for s in st: inv[s].add(va)
def best(va, exclude=None):
    st = {s for s in feats_t(va, exclude) if 0 < len(inv.get(s, ())) <= 3}
    if not st: return None
    score = collections.Counter()
    for s in st:
        for a in inv[s]: score[a] += 1
    top = score.most_common(2)
    c1 = top[0][1]; c2 = top[1][1] if len(top) > 1 else 0
    return top[0][0], c1, c2, len(st)
ok = bad = 0
for va, (n, o, s) in tn.items():
    r = best(va, exclude=va)
    if not r: continue
    a1, c1, c2, k = r
    if c1 >= 2 and c1 > c2:
        if an[a1][0] == n: ok += 1
        else: bad += 1
print("seed vtable pairs", len(pairs), "mapped functions", len(fmap))
print("leave-one-out validation on TLC RTTI vtables: correct", ok, "wrong", bad)
norr = [f[0] for f in rd("tlc_norrti_vt.tsv")]
hit = 0
with open(os.path.join(W, "vt_match2_norrti.tsv"), "w") as fo:
    for va in norr:
        r = best(va)
        if not r: continue
        a1, c1, c2, k = r
        if c1 >= 2 and c1 > c2:
            hit += 1
            fo.write("%s\t%d\t%s\t%s\t%d\t%d\t%d\n" % (va, len(tv[va]), a1, an[a1][0], c1, c2, k))
print("TLC RTTI-less vtables", len(norr), "matched", hit)
