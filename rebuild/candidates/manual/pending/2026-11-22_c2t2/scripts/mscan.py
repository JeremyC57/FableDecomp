#!/usr/bin/env python3
"""mscan.py -- insertion-point scan with fsolve's key model.  Records inserted where number N1 is allocated shift every
n >= N1 by F; a second insertion at N2 > N1 adds T for n >= N2 (fsolve --min N1 --late-min N2).  For each candidate
(N1[, N2]) the per-site sets (spec file as psolve.py) are intersected over the (F, T) grid.

  mscan.py <log.json> --spec SPEC --mins N,N,... [--two] [--F LO HI] [--T LO HI]
"""
import argparse, os, sys
import numpy as np
sys.dont_write_bytecode = True
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe/lk")
import lk, fsolve  # noqa: E402

ap = argparse.ArgumentParser()
ap.add_argument("log"); ap.add_argument("--spec", required=True); ap.add_argument("--mins", required=True)
ap.add_argument("--two", action="store_true")
ap.add_argument("--F", type=int, nargs=2, default=[-2048, 2047]); ap.add_argument("--T", type=int, nargs=2, default=[0, 0])
a = ap.parse_args()
name, ev = lk.function_events(lk.load(a.log), None)
ss = {s["n"]: s for s in lk.sites(ev)}
specs = []
for l in open(a.spec):
    l = l.split("#")[0].strip()
    if l:
        p = l.split(None, 2)
        w = p[1]
        specs.append((int(p[0]), (True, None) if w == "flip" else (False, None) if w == "keep" else (False, [int(c) for c in w[1:]]), p[2] if len(p) > 2 else ""))
mins = [int(x) for x in a.mins.split(",")]
Fv = np.arange(a.F[0], a.F[1] + 1); Tv = np.arange(a.T[0], a.T[1] + 1)
F, T = np.meshgrid(Fv, Tv, indexing="ij")
F = np.vstack([F, np.zeros((1, F.shape[1]), dtype=F.dtype)]).astype(np.int64)
T = np.vstack([T, np.zeros((1, T.shape[1]), dtype=T.dtype)]).astype(np.int64)
pairs = [(m, None) for m in mins] if not a.two else [(m1, m2) for i, m1 in enumerate(mins) for m2 in mins[i + 1:]]
best = (0, None)
for m1, m2 in pairs:
    ctx = fsolve.Ctx(F, T if m2 is not None else T * 0, m1, m2)
    cnt = np.zeros((F.shape[0] - 1, F.shape[1]), dtype=np.int32)
    for n, tg, lab in specs:
        cnt += fsolve.site_perm_ok(ss[n], tg, ctx)[:-1]
    ok = cnt == len(specs)
    tot = int(ok.sum())
    mx = int(cnt.max())
    i = np.unravel_index(int(cnt.argmax()), cnt.shape)
    if mx > best[0]:
        best = (mx, (m1, m2, int(Fv[i[0]]), int(Tv[i[1]])))
    sol = ""
    if tot:
        pts = np.argwhere(ok)[:6]
        sol = " e.g. " + " ".join(f"(F={Fv[p[0]]},T={Tv[p[1]]})" for p in pts)
    print(f"N1={m1} N2={m2}: solutions {tot}; best point satisfies {mx}/{len(specs)} at F={Fv[i[0]]} T={Tv[i[1]]}{sol}", flush=True)
print(f"# best overall: {best[0]}/{len(specs)} at N1,N2,F,T = {best[1]}")
