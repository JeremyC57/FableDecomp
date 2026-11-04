"""Match TLC vtables to Anniversary RTTI vtables by shared string literals of their own slot functions.
usage: vt_match.py <work_dir>
reads tlc_vts.tsv(+.fstr), tlc_rtti_vt.tsv, tlc_norrti_vt.tsv, ann_vts.tsv(+.fstr), ann_rtti_vt.tsv
writes vt_match_validation.tsv, vt_match_norrti.tsv
"""
import sys, os, collections
W = sys.argv[1]
USE_STORE = len(sys.argv) > 2 and sys.argv[2] == "store"
SUF = "_store" if USE_STORE else ""
def load(prefix, rtti_file):
    vts = {}
    for l in open(os.path.join(W, prefix + "_vts.tsv")):
        va, ns, slots = l.rstrip("\n").split("\t"); vts[va] = slots.split(",") if slots else []
    fstr = {}
    for l in open(os.path.join(W, prefix + "_vts.tsv.fstr"), encoding="latin-1"):
        a, s = l.rstrip("\n").split("\t", 1); fstr[a] = set(s.split("\x1f"))
    names = {}
    for l in open(os.path.join(W, rtti_file)):
        f = l.rstrip("\n").split("\t")
        names[f[0]] = (f[3], int(f[2]))
    # slot uniqueness
    cnt = collections.Counter(s for v in vts.values() for s in set(v))
    own = {}
    for va, slots in vts.items():
        st = set()
        for s in slots:
            if cnt[s] == 1: st |= fstr.get(s, set())
        own[va] = st
    if USE_STORE:
        for l in open(os.path.join(W, prefix + "_store.tsv"), encoding="latin-1"):
            va, n, ss = l.rstrip("\n").split("\t", 2)
            if ss and va in own: own[va] |= set(ss.split("\x1f"))
    return vts, own, names
tv, town, tnames = load("tlc", "tlc_rtti_vt.tsv")
av, aown, anames = load("ann", "ann_rtti_vt.tsv")
# string -> anniversary vtables
inv = collections.defaultdict(set)
for va, st in aown.items():
    for s in st: inv[s].add(va)
tdf = collections.Counter(s for st in town.values() for s in st)
def best(va):
    st = {s for s in town[va] if len(inv.get(s, ())) <= 3 and tdf[s] <= 3}
    if not st: return None
    score = collections.Counter()
    for s in st:
        for a in inv.get(s, ()): score[a] += 1
    if not score: return None
    (a1, c1), = score.most_common(1)
    c2 = score.most_common(2)[1][1] if len(score) > 1 else 0
    jac = c1 / len(st | aown[a1]) if aown[a1] else 0
    return a1, c1, c2, len(st), jac
ok = bad = none = 0
with open(os.path.join(W, "vt_match_validation%s.tsv" % SUF), "w") as f:
    for va, (tn, off) in sorted(tnames.items()):
        r = best(va)
        if not r: none += 1; continue
        a1, c1, c2, n, jac = r
        an = anames[a1][0]
        conf = c1 >= 2 and c1 > c2
        if not conf: continue
        good = an == tn
        ok += good; bad += (not good)
        f.write("%s\t%s\t%s\t%s\t%d\t%d\t%d\t%.2f\t%s\n" % (va, tn, a1, an, c1, c2, n, jac, "OK" if good else "DIFF"))
print("validation on TLC RTTI vtables: confident", ok + bad, "correct", ok, "wrong", bad, "no-evidence", none, "of", len(tnames))
norr = [l.split("\t")[0] for l in open(os.path.join(W, "tlc_norrti_vt.tsv"))]
hit = 0
with open(os.path.join(W, "vt_match_norrti%s.tsv" % SUF), "w") as f:
    for va in norr:
        r = best(va)
        if not r: continue
        a1, c1, c2, n, jac = r
        if c1 >= 2 and c1 > c2:
            hit += 1
            f.write("%s\t%d\t%s\t%s\t%d\t%d\t%d\t%.2f\n" % (va, len(tv[va]), a1, anames[a1][0], c1, c2, n, jac))
print("TLC RTTI-less vtables", len(norr), "matched confidently", hit)
