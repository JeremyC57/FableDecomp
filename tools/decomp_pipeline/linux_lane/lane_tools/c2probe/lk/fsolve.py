#!/usr/bin/env python3
"""fsolve.py -- the uniform-shift solver on one lk log.

  fsolve.py <log.json> <first> <last> <flips> [--func SUBSTR] [--min N] [--F LO HI] [--pass a|b]
            [--T LO HI (--late-min N | --late-after SITE | --late-temps)]

Model (wfE6-4/-5): every c2 symbol number n >= --min (default 0) moves by F (records added before every site
in [first, last]), and every LATE number moves by F + T (T = records added after the last site, which only the
late, whole-function-pass numbers see).  Which numbers are late must be said explicitly whenever T is not 0:
  --late-min N      numbers >= N;
  --late-after SITE numbers the log saw ALLOCATED (lk run --syms "Y" events) after sort SITE -- the wfE6 split:
                    X = a first-pass temp made before the sites, Y = a late temp made by the later passes;
  --late-temps      every CSE temp (symbol kind 3).  Caution: in the wfE6 bookmark case X is itself a CSE temp,
                    so this moves X and Y together and T cancels; use --late-after there.
T defaults to 0..0 (F only); a T range without one of the three options is refused.
For each (F, T) every operand key of the sites first..last (lk.py site numbers, inclusive) is RECOMPUTED through
its logged tree with c2's hash (lk.py key model: temps 64n mod 65536, symbols 32(h mod 2048) ^ h>>11, interior
sum child16 << i + op - 0x24A; need / count do not change), the sort is re-run (stable, descending) and compared
with the target:
  <flips> = comma list; "N" = site N must come out with a DIFFERENT permutation than now, "N=p0p1.." = site N must
  give exactly that permutation (indices into the incoming operand list), every other site in range must keep
  its current permutation.  "-" = no flips (where does the current state survive?).
Output: the exact solution set (no ranges that hide gaps): per F, the T values as maximal runs, then the count.
A site whose logged tree is depth-truncated keeps its logged hash for the truncated part (flagged "approx").
"""
import argparse, os, sys
import numpy as np
sys.dont_write_bytecode = True
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lk  # noqa: E402

M16 = 0xFFFF


class Ctx:
    def __init__(self, F, T, nmin, late_min, late_set=None, late_temps=False):
        self.F, self.T, self.nmin, self.late_min = F, T, nmin, late_min
        self.late_set, self.late_temps = late_set, late_temps
        self.approx = False


def shifted(num, sk, ctx):
    if num is None or num < ctx.nmin:
        return np.full(ctx.F.shape, num or 0, dtype=np.int64)
    if ctx.late_min is not None:
        late = num >= ctx.late_min
    elif ctx.late_set is not None:
        late = num in ctx.late_set
    else:
        late = ctx.late_temps and sk == 3
    return num + ctx.F + (ctx.T if late else 0)


def hash_arr(d, ctx):
    """16-bit hash of desc d under the shifts, as an int64 array over the (F, T) grid.
    Used where c2 READS a stored key (the comparator's operands, an interior node's children).  The grid's LAST ROW is
    the zero shift: where the model does not reproduce the logged key there (a stale key, e.g. 0 on an operand that
    was not re-keyed), the logged key is kept constant under every shift.  Where c2 RECOMPUTES through 0x1075D54E
    (the leaf under an IND, a forwarded temp's value node) _hash_raw is used instead."""
    h = _hash_raw(d, ctx)
    stored = (d[3] or 0) & M16
    if int(h[-1].flat[0]) != stored:
        return np.full(ctx.F.shape, stored, dtype=np.int64)
    return h


def _hash_raw(d, ctx):
    p, op, kind, key, w14, leaf, sub = d
    stored = np.full(ctx.F.shape, (key or 0) & M16, dtype=np.int64)
    if kind is None:
        ctx.approx = True
        return stored
    if kind <= 3:
        s, num, sk, fwd, nm = leaf
        if kind in (1, 2) and fwd:
            if not sub:
                ctx.approx = True
                return stored
            return _hash_raw(sub[0], ctx)
        n = shifted(num, sk, ctx)
        if kind in (1, 2) and sk == 3:
            return (n << 6) & M16
        n = n & 0xFFFFFFFF
        h = ((n >> 16) ^ n) & M16
        if kind == 3:
            return h
        return ((h >> 11) ^ ((h & 0x7FF) << 5)) & M16
    if kind >= 13:
        if sub is None:
            ctx.approx = True
            return stored
        h = np.zeros(ctx.F.shape, dtype=np.int64)
        for i, c in enumerate(sub):
            h = h + (hash_arr(c, ctx) << (i & 7))
        return (h + ((op & M16) - 0x24A)) & M16
    if kind in (5, 6) and leaf and leaf[0] == 0 and sub and lk.value_key(sub[0]) is not None:
        return _hash_raw(sub[0], ctx)      # copy rule: IND over a forwarded temp carries the value node's key
    if kind in (5, 6):
        w30, w34 = leaf
        h = np.full(ctx.F.shape, (lk.fold(w30) + (op & M16) - 0x24A) & 0xFFFFFFFF, dtype=np.int64)
        if w34:
            if not sub:
                ctx.approx = True
                return stored
            h = h + (_hash_raw(sub[0], ctx) << 8)
        return h & M16
    return stored          # constants and kinds 4 / 7..12: no symbol number inside


