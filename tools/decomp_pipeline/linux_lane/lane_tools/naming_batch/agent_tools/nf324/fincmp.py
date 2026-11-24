#!/usr/bin/env python3
"""fincmp.py <final gen dir> <graded gen dir>... (NF324): compare each accepted row's source from the final tool (gen dir) with the source
that was graded; prints SAME / DIFF / FINAL_SKIP <reason> per address and counts. A DIFF row is regraded; a FINAL_SKIP row is held."""
import sys, os, collections
fin = sys.argv[1]; graded = sys.argv[2:]
G = {l.split("\t")[0]: l.split("\t") for l in open(f"{fin}/GEN.tsv").read().splitlines()[1:]}
c = collections.Counter()
for a, g in sorted(G.items()):
    if g[1] != "GEN": print("FINAL_SKIP", a, g[5]); c["FINAL_SKIP"] += 1; continue
    src = [f"{d}/gen/{a}.cpp" for d in graded if any(l.startswith(a + "\tACCEPT") for gp in __import__("glob").glob(f"{d}/run*/GRADE.tsv") for l in open(gp))]
    same = src and open(src[0]).read() == open(f"{fin}/gen/{a}.cpp").read()
    print("SAME" if same else "DIFF", a); c["SAME" if same else "DIFF"] += 1
print(dict(c))
