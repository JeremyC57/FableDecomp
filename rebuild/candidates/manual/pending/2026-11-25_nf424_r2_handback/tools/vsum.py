#!/usr/bin/env python3
"""vsum.py: per row of the round-2 vland runs, HEAD and NEW grade/test, and NEW-side RELOC_NAME warnings whose target is not in a HEAD
RELOC_NAME warning of the same row. Writes r2/work/vsum.tsv."""
import re, glob
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
g = {}; w = {}
for p in glob.glob(A + "r2/work/vland_rp*.log"):
    side = p[-5]
    for l in open(p):
        m = re.match(r"\s*([0-9a-f]{8})\s+(RELOCATION_MATCH\S*|MATCH\S*|DIFFER\S*|SRC_FAIL|TCC_FAIL)\s+(PASS|FAIL|LINK_FAIL|TCC_FAIL)\b", l)
        if m: g[(m.group(1), side)] = m.group(2) + "/" + m.group(3)
        m = re.match(r"\s*([0-9a-f]{8})\s+WARN RELOC_NAME: (\S+) -> ([0-9a-f]{8}) labelled '([^']*)'", l)
        if m: w.setdefault((m.group(1), side), []).append((m.group(3), m.group(2), m.group(4)))
addrs = sorted(set(a for a, s in g))
from collections import defaultdict
bind = defaultdict(set)
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    q = l.split("\t"); bind[q[0]].add(q[1].lower())
def bk(s, t): return "bound_here" if t in bind[s] else ("bound_elsewhere" if bind[s] else "unbound")
out = []
for a in addrs:
    ht = set(t for t, s, lab in w.get((a, "h"), []))
    new = ["%s->%s '%s' [%s%s]" % (s, t, lab, bk(s, t), "" if t not in ht else ", also at HEAD") for t, s, lab in w.get((a, "n"), [])]
    bad = [x for x in new if "[bound_here" not in x]
    out.append("\t".join([a, g.get((a, "h"), "-"), g.get((a, "n"), "-"), "; ".join(bad) or "-", "; ".join(x for x in new if "[bound_here" in x) or "-"]))
open(A + "r2/work/vsum.tsv", "w").write("\n".join(out) + "\n")
from collections import Counter
print(len(out), Counter((o.split("\t")[2], o.split("\t")[3] != "-", o.split("\t")[4] != "-") for o in out))
