#!/usr/bin/env python3
"""lka.py -- align two lk logs by site and list every comparator bit that differs.

  lka.py <base.json> <other.json> [--func SUBSTR] [--pass a|b] [--quiet] [--shifts]

Each log is an lk.py / lkrun.py comparator log of the same function from two compiles (two candidate texts, or
ours v a reference row's variant).  Sites (one operand-list sort each, see lk.py) are aligned on an id-free
signature: pass, parent op, parent line word (+0x14 low 16, line relative to the function start), and the SORTED
multiset of operand shapes (op / kind / need+count; no hash, no symbol number; the incoming order is left out because
it depends on earlier sorts), with difflib (autojunk off).  Operands are labelled by shape (+ rank by leaf numbers
among equal shapes).  Then for every aligned pair:
  - FLIP: the sort's OUTPUT order of operand labels differs; the operand pairs that swapped are listed;
  - the key deltas of the leaf numbers (X, Y) are collected (--shifts prints their histogram: the uniform
    shift fsolve.py models).
Unaligned runs (a text change that altered tree shapes, e.g. the edited statement itself) are reported as
"not comparable", never as flips.
Exit 0 always; the verdict line is `FLIPS <n> sorts in <m> trees`, then one line per flipped tree.
"""
import argparse, difflib, os, sys
from collections import Counter
sys.dont_write_bytecode = True
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lk  # noqa: E402


def site_list(path, func, pas):
    tr = lk.load(path)
    name, ev = lk.function_events(tr, func)
    ss = lk.sites(ev)
    if pas:
        ss = [s for s in ss if s["pass"] == pas]
    return name, ss


def nums(d):
    out = []
    for l in lk.leaves(d):
        t = l.split(":")[0].rstrip("=>")
        if t[:1] in "ts" and t[1:].isdigit():
            out.append(int(t[1:]))
    return tuple(out)


def labels(s):
    """Id-free operand labels: shape, plus the rank by leaf numbers among operands of the same shape."""
    sh = [lk.shape(k) for k in s["kids"]]
    lab = []
    for i, x in enumerate(sh):
        same = sorted((nums(s["kids"][j]), j) for j in range(len(sh)) if sh[j] == x)
        r = [j for _, j in same].index(i)
        lab.append(x + ("#%d" % r if len(same) > 1 else ""))
    return lab


def osig(s):
    """Order-independent site signature (the incoming order depends on earlier sorts, so it is not part of it)."""
    return "%s|%s|%s|%s" % (s["pass"], s["op"], s["line"], ";".join(sorted(lk.shape(k) for k in s["kids"])))


def outorder(s):
    lab = labels(s)
    return [lab[i] for i in s["perm"]] if s["perm"] is not None else None


def cmpmap(s):
    m = {}
    for c in s["cmps"]:
        if c[0] is not None:
            m[(c[0], c[1])] = c
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("base")
    ap.add_argument("other")
    ap.add_argument("--func")
    ap.add_argument("--pass", dest="pas")
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("--shifts", action="store_true")
    a = ap.parse_args()
    n1, A = site_list(a.base, a.func, a.pas)
    n2, B = site_list(a.other, a.func, a.pas)
    sm = difflib.SequenceMatcher(None, [osig(s) for s in A], [osig(s) for s in B], autojunk=False)
    flips = []
    aligned = 0
    unal = []
    dx = Counter()
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != "equal":
            unal.append((op, i1, i2, j1, j2))
            continue
        for k in range(i2 - i1):
            sa, sb = A[i1 + k], B[j1 + k]
            aligned += 1
            oa, ob = outorder(sa), outorder(sb)
            if oa != ob:
                # which operand pairs changed their relative order
                la, lb = labels(sa), labels(sb)
                pa = {x: k for k, x in enumerate(oa)}
                pb = {x: k for k, x in enumerate(ob)}
                pairs = [(x, y) for x in pa for y in pa if pa[x] < pa[y] and x in pb and y in pb and pb[x] > pb[y]]
                flips.append((sa, sb, pairs))
            for ka, kb in zip(sa["kids"], sb["kids"]):
                xa, _ = lk.single_num(ka)
                xb, _ = lk.single_num(kb)
                if xa is not None and xb is not None:
                    dx[xb - xa] += 1
    print(f"# base {a.base} ({n1}: {len(A)} sites)  other {a.other} ({n2}: {len(B)} sites)")
    print(f"# aligned {aligned} sites; unaligned runs {len(unal)} "
          f"({sum(i2 - i1 for _, i1, i2, _, _ in unal)} base / {sum(j2 - j1 for _, _, _, j1, j2 in unal)} other sites)")
    for op, i1, i2, j1, j2 in unal:
        la = sorted({s["line"] for s in A[i1:i2]})
        lb = sorted({s["line"] for s in B[j1:j2]})
        print(f"  not comparable: {op} base {i1}..{i2 - 1} lines {la[:8]}  other {j1}..{j2 - 1} lines {lb[:8]}")
    if a.shifts:
        print("# leaf-number deltas (other - base) over aligned operands:", dict(dx.most_common(12)))
    # group the flipped sorts by tree (c2 re-sorts one tree many times; the parent node address names the tree)
    gd = {}
    for sa, sb, dc in flips:
        gd.setdefault(sa["parent"], []).append((sa, sb, dc))
    groups = list(gd.items())
    print(f"FLIPS {len(flips)} sorts in {len(groups)} trees")
    for par, g in groups:
        sa, sb, dc = g[-1]
        print(f"  tree @{par:x} line {sa['line']} op {sa['op']:x}: {len(g)} sorts (base {g[0][0]['n']}..{sa['n']}, "
              f"passes {''.join(sorted({x[0]['pass'] for x in g}))}), swapped {dc}")
    if a.quiet:
        return
    for par, g in groups:
        sa, sb, dc = g[-1]
        print(f"## tree @{par:x}: last flipped sort base {sa['n']} / other {sb['n']}: pass {sa['pass']} line {sa['line']} "
              f"op {sa['op']:x}\n   out {outorder(sa)}\n    -> {outorder(sb)}")
        for tag, s in (("base ", sa), ("other", sb)):
            for i, kd in enumerate(s["kids"]):
                print(f"   {tag} [{i}] key {kd[3]:08x} {lk.shape(kd, 2)[:60]:60} {' '.join(lk.leaves(kd))[:90]}")
            for c in s["cmps"]:
                r = lk.cmp_row(s, c)
                if r:
                    print("   " + tag + " " + lk.fmt_row(r))


if __name__ == "__main__":
    main()
