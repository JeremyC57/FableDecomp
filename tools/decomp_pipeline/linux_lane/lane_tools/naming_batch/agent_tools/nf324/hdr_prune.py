#!/usr/bin/env python3
"""hdr_prune.py <agent folder> <batch dir>... (NF324, 2026-11-24; repo root): keep in hdr/ only the edited headers that an ACCEPTed row of the given
(header-route) batches uses, and write <batch>/HEADERS.tsv (address, header, opt-in macro) for each batch. A header no accepted row uses
is moved (os.replace, nothing deleted) with its .diff and orig/ copy to work/hdr_unused/, and listed in work/hdr_unused/LIST.txt.
The row -> macro map comes from each batch's work GEN.tsv (switches column), found through <batch>/SOURCE (one line: the work dir)."""
import os, sys, re
A, batches = sys.argv[1], sys.argv[2:]
edited = {f[:-2] for f in os.listdir(f"{A}/hdr/engine") if f.endswith(".h")}
used = set()
for b in batches:
    work = open(f"{b}/SOURCE").read().strip()
    gen = {l.split("\t")[0]: l.split("\t") for l in open(f"{work}/GEN.tsv").read().splitlines()[1:]}
    acc = [l.split("\t")[0] for l in open(f"{b}/RESULTS.tsv").read().splitlines()[1:] if l.split("\t")[1] == "ACCEPT"]
    rows = []
    for a in acc:
        for m in gen[a][8].split(","):
            mm = re.match(r'FABLE_RETAIL_(\w+)_RTTI_DECLS$', m)
            if not mm: continue
            cls = [c for c in edited if c.upper() == mm.group(1)]
            repo = [f for f in os.listdir("rebuild/include/engine") if f[:-2].upper() == mm.group(1)]
            hdr = f"engine/{cls[0]}.h (hdr/ edit)" if cls else f"engine/{repo[0] if repo else mm.group(1)} (already in the repo header)"
            if cls: used.add(cls[0])
            rows.append((a, hdr, m))
    with open(f"{b}/HEADERS.tsv", "w") as o:
        o.write("address\theader\topt_in_macro\n" + "".join("\t".join(r) + "\n" for r in rows))
un = sorted(edited - used)
os.makedirs(f"{A}/work/hdr_unused/engine", exist_ok=True); os.makedirs(f"{A}/work/hdr_unused/orig", exist_ok=True)
for c in un:
    os.replace(f"{A}/hdr/engine/{c}.h", f"{A}/work/hdr_unused/engine/{c}.h")
    if os.path.exists(f"{A}/hdr/{c}.diff"): os.replace(f"{A}/hdr/{c}.diff", f"{A}/work/hdr_unused/{c}.diff")
    if os.path.exists(f"{A}/orig/hdr_{c}.h"): os.replace(f"{A}/orig/hdr_{c}.h", f"{A}/work/hdr_unused/orig/hdr_{c}.h")
open(f"{A}/work/hdr_unused/LIST.txt", "a").write("".join(c + "\n" for c in un))
print(f"hdr_prune: {len(used)} headers kept, {len(un)} moved to work/hdr_unused")
