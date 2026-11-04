"""Name TLC RTTI-less vtables by order alignment against Anniversary RTTI vtables between anchors.
Anchors: TLC RTTI vtables with a unique same-name Anniversary RTTI vtable, plus string-matched RTTI-less ones.
Within each gap, align by LCS on slot count (TLC RTTI-less slot counts are clipped at the next vtable start).
Assign only when the gap is small (<= MAXGAP each side). Validation: hide the string anchors, re-run, score.
usage: vt_gapfill.py <work_dir> <match.tsv>
"""
import sys, os, collections
W, M = sys.argv[1], sys.argv[2]
MAXGAP = 12
STRICT = len(sys.argv) > 3 and sys.argv[3] == 'strict'
def rd(p): return [l.rstrip("\n").split("\t") for l in open(os.path.join(W, p), encoding="latin-1")]
trt = {int(f[0], 16): (f[3], int(f[2]), int(f[1])) for f in rd("tlc_rtti_vt.tsv")}
tnr = {int(f[0], 16): int(f[1]) for f in rd("tlc_norrti_vt.tsv")}
art = {int(f[0], 16): (f[3], int(f[2]), int(f[1])) for f in rd("ann_rtti_vt.tsv")}
starts = sorted(set(trt) | set(tnr))
import bisect
def clip(va, n):
    i = bisect.bisect_right(starts, va)
    if i < len(starts): n = min(n, (starts[i] - va) // 4)
    return n
tnr = {va: clip(va, n) for va, n in tnr.items()}
akey = collections.defaultdict(list)
for va, (n, o, s) in art.items(): akey[(n, o)].append(va)
rtti_anchor = {}
for va, (n, o, s) in trt.items():
    l = akey.get((n, o), [])
    if len(l) == 1: rtti_anchor[va] = l[0]
str_anchor = {int(f[0], 16): int(f[2], 16) for f in rd(M)}
aslots = {va: s for va, (n, o, s) in art.items()}
asorted = sorted(art)
def run(anchors):
    pts = sorted(anchors.items())
    chain = [(t, a) for t, a in pts]
    out = {}
    for (t1, a1), (t2, a2) in zip(chain, chain[1:]):
        if a2 <= a1: continue
        tg = [v for v in tnr if t1 < v < t2 and v not in anchors]
        ag = [v for v in asorted[bisect.bisect_right(asorted, a1): bisect.bisect_left(asorted, a2)] if v not in anchors.values()]
        tg.sort()
        if not tg or not ag or len(tg) > MAXGAP or len(ag) > MAXGAP: continue
        if STRICT and (len(tg) != len(ag) or any(tnr[x] != aslots[y] for x, y in zip(tg, ag))): continue
        # LCS on slot counts
        n, m = len(tg), len(ag)
        dp = [[0] * (m + 1) for _ in range(n + 1)]
        for i in range(n - 1, -1, -1):
            for j in range(m - 1, -1, -1):
                dp[i][j] = dp[i + 1][j + 1] + 1 if tnr[tg[i]] == aslots[ag[j]] else max(dp[i + 1][j], dp[i][j + 1])
        i = j = 0
        while i < n and j < m:
            if tnr[tg[i]] == aslots[ag[j]] and dp[i][j] == dp[i + 1][j + 1] + 1:
                out[tg[i]] = (ag[j], t1, t2); i += 1; j += 1
            elif dp[i + 1][j] >= dp[i][j + 1]: i += 1
            else: j += 1
    return out
# validation: 5-fold, hide 20% of string anchors at a time
keys = sorted(str_anchor); ok = bad = 0
for fold in range(5):
    hide = set(keys[fold::5])
    anc = dict(rtti_anchor); anc.update({t: a for t, a in str_anchor.items() if t not in hide})
    hid = run(anc)
    ok += sum(1 for t in hide if t in hid and hid[t][0] == str_anchor[t])
    bad += sum(1 for t in hide if t in hid and hid[t][0] != str_anchor[t])
print("5-fold validation on string anchors: recovered", ok + bad, "correct", ok, "wrong", bad, "of", len(str_anchor))
anchors = dict(rtti_anchor); anchors.update(str_anchor)
res = run(anchors)
with open(os.path.join(W, "vt_gapfill%s.tsv" % ("_strict" if STRICT else "")), "w") as fo:
    for t, (a, t1, t2) in sorted(res.items()):
        fo.write("%08x\t%d\t%08x\t%s\tgap %08x..%08x\n" % (t, tnr[t], a, art[a][0], t1, t2))
print("new gap-fill assignments", len(res))
