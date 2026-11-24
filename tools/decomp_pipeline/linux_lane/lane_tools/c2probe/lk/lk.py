#!/usr/bin/env python3
"""lk.py -- c2 operand-sort comparator readout for one compile (VC7.1 13.10.3077 c2.dll).

  lk.py run   <src.cpp> <log.json> [lkrun options]      compile + log (= lkrun.py; see there)
  lk.py show  <log.json> [--func SUBSTR] [--pass a|b] [--line LO HI] [--final] [--min-kids N]   one line per comparator call
                                                      (--final: only the last sort of each tree)
  lk.py check <log.json> [--func SUBSTR]                recompute every logged key from structure (decode self-check)
  lk.py sites <log.json> [--func SUBSTR] [--json OUT]   the per-site table as JSON (for ad-hoc readers)

What a "site" is: one call of c2's merge sort of a commutative node's operand list (canonicaliser pass "a",
0x1075CC68, or the CSE pass "b", 0x1075D6AD).  Each comparator call inside it compares two operands a, b of that
list; c2 keeps a before b iff key(a) >= key(b) (unsigned 32-bit key at node+0x10, sorted descending).
key = need:8 | count:8 | hash:16 (0x1075D388); for two operands with equal need and count the 16-bit hash decides.
Leaf hash (0x1075D54E), number n = [sym+0x1c]:
  CSE temp (leaf kind 1/2, symbol kind 3):  hash = 64*n mod 65536      = 32 * (2n mod 2048)
  other symbol leaf (kinds 0..3):          hash = 32*(h mod 2048) ^ (h >> 11), h = (n >> 16) ^ (n & 0xffff)
  leaf kind 1/2 whose symbol has a value node (+0x14): the value node's hash (forwarded)
  interior (kind >= 13): sum(child_hash16 << (i & 7)) + op - 0x24A
So for two CSE-temp operands with first-pass id X (the operand that comes first in the incoming list) and late
id Y, the bit is B = [2X mod 2048 >= 2Y mod 2048] (the wfE6 rule; a tie keeps the order).  Every line prints
the real keys, which part decided (need / count / hash / tie), the leaf numbers inside each operand, B, and,
where both operands are temps, the rule's prediction as a cross-check.

Output line of `show` (one per comparator call):
  site#  pass  line  parent-op  [i]v[j]  B  by  keyA keyB  X=..  Y=..  rule  A-leaves | B-leaves
X / Y are the leaf numbers when the operand is a single leaf (a forwarded temp shows its own number; "*n" = an
IND over leaf n, whose key only sees the low byte of n's hash, i.e. n mod 4 for a temp); "rule"
is [2X mod 2048 >= 2Y mod 2048], printed only when BOTH operands are plain CSE temps (kind 1/2, symbol kind 3, no
value node) -- other symbols hash as 32(h mod 2048) ^ h>>11 -- ("-" otherwise; "!" = disagreement with B on a
hash-decided compare).
"""
import json, sys, os
from collections import Counter

sys.dont_write_bytecode = True
M16 = 0xFFFF


# ---------------------------------------------------------------- loading
def load(path):
    return json.load(open(path))


def windows(tr):
    """[(function name, [events])] in compile order."""
    res, cur = [], []
    for e in tr["events"]:
        if e[0] == "P":
            res.append((e[1], cur))
            cur = []
        else:
            cur.append(e)
    if cur:
        res.append(("<tail>", cur))
    return res


def function_events(tr, func=None):
    ws = windows(tr)
    if func is None:
        func = (tr.get("meta") or {}).get("func")
    if func is None:
        if len(ws) == 1:
            return ws[0]
        raise SystemExit("several functions in the log; pass --func")
    hits = [w for w in ws if func in w[0]]
    if len(hits) != 1:
        raise SystemExit(f"{len(hits)} functions match {func!r}: {[w[0] for w in ws][:20]}")
    return hits[0]


# ---------------------------------------------------------------- key model
def fold(x):
    x &= 0xFFFFFFFF
    return ((x >> 16) ^ x) & M16