def site_perm_ok(s, target, ctx):
    """Boolean grid: the re-sorted permutation equals / differs as required."""
    keys = []
    for kd in s["kids"]:
        top = (kd[3] or 0) & 0xFFFF0000
        keys.append(top + hash_arr(kd, ctx))
    n = len(keys)
    K = np.stack(keys)                      # n x grid
    # stable descending: rank of i = #{j: K[j] > K[i]} + #{j < i: K[j] == K[i]}
    rank = np.zeros(K.shape, dtype=np.int64)
    for i in range(n):
        for j in range(n):
            if j == i:
                continue
            rank[i] += (K[j] > K[i]) | ((j < i) & (K[j] == K[i]))
    cur = s["perm"]
    want_any_change, want = target
    if want is not None:
        ok = np.ones(ctx.F.shape, dtype=bool)
        for pos, i in enumerate(want):
            ok &= rank[i] == pos
        return ok
    same = np.ones(ctx.F.shape, dtype=bool)
    for pos, i in enumerate(cur):
        same &= rank[i] == pos
    return ~same if want_any_change else same


def runs(vals):
    out = []
    for v in vals:
        if out and v == out[-1][1] + 1:
            out[-1][1] = v
        else:
            out.append([v, v])
    return ",".join(f"{a}" if a == b else f"{a}..{b}" for a, b in out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("log")
    ap.add_argument("first", type=int)
    ap.add_argument("last", type=int)
    ap.add_argument("flips")
    ap.add_argument("--func")
    ap.add_argument("--pass", dest="pas")
    ap.add_argument("--late-min", type=int)
    ap.add_argument("--min", type=int, default=0)
    ap.add_argument("--F", type=int, nargs=2, default=[-1024, 1023])
    ap.add_argument("--T", type=int, nargs=2, default=[0, 0])
    ap.add_argument("--late-after", type=int)
    ap.add_argument("--late-temps", action="store_true")
    a = ap.parse_args()
    tr = lk.load(a.log)
    name, ev = lk.function_events(tr, a.func)
    ss = [s for s in lk.sites(ev) if a.first <= s["n"] <= a.last and (not a.pas or s["pass"] == a.pas)]
    tg = {}
    if a.flips != "-":
        for it in a.flips.split(","):
            if "=" in it:
                n, p = it.split("=")
                tg[int(n)] = (False, [int(c) for c in p])
            else:
                tg[int(it)] = (True, None)
    Fv = np.arange(a.F[0], a.F[1] + 1)
    Tv = np.arange(a.T[0], a.T[1] + 1)
    F, T = np.meshgrid(Fv, Tv, indexing="ij")
    F = np.vstack([F, np.zeros((1, F.shape[1]), dtype=F.dtype)])      # last row: zero shift (stale-key test)
    T = np.vstack([T, np.zeros((1, T.shape[1]), dtype=T.dtype)])
    late_set = None
    if a.late_after is not None:
        late_set, k = set(), -1
        for e in ev:
            if e[0] == "S":
                k += 1
            elif e[0] == "Y" and k >= a.late_after and e[3] is not None:
                late_set.add(e[3])
        if not any(e[0] == "Y" for e in ev):
            sys.exit("fsolve: --late-after needs a log made with lk run --syms")
    if a.T != [0, 0] and a.late_min is None and late_set is None and not a.late_temps:
        sys.exit("fsolve: a T range needs --late-min, --late-after or --late-temps (which numbers are late?)")
    ctx = Ctx(F.astype(np.int64), T.astype(np.int64), a.min, a.late_min, late_set, a.late_temps)
    ok = np.ones(F.shape, dtype=bool)
    used = 0
    for s in ss:
        if s["perm"] is None or len(s["kids"]) < 2:
            continue
        ok &= site_perm_ok(s, tg.get(s["n"], (False, None)), ctx)
        used += 1
    ok = ok[:-1]
    miss = [n for n in tg if n not in {s["n"] for s in ss}]
    print(f"# {name}: sites {a.first}..{a.last} ({used} sorts), flips {a.flips}, late = "
          f"{'n >= %d' % a.late_min if a.late_min is not None else ('allocated after site %d (%d numbers)' % (a.late_after, len(late_set)) if late_set is not None else ('CSE temps' if a.late_temps else 'none (T = 0)'))}"
          f"{'  (APPROX: truncated trees)' if ctx.approx else ''}{'  MISSING sites %s' % miss if miss else ''}")
    tot = 0
    for fi, f in enumerate(Fv):
        ts = Tv[ok[fi]]
        if len(ts):
            tot += len(ts)
            print(f"F={f:5d}  T={runs(ts.tolist())}")
    print(f"# {tot} (F, T) solutions in F {a.F[0]}..{a.F[1]} x T {a.T[0]}..{a.T[1]}")


if __name__ == "__main__":
    main()
