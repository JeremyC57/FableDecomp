# mklabels.py <in.tsv: addr<TAB>evidence> <out.tsv>: address, name (qualified, from the row's // Leaf: as sync_catalog_names does), evidence
import sys, re
sys.path.insert(0, "tools")
import sync_catalog_names as C
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
src = {m.group(1).lower(): m.group(2) for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'\s*\n\s*Module = '[^']*'\s*\n\s*Source = '([^']*)'", ps)}
rows = [l.rstrip("\n").split("\t", 1) for l in open(sys.argv[1]) if l.strip()]
leaves = {}
for a, ev in rows:
    lf = C.leaf_of(C.SRC / src[a.lower()]); leaves[a] = lf
dm = C.demangle([l for l in leaves.values() if l and l.startswith("?")])
out = ["address\tname\tevidence"]
for a, ev in rows:
    lf = leaves[a]; nm = C.qualified(dm[lf]) if lf in dm else None
    if not nm: nm = lf; ev += " (name = Leaf; not demangled)"
    out.append(f"{a.lower()}\t{nm}\t{ev}")
open(sys.argv[2], "w").write("\n".join(out) + "\n"); print(len(out) - 1, "labels")
