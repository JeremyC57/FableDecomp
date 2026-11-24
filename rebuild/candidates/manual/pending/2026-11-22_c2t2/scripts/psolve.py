#!/usr/bin/env python3
"""psolve.py -- per-site uniform-shift solution sets on one lk log (fsolve's key model, one site at a time), then their
intersection and, if it is empty, the pairwise conflicts.  Unlike fsolve.py <first> <last>, sites NOT listed are left free
(fsolve requires every unlisted sort in the range to keep its permutation, which over-constrains code-neutral re-sorts).

  psolve.py <log.json> --func SUBSTR --spec SPECFILE [--min N] [--F LO HI] [--T LO HI --late-after SITE | --late-min N]

SPECFILE lines: `<site#> flip|keep|=<perm> <label>`  (perm = indices into the incoming operand list, as fsolve).
Prints per site the F runs (T = 0) or the (F, T) count, the intersection, and a conflict matrix when it is empty.
"""
import argparse, os, sys
import numpy as np
sys.dont_write_bytecode = True
LK = os.environ.get("LKDIR", "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe/lk")
sys.path.insert(0, LK)
import lk, fsolve  # noqa: E402

ap = argparse.ArgumentParser()
ap.add_argument("log"); ap.add_argument("--func"); ap.add_argument("--spec", required=True)
ap.add_argument("--min", type=int, default=0)
ap.add_argument("--F", type=int, nargs=2, default=[-1024, 1023])
ap.add_argument("--T", type=int, nargs=2, default=[0, 0])
ap.add_argument("--late-after", type=int); ap.add_argument("--late-min", type=int)
a = ap.parse_args()
name, ev = lk.function_events(lk.load(a.log), a.func)
ss = {s["n"]: s for s in lk.sites(ev)}
Fv = np.arange(a.F[0], a.F[1] + 1); Tv = np.arange(a.T[0], a.T[1] + 1)
F, T = np.meshgrid(Fv, Tv, indexing="ij")
F = np.vstack([F, np.zeros((1, F.shape[1]), dtype=F.dtype)]); T = np.vstack([T, np.zeros((1, T.shape[1]), dtype=T.dtype)])
late_set = None
if a.late_after is not None:
    late_set, k = set(), -1
    for e in ev:
        if e[0] == "S":
            k += 1
        elif e[0] == "Y" and k >= a.late_after and e[3] is not None:
            late_set.add(e[3])
if a.T != [0, 0] and a.late_min is None and late_set is None:
    sys.exit("psolve: a T range needs --late-after or --late-min")
specs = []
for l in open(a.spec):
    l = l.split("#")[0].strip()
    if not l:
        continue
    p = l.split(None, 2)
    n, w = int(p[0]), p[1]
    tg = (True, None) if w == "flip" else (False, None) if w == "keep" else (False, [int(c) for c in w[1:]])
    specs.append((n, w, p[2] if len(p) > 2 else "", tg))
grids, approx = {}, []
for n, w, lab, tg in specs:
    ctx = fsolve.Ctx(F.astype(np.int64), T.astype(np.int64), a.min, a.late_min, late_set, False)
    ok = fsolve.site_perm_ok(ss[n], tg, ctx)
    z = bool(ok[-1].all()) if w == "keep" else not bool(ok[-1].any())   # zero-shift sanity: keep holds, flip / =perm fail
    grids[n] = ok[:-1]
    if ctx.approx:
        approx.append(n)
    f0 = [int(f) for f, r in zip(Fv, grids[n]) if r[list(Tv).index(0)]] if 0 in Tv else []
    cnt = int(grids[n].sum())
    print(f"{n:6d} {w:6s} {lab:28s} zero-shift-consistent={'yes' if z else 'NO'}  (F,T) sols {cnt:6d}  T=0 F: {fsolve.runs(f0) or '-'}")
print(f"# {name}: min {a.min}, F {a.F[0]}..{a.F[1]}, T {a.T[0]}..{a.T[1]}, late "
      f"{'after site %d (%d numbers)' % (a.late_after, len(late_set)) if late_set is not None else ('n >= %d' % a.late_min if a.late_min is not None else 'none')}"
      f"{'  APPROX at %s' % approx if approx else ''}")
inter = np.ones(grids[specs[0][0]].shape, dtype=bool)
for n in grids:
    inter &= grids[n]
tot = int(inter.sum())
print(f"# intersection: {tot} (F, T) points")
for fi, f in enumerate(Fv):
    ts = Tv[inter[fi]]
    if len(ts):
        print(f"F={f:5d}  T={fsolve.runs(ts.tolist())}")
if tot == 0:
    # collapse sites by label group (text before the first '/'), e.g. the AND and OR trees of one bookmark site
    grp = {}
    for n, w, lab, tg in specs:
        g = lab.split("/")[0] or str(n)
        grp.setdefault(g, [np.ones(inter.shape, dtype=bool), w])
        grp[g][0] &= grids[n]
    gs = list(grp)
    print("# pairwise over groups: '.' = the two groups' sets intersect, 'x' = disjoint")
    print("      " + " ".join(f"{g[-4:]:>4s}" for g in gs))
    for i in gs:
        print(f"{i[-5:]:>5s} " + " ".join("   ." if (grp[i][0] & grp[j][0]).any() else "   x" for j in gs) + f"  {grp[i][1]}")
    cur = np.ones(inter.shape, dtype=bool); kept, dropped = [], []
    for g in gs:
        if (cur & grp[g][0]).any():
            cur &= grp[g][0]; kept.append(g)
        else:
            dropped.append(g)
    print(f"# greedy (spec order): kept {kept}; conflicting with them: {dropped}")
