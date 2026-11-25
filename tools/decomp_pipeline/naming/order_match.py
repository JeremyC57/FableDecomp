#!/usr/bin/env python3
"""order_match.py <rec.json> [--pdb ghidra_out/egor_obj_functions.tsv] [--anchors N] [--no-size] [--out preds.tsv]

Object-order name predictor (2026-11-25, owner request "check whether the PDB function order can predict names").

A PDB's module streams list each object's functions in source order (ghidra_out/egor_obj_functions.tsv: object, index_in_object,
va, code_size, kind, proc_name, decorated_public). The retail linker keeps one object's functions contiguous and in that order, so a
retail catalog address that sits between known addresses of the SAME object, with the same number of catalog functions between them
as the PDB has between the anchors, is predicted to be the PDB function at the matching index.

Known anchors are addresses the naming tracker measures `agree_pdb` (one spelling, a PDB name) whose decorated name occurs once in the
PDB's module streams. Guards: N anchors on each side, all in one object with consistent offsets (--anchors, default 2), and the retail
catalog size within 0.67x-1.5x of the PDB code size (off with --no-size).

Measured on 2026-11-25 (Ego_r.pdb, leave-one-out over 17,282 usable anchors): N=1 + size 98.0% precise (4,735 right, 98 wrong);
N=2 + size 99.5% (2,571 right, 13 wrong). On open split_fixable addresses it agrees with the tracker's PDB spelling 96-98%.
A prediction is ONE source (EXTRA_RULES: names need two); pair it with RTTI, a caller binding, the FSE manifest or the PDB signature.

<rec.json> comes from `naming_status.py --dump-rec <file>`. Prints the leave-one-out score and writes the predictions for open
(and agree_unverified) addresses: target, status, predicted decorated name, PDB object.
"""
import argparse
import bisect
import collections
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
OPEN = {"standin", "split_unknown", "split_fixable", "rtti_identified", "rtti_conflict", "agree_unverified"}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rec")
    ap.add_argument("--pdb", default=os.path.join(ROOT, "ghidra_out", "egor_obj_functions.tsv"))
    ap.add_argument("--anchors", type=int, default=2)
    ap.add_argument("--no-size", action="store_true")
    ap.add_argument("--out", default="")
    a = ap.parse_args()
    rec = json.load(open(a.rec))
    rows = [l.split("\t") for l in open(os.path.join(ROOT, "rebuild", "oracles", "auto-re-candidates.tsv"),
                                       encoding="utf-8").read().splitlines()[1:] if l.strip()]
    rsize = {int(f[0], 16): int(f[2]) for f in rows if len(f) > 2 and f[2].isdigit()}
    cat = sorted(rsize)
    pos = {x: i for i, x in enumerate(cat)}
    known = {int(t, 16): r["pdb_spelling"] for t, r in rec.items() if r["status"] == "agree_pdb" and r.get("pdb_spelling")}
    objs = collections.defaultdict(list)
    for l in open(a.pdb, encoding="utf-8", errors="replace"):
        if l.startswith("#"):
            continue
        f = l.rstrip("\n").split("\t")
        if len(f) < 7:
            continue
        objs[f[0]].append((int(f[1]), f[6] or f[5], int(f[3]) if f[3].isdigit() else 0))
    where = collections.defaultdict(list)
    for o in objs:
        objs[o].sort()
        for k, (_i, d, _s) in enumerate(objs[o]):
            where[d].append((o, k))
    uniq = {d: w[0] for d, w in where.items() if len(w) == 1}
    kp = sorted((pos[x], x) for x in known if x in pos and known[x] in uniq)
    kpos = [p for p, _ in kp]
    n = a.anchors

    def predict(p, exclude=None):
        i = bisect.bisect_left(kpos, p)
        left = [x for x in kp[max(0, i - n - 1):i] if x[1] != exclude]
        right = [x for x in kp[i:i + n + 1] if x[1] != exclude and x[0] != p]
        if len(left) < n or len(right) < n:
            return None
        anchors = left[-n:] + right[:n]
        ws = [uniq[known[x[1]]] for x in anchors]
        if len({w[0] for w in ws}) != 1:
            return None
        o, base = ws[0][0], ws[0][1] - anchors[0][0]
        if any(w[1] - x[0] != base for w, x in zip(ws, anchors)):
            return None
        k = base + p
        if not 0 <= k < len(objs[o]):
            return None
        _i, d, es = objs[o][k]
        rs = rsize.get(cat[p], 0)
        if not a.no_size and (es == 0 or rs == 0 or not 0.67 <= rs / es <= 1.5):
            return None
        return d, o

    ok = bad = 0
    for p, x in kp:
        g = predict(p, exclude=x)
        if g:
            ok, bad = (ok + 1, bad) if g[0] == known[x] else (ok, bad + 1)
    print("leave-one-out: anchors %d, right %d, wrong %d, precision %.4f" % (len(kp), ok, bad, ok / max(1, ok + bad)))
    out = []
    for t, r in sorted(rec.items()):
        x = int(t, 16)
        if r["status"] in OPEN and x in pos:
            g = predict(pos[x])
            if g:
                out.append((t, r["status"], g[0], g[1]))
    print("predictions:", dict(collections.Counter(s for _, s, _, _ in out)), "total", len(out))
    if a.out:
        with open(a.out, "w", encoding="utf-8") as f:
            f.write("target\tstatus\tpredicted\tpdb_object\n")
            for row in out:
                f.write("\t".join(row) + "\n")


if __name__ == "__main__":
    main()