def leaf_hash(d):
    """Recompute the 16-bit hash of a logged desc; None if the log lacks what is needed."""
    p, op, kind, key, w14, leaf, sub = d
    if kind is None:
        return None
    if kind <= 3:
        s, num, sk, fwd, nm = leaf
        if kind in (1, 2) and fwd:
            if not sub:
                return None
            return sub[0][3] & M16 if sub[0][3] is not None else None
        if num is None:
            return None
        if kind in (1, 2) and sk == 3:
            return (num << 6) & M16
        h = fold(num)
        if kind == 3:
            return h
        return ((h >> 11) ^ ((h & 0x7FF) << 5)) & M16
    if kind == 7:
        lo, hi = leaf
        return fold(lo ^ hi)
    if kind in (5, 6):
        w30, w34 = leaf
        h = (fold(w30) + (op & M16) - 0x24A) & 0xFFFFFFFF
        if w34:
            if not sub:
                return None
            ch = leaf_hash(sub[0])          # the operand's own key can be stale (0) when this node is keyed
            if ch is None:
                return None
            h += ch << 8
        return h & M16
    if 10 <= kind <= 12:
        return 0
    if kind in (4, 8, 9):
        if not leaf or len(leaf) < 2 or leaf[1] is None:
            return None
        return fold(leaf[1])
    if kind >= 13:
        if sub is None:
            return None
        h = 0
        for i, c in enumerate(sub):
            if c[3] is None:
                return None
            h += (c[3] & M16) << (i & 7)
        return (h + ((op & M16) - 0x24A)) & M16
    return None      # kinds 4, 8, 9: the log does not carry the field


def value_key(d):
    """Key of the value node a forwarded CSE-temp leaf stands for (None if d is not one)."""
    p, op, kind, key, w14, leaf, sub = d
    if kind in (1, 2) and leaf and leaf[3] and sub:
        return sub[0][3]
    return None


def key_model(d):
    """Full 32-bit key recomputed from the desc (children's stored keys for interior nodes).
    Measured copy rules (0x1075D6AD keeps the value node's key, lk.py check on 0x00B4D370: 1837 / 1837 operands):
    a forwarded CSE-temp leaf (kind 1/2, symbol +0x14 != 0) carries its value node's full key, and an IND node
    (kind 5/6, +0x30 == 0) over such a leaf carries that same key."""
    p, op, kind, key, w14, leaf, sub = d
    v = value_key(d)
    if v is not None:
        return v
    if kind in (5, 6) and leaf and leaf[0] == 0 and sub:
        v = value_key(sub[0])
        if v is not None:
            return v
    h = leaf_hash(d)
    if h is None:
        return None
    if 1 <= kind <= 12:
        return h | ((0 if kind in (7, 8, 9) else 1) << 16)
    if sub is None or not sub:
        return None
    need = (sub[0][3] >> 24) & 0xFF
    cnt = 1 + ((sub[0][3] >> 16) & 0xFF)
    for c in sub[1:]:
        cn = (c[3] >> 24) & 0xFF
        cnt += (c[3] >> 16) & 0xFF
        if cn > need:
            need = cn + 1
        elif cn == need:
            need += 1
    if kind == 0x0F:
        need += 7
    elif kind == 0x13:
        need += 5
    return ((need & 0xFF) << 24) | ((cnt & 0xFF) << 16) | h


def walk(d):
    yield d
    for c in d[6] or []:
        yield from walk(c)


# ---------------------------------------------------------------- naming
def leaves(d, out=None):
    """Leaf numbers inside an operand: 't<n>' CSE temp, 's<n>' other symbol, 'c<v>' constant."""
    if out is None:
        out = []
    p, op, kind, key, w14, leaf, sub = d
    if kind is not None and kind <= 3 and leaf:
        s, num, sk, fwd, nm = leaf
        tag = ("t" if sk == 3 else "s") + str(num)
        if nm:
            tag += ":" + nm
        if fwd:
            tag += "=>"
        out.append(tag)
        return out
    if kind == 7 and leaf:
        out.append("c%x" % leaf[0])
        return out
    for c in sub or []:
        leaves(c, out)
    return out


def single_num(d):
    """(number, symbol kind) of a single-leaf operand, else (None, None)."""
    p, op, kind, key, w14, leaf, sub = d
    if kind is not None and kind <= 3 and leaf:
        return leaf[1], leaf[2]
    return None, None


def plain_temp(d):
    """A CSE-temp leaf (kind 1/2, symbol kind 3) without a value node: its hash is 64n mod 65536, the only operand
    shape the wfE6 rule [2X mod 2048 >= 2Y mod 2048] is stated for."""
    p, op, kind, key, w14, leaf, sub = d
    return kind in (1, 2) and bool(leaf) and leaf[2] == 3 and not leaf[3]


