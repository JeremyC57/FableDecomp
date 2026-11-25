#!/usr/bin/env python3
"""definers.py <wd> <rows.txt>: landed rows (self_symbols + // Leaf: map) whose self is a symbol any row object (HEAD or new wd) defines or
references; prints their addresses (batch rows excluded). Used as the /FORCE link definer census."""
import sys, glob, subprocess, re
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"
wds = sys.argv[1].split(","); rows = open(sys.argv[2]).read().split()
s2a = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 1 and f[1]: s2a.setdefault(f[1], set()).add(f[0])
for l in open(W + "/work/selfmap_all.tsv"):
    a, _, s = l.rstrip("\n").split("\t"); s2a.setdefault(s, set()).add(a)
syms = set()
for wd in wds:
    for a in rows:
        for o in glob.glob(f"{wd}/**/{a}/{a}.obj", recursive=True):
            out = subprocess.run(["llvm-nm", o], capture_output=True, text=True).stdout
            for l in out.splitlines():
                p = l.split()
                if len(p) >= 2 and p[-2] in ("T", "t", "R", "r", "D", "U"): syms.add(p[-1])
d = set()
for s in syms:
    for a in s2a.get(s, ()):
        if a not in rows: d.add(a)
print(" ".join(sorted(d)))
