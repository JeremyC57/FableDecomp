#!/usr/bin/env python3
"""assemble_nf329.py (NF329, 2026-11-24; repo root, A exported): hand-back tables from the dry triage (work/dry_all/GEN.tsv) and the
exported batches (batches/b*/RESULTS.tsv). One STATUS line per ENTRIES_q1 address (396). Writes STATUS.tsv, RENAMES.tsv, APPLIED.tsv,
REJECTED.tsv, TABLE_CHANGES.tsv, rows.txt, rows/<addr>.cpp, and HEADERS_<batch>.tsv (header, macro, rows) for header-route batches."""
import os, re, sys, shutil, collections, glob
A = os.environ["A"]
sys.path.insert(0, "tools/decomp_pipeline"); import naming_status as ns
ent = ns.landed_rows()
E = [l.split("\t") for l in open(f"{A}/work/ENTRIES_q1.tsv").read().splitlines()[1:]]
G = {l.split("\t")[0]: l.split("\t") for l in open(f"{A}/work/dry_all/GEN.tsv").read().splitlines()[1:]}
HOLD = {}
if os.path.exists(f"{A}/work/holds.tsv"):
    for l in open(f"{A}/work/holds.tsv").read().splitlines():
        if l.strip(): HOLD[l.split("\t")[0]] = l.split("\t", 1)[1]
R = {}
for b in sorted(glob.glob(f"{A}/batches/b*")):
    for l in open(f"{b}/RESULTS.tsv").read().splitlines()[1:]:
        q = l.split("\t"); R[q[0]] = q + [b]
_oh = open("rebuild/candidates/manual/naming/status/OPEN.tsv").readline().rstrip("\n").split("\t")
CAT = {l.split("\t")[0]: l.rstrip("\n").split("\t")[_oh.index("catalog")] for l in open("rebuild/candidates/manual/naming/status/OPEN.tsv").read().splitlines()[1:]}
os.makedirs(f"{A}/rows", exist_ok=True)
assert not os.listdir(f"{A}/rows"), "rows/ is not empty: clean it with clean.py first"
st, ren, app = [], [], []
for t, s, p, base in E:
    if t in HOLD: st.append((t, "skipped-held", p, HOLD[t])); continue
    if t in R and R[t][1] == "ACCEPT":
        r = R[t]; b = os.path.basename(r[5]); st.append((t, "done", p, f"batches/{b}: {r[4]}"))
        meth, cls = re.match(r'\?([^@]+)@([^@]+)@@', p).groups()
        ren.append((f"0x{t.upper()}", cls, meth, p, CAT.get(t) or f"{cls}::{meth}", r[2],
                    f"RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names {p} here + PDB name set; flatten_pilot v5 batches/{b} {r[4]}"))
        app.append((f"0x{t.upper()}", ent[t]["Source"], f"flatten v5: {r[2]} -> {p} ({r[4]})"))
        shutil.copy(f"{r[5]}/rows/{t}.cpp", f"{A}/rows/{t}.cpp"); continue
    if t in R:
        r = R[t]; st.append((t, "rejected" if r[1] == "REJECT" else "skipped-held", p, f"batches/{os.path.basename(r[5])} {r[1]}: {r[4]}")); continue
    g = G.get(t); why = g[5] if g else "not reached"
    st.append((t, "skipped-port", p, f"triage (base {base}; brief item 4, the thin remainder, not reached): {why}"))
with open(f"{A}/STATUS.tsv", "w") as o:
    o.write("address\tstatus\tpdb_spelling\treason\n" + "".join("\t".join(x) + "\n" for x in st))
with open(f"{A}/RENAMES.tsv", "w") as o:
    o.write("address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\n" + "".join("\t".join(x) + "\n" for x in ren))
with open(f"{A}/APPLIED.tsv", "w") as o:
    o.write("address\tfile\tchange\n" + "".join("\t".join(x) + "\n" for x in app))
tc = ["action\tsymbol\ttarget\twitness\tnote"]
for b in sorted(glob.glob(f"{A}/batches/b*")):
    if os.path.exists(f"{b}/TABLE_CHANGES.tsv"): tc += open(f"{b}/TABLE_CHANGES.tsv").read().splitlines()[1:]
open(f"{A}/TABLE_CHANGES.tsv", "w").write("\n".join(tc) + "\n")
with open(f"{A}/REJECTED.tsv", "w") as o:
    o.write("address\tpdb_spelling\treason\n" + "".join(f"{x[0]}\t{x[2]}\t{x[3]}\n" for x in st if x[1] == "rejected"))
open(f"{A}/rows.txt", "w").write("".join(x[0] + "\n" for x in st if x[1] == "done"))
# HEADERS_<batch>.tsv: the hdr/ header and macro each done header-route row needs
for b in sorted(glob.glob(f"{A}/batches/b*")):
    bn = os.path.basename(b); lines = []
    for l in open(f"{b}/RESULTS.tsv").read().splitlines()[1:]:
        q = l.split("\t")
        if q[1] != "ACCEPT" or "[route header]" not in q[4]: continue
        cls = re.match(r'\?[^@]+@([^@]+)@@', q[3]).group(1)
        lines.append((f"engine/{cls}.h", f"FABLE_RETAIL_{cls.upper()}_VIRTUAL_MODEL", q[0]))
    if lines:
        by = collections.defaultdict(list)
        for h, m, a in lines: by[(h, m)].append(a)
        with open(f"{A}/HEADERS_{bn}.tsv", "w") as o:
            o.write("header\tmacro\trows\n" + "".join(f"{h}\t{m}\t{','.join(sorted(v))}\n" for (h, m), v in sorted(by.items())))
        shutil.copy(f"{A}/HEADERS_{bn}.tsv", f"{b}/HEADERS.tsv")
print(collections.Counter(x[1] for x in st))
