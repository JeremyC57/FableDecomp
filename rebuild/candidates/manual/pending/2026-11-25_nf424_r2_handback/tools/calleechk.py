#!/usr/bin/env python3
"""calleechk.py: every relocation of each thin ACCEPT row's leaf COMDAT resolved against retail (rel32 / dir32 at the same offset); the
relocation symbol must be bound at that retail target by reloc_bindings.tsv or be the target's own self symbol. Writes r2/work/calleechk.tsv."""
import sys, glob, struct, re
from collections import defaultdict
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
bind = defaultdict(set)
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    q = l.split("\t"); bind[q[0]].add(q[1].lower())
selfs = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    q = l.split("\t"); selfs[q[0]] = q[1]
gt = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/gen_thin.tsv")}
res = {}
for f in sorted(glob.glob(A + "r2/work/cc2_*.log")):
    tag = f.split("cc2_")[1][:-4]
    for l in open(f):
        q = l.split()
        if len(q) > 2 and re.match(r"[0-9a-f]{8}$", q[0]): res[q[0]] = tag
out = []
for a in sys.argv[1:]:
    leaf = gt[a][4]; obj = A + "r2/obj/%s/%s.obj" % (res[a], a)
    retail = bytes.fromhex(nf.catalog()[a]["bytes"]); va0 = int(a, 16)
    secs, sl = nf.coff(obj)
    cand = [(n, v, sec) for i, n, v, sec, t, scl in sl if scl == 2 and sec > 0 and n == leaf]
    if not cand: out.append("%s\tNOLEAF" % a); continue
    n, v, sec = cand[0]; s = secs[sec - 1]; bad = []; good = []
    for off, sym, typ in s["rel"]:
        x = struct.unpack_from("<I", retail, off)[0]
        tgt = "%08x" % (((va0 + off + 4 + x) & 0xffffffff) if typ == 0x14 else x)
        ok = tgt in bind[sym] or selfs.get(tgt) == sym
        (good if ok else bad).append("%s@%s%s" % (sym, tgt, "" if ok else "[self=%s]" % selfs.get(tgt, "-")))
    out.append("%s\t%s\t%s\t%s" % (a, "OK" if not bad else "BAD", ";".join(bad) or "-", ";".join(good)))
open(A + "r2/work/calleechk.tsv", "w").write("\n".join(out) + "\n")
from collections import Counter
print(Counter(o.split("\t")[1] for o in out))