def ind_num(d):
    """Number of the leaf under an IND operand (kind 5/6 with a leaf at +0x34), else None.  Its hash is
    fold(+0x30) + op - 0x24A + (leaf hash << 8): only the leaf hash's LOW BYTE counts (for a CSE temp: n mod 4)."""
    p, op, kind, key, w14, leaf, sub = d
    if kind in (5, 6) and sub and sub[0][2] is not None and sub[0][2] <= 3 and sub[0][5]:
        return sub[0][5][1]
    return None


def opnum(d):
    n, k = single_num(d)
    if n is not None:
        return str(n)
    n = ind_num(d)
    return "*" + str(n) if n is not None else None


def shape(d, depth=3):
    """Id-free structural signature of an operand (ops / kinds / need / count; no hashes, no numbers)."""
    p, op, kind, key, w14, leaf, sub = d
    if kind is None:
        return "?"
    s = "%x.%d.%d" % (op & 0xFFFF, kind, (key >> 16) & 0xFFFF if key is not None else -1)
    if kind <= 3 and leaf:
        s += "s%s" % leaf[2]
    if kind == 7 and leaf:
        s += "=%x" % leaf[0]
    if sub and depth > 0 and kind >= 13:
        # operand order inside a subtree is itself a sort result: keep it out of the signature
        s += "(" + ",".join(sorted(shape(c, depth - 1) for c in sub)) + ")"
    return s


# ---------------------------------------------------------------- sites
def sites(events):
    """One dict per sort: pass, parent op/line, children descs, compares [(i, j, B, by)], permutation."""
    res = []
    cur = None
    for e in events:
        if e[0] == "S":
            par, kids = e[2], e[3]
            cur = {"n": len(res), "pass": e[1], "parent": par[0], "op": par[1] & 0xFFFF if par[1] else None,
                   "line": (par[4] & 0xFFFF) if par[4] is not None else None, "kids": kids, "cmps": [], "perm": None}
            res.append(cur)
        elif e[0] == "C" and cur is not None:
            idx = {k[0]: i for i, k in enumerate(cur["kids"])}
            a, b = e[1], e[2]
            if a in idx and b in idx:
                i, j = idx[a], idx[b]
                ka, kb = cur["kids"][i][3], cur["kids"][j][3]
                B = 1 if ka >= kb else 0
                if ka >> 24 != kb >> 24:
                    by = "need"
                elif (ka >> 16) & 0xFF != (kb >> 16) & 0xFF:
                    by = "count"
                elif ka & M16 != kb & M16:
                    by = "hash"
                else:
                    by = "tie"
                cur["cmps"].append((i, j, B, by))
            else:
                cur["cmps"].append((None, None, None, "foreign"))
        elif e[0] == "R" and cur is not None:
            idx = {k[0]: i for i, k in enumerate(cur["kids"])}
            cur["perm"] = [idx.get(p) for p in e[3]]
            cur = None
    for s in res:
        s["sig"] = "%s|%s|%s|%s" % (s["pass"], s["op"], s["line"], ";".join(shape(k) for k in s["kids"]))
    return res


def cmp_row(s, c):
    i, j, B, by = c
    if i is None:
        return None
    a, b = s["kids"][i], s["kids"][j]
    X, xk = single_num(a)
    Y, yk = single_num(b)
    rule = "-"
    if plain_temp(a) and plain_temp(b):
        r = 1 if (2 * X) % 2048 >= (2 * Y) % 2048 else 0
        rule = str(r) + ("" if r == B or by != "hash" else "!")
    return {"site": s["n"], "pass": s["pass"], "line": s["line"], "op": s["op"], "i": i, "j": j, "B": B, "by": by,
            "keyA": a[3], "keyB": b[3], "X": opnum(a), "Y": opnum(b), "xk": xk, "yk": yk, "rule": rule,
            "la": leaves(a), "lb": leaves(b)}


def fmt_row(r):
    return ("%5d %s L%-5s op%-4x [%d]v[%d] B%d %-5s %08x %08x X=%-6s Y=%-6s rule %-2s %s | %s" % (
        r["site"], r["pass"], r["line"], r["op"] or 0, r["i"], r["j"], r["B"], r["by"], r["keyA"], r["keyB"],
        r["X"], r["Y"], r["rule"], " ".join(r["la"])[:70], " ".join(r["lb"])[:70]))


# ---------------------------------------------------------------- commands
def cmd_show(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="lk.py show")
    ap.add_argument("log")
    ap.add_argument("--func")
    ap.add_argument("--pass", dest="pas")
    ap.add_argument("--line", type=int, nargs=2, metavar=("LO", "HI"))
    ap.add_argument("--final", action="store_true",
                    help="only the LAST sort of each parent node (what the code generator sees; a node address that c2 "
                         "frees and reuses counts as one parent)")
    ap.add_argument("--min-kids", type=int, default=2)
    a = ap.parse_args(argv)
    tr = load(a.log)
    name, ev = function_events(tr, a.func)
    ss = sites(ev)
    if a.final:
        last = {}
        for s in ss:
            last[s["parent"]] = s["n"]
        keep = set(last.values())
        ss = [s for s in ss if s["n"] in keep]
    ss = [s for s in ss if len(s["kids"]) >= a.min_kids]
    print(f"# {name}: {len(ss)} sorts, {sum(len(s['cmps']) for s in ss)} comparator calls")
    for s in ss:
        if a.pas and s["pass"] != a.pas:
            continue
        if a.line and not (s["line"] is not None and a.line[0] <= s["line"] <= a.line[1]):
            continue
        for c in s["cmps"]:
            r = cmp_row(s, c)
            if r:
                print(fmt_row(r))


def cmd_check(argv):
    """Decode self-check: every OPERAND the comparator reads (the direct children of each sorted list) must have the
    key the model recomputes from its logged structure, and every sort's result must be the stable descending key
    order.  Deeper nodes are not checked (c2 leaves stale keys there, e.g. 0 on a leaf under an IND)."""
    import argparse
    ap = argparse.ArgumentParser(prog="lk.py check")
    ap.add_argument("log")
    ap.add_argument("--func")
    a = ap.parse_args(argv)
    tr = load(a.log)
    name, ev = function_events(tr, a.func)
    ok = bad = unk = 0
    badk = Counter()
    ex = []
    for e in ev:
        if e[0] != "S":
            continue
        for d in e[3]:
            m = key_model(d)
            if m is None:
                unk += 1
                badk["not decodable: pass %s kind %s op %x" % (e[1], d[2], (d[1] or 0) & M16)] += 1
            elif m == d[3]:
                ok += 1
            else:
                bad += 1
                badk["WRONG: pass %s kind %s op %x" % (e[1], d[2], d[1] & M16)] += 1
                if len(ex) < 8:
                    ex.append("%x kind %d op %x key %08x model %08x leaf %s" % (d[0], d[2], d[1] & M16, d[3], m, d[5]))
    perm_bad = 0
    for s in sites(ev):
        if s["perm"] is None:
            continue
        order = sorted(range(len(s["kids"])), key=lambda i: -s["kids"][i][3])   # stable descending
        if order != s["perm"]:
            perm_bad += 1
    print(f"# {name}: operand keys recomputed ok {ok}, wrong {bad}, not decodable {unk}; sorts whose result != "
          f"stable descending key order: {perm_bad}")
    for k, v in badk.most_common(12):
        print("  ", k, v)
    for x in ex:
        print("   e.g.", x)


def cmd_sites(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="lk.py sites")
    ap.add_argument("log")
    ap.add_argument("--func")
    ap.add_argument("--json")
    a = ap.parse_args(argv)
    tr = load(a.log)
    name, ev = function_events(tr, a.func)
    ss = sites(ev)
    rows = []
    for s in ss:
        rows.append({"n": s["n"], "pass": s["pass"], "op": s["op"], "line": s["line"], "sig": s["sig"],
                     "perm": s["perm"], "cmps": [cmp_row(s, c) for c in s["cmps"]]})
    if a.json:
        json.dump({"func": name, "sites": rows}, open(a.json, "w"))
    print(f"# {name}: {len(rows)} sites")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    c, argv = sys.argv[1], sys.argv[2:]
    if c == "run":
        sys.argv = [os.path.join(os.path.dirname(os.path.abspath(__file__)), "lkrun.py")] + argv
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import lkrun
        lkrun.main()
    elif c == "show":
        cmd_show(argv)
    elif c == "check":
        cmd_check(argv)
    elif c == "sites":
        cmd_sites(argv)
    else:
        print(__doc__)
        sys.exit(1)


if __name__ == "__main__":
    main()
