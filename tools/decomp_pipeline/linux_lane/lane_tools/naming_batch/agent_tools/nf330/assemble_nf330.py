#!/usr/bin/env python3
"""assemble_nf330.py (NF330, 2026-11-24; repo root, A exported): NF329's assemble_nf329.py for this pass. Hand-back tables from the gen4 /
gen5 triage and the exported batches (batches/b*/RESULTS.tsv): one STATUS line per ENTRIES address (95), with the measured skip reasons in
work/skips.tsv; RENAMES / APPLIED / REJECTED / TABLE_CHANGES / rows.txt / rows/<addr>.cpp; per batch hdr/engine, hdr/diff, orig/ (headers +
HEAD row sources), HEADERS.tsv, PROBES.tsv, EQUIV_CHECK.tsv, EQUIV_CONTROL.tsv; HEADERS_<batch>.tsv at the top."""
import os, re, sys, shutil, collections, glob, difflib
A = os.environ["A"]
sys.path.insert(0, "tools/decomp_pipeline"); import naming_status as ns
ent = ns.landed_rows()
E = [l.split("\t") for l in open(f"{A}/work/ENTRIES.tsv").read().splitlines()[1:]]
SK = {l.split("\t")[0]: l.split("\t", 1)[1] for l in open(f"{A}/work/skips.tsv").read().splitlines() if l.strip()}
BH = {}   # batch -> header classes (models and base opt-ins)
for l in open(f"{A}/work/batch_headers.tsv").read().splitlines():
    if l.strip(): b, c = l.split("\t"); BH.setdefault(b, []).append(c)
R = {}
for b in sorted(glob.glob(f"{A}/batches/b*")):
    for l in open(f"{b}/RESULTS.tsv").read().splitlines()[1:]:
        q = l.split("\t"); R[q[0]] = q + [b]
_oh = open("rebuild/candidates/manual/naming/status/OPEN.tsv").readline().rstrip("\n").split("\t")
CAT = {l.split("\t")[0]: l.rstrip("\n").split("\t")[_oh.index("catalog")] for l in open("rebuild/candidates/manual/naming/status/OPEN.tsv").read().splitlines()[1:]}
if os.path.isdir(f"{A}/rows") and os.listdir(f"{A}/rows"): sys.exit("rows/ is not empty: clean it with clean.py first")
os.makedirs(f"{A}/rows", exist_ok=True)
st, ren, app = [], [], []
for t, s, p, base in E:
    if t in R and R[t][1] == "ACCEPT":
        r = R[t]; b = os.path.basename(r[5]); st.append((t, "done", p, f"batches/{b}: {r[4]}"))
        meth, cls = re.match(r'\?([^@]+)@([^@]+)@@', p).groups()
        ren.append((f"0x{t.upper()}", cls, meth, p, CAT.get(t) or f"{cls}::{meth}", r[2],
                    f"RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names {p} here + PDB name set; flatten_pilot v5 (NF330) batches/{b} {r[4]}"))
        app.append((f"0x{t.upper()}", ent[t]["Source"], f"flatten v5: {r[2]} -> {p} ({r[4]})"))
        shutil.copy(f"{r[5]}/rows/{t}.cpp", f"{A}/rows/{t}.cpp"); continue
    if t in R:
        r = R[t]; st.append((t, "rejected" if r[1] == "REJECT" else "skipped-held", p, f"batches/{os.path.basename(r[5])} {r[1]}: {r[4]}")); continue
    st.append((t, "skipped-port", p, SK.get(t, "not reached")))
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
PS = {}
for l in open(f"{A}/evidence/probe_summary.tsv").read().splitlines():
    q = l.split("\t")
    if len(q) > 2 and q[2] in ("OK", "FAIL"): PS[q[0]] = l
EQ = open(f"{A}/evidence/equiv_check.tsv").read() if os.path.exists(f"{A}/evidence/equiv_check.tsv") else ""
CT = open(f"{A}/evidence/equiv_planted_control.tsv").read() if os.path.exists(f"{A}/evidence/equiv_planted_control.tsv") else ""
for b in sorted(glob.glob(f"{A}/batches/b*")):
    bn = os.path.basename(b); cls = BH.get(bn, [])
    for d in ("hdr/engine", "hdr/diff", "orig/rows"): os.makedirs(f"{b}/{d}", exist_ok=True)
    acc = [l.split("\t") for l in open(f"{b}/RESULTS.tsv").read().splitlines()[1:] if l.split("\t")[1] == "ACCEPT"]
    lines = []
    for q in acc:
        c = re.match(r'\?[^@]+@([^@]+)@@', q[3]).group(1)
        lines.append((f"engine/{c}.h", f"FABLE_RETAIL_{c.upper()}_VIRTUAL_MODEL", q[0]))
        hp = "rebuild/src/compiled/" + ent[q[0]]["Source"]
        if q[0] not in open(f"{b}/head/HEAD_FROM_REPO.txt").read().split(): shutil.copy(hp, f"{b}/orig/rows/{q[0]}.cpp")
    for c in cls:
        shutil.copy(f"{A}/hdr/engine/{c}.h", f"{b}/hdr/engine/{c}.h"); shutil.copy(f"{A}/orig/hdr_{c}.h", f"{b}/orig/hdr_{c}.h")
        o = open(f"{A}/orig/hdr_{c}.h").read().splitlines(True); n = open(f"{A}/hdr/engine/{c}.h").read().splitlines(True)
        open(f"{b}/hdr/diff/{c}.diff", "w").writelines(difflib.unified_diff(o, n, f"a/rebuild/include/engine/{c}.h", f"b/rebuild/include/engine/{c}.h"))
    by = collections.defaultdict(list)
    for h, m, a in lines: by[(h, m)].append(a)
    base_only = [c for c in cls if (f"engine/{c}.h", f"FABLE_RETAIL_{c.upper()}_VIRTUAL_MODEL") not in by]
    with open(f"{A}/HEADERS_{bn}.tsv", "w") as o:
        o.write("header\tmacro\trows\n" + "".join(f"{h}\t{m}\t{','.join(sorted(v))}\n" for (h, m), v in sorted(by.items()))
                + "".join(f"engine/{c}.h\tFABLE_RETAIL_{c.upper()}_RTTI_DECLS (base opt-in)\t(none; the batch's derived models need it)\n" for c in base_only))
    shutil.copy(f"{A}/HEADERS_{bn}.tsv", f"{b}/HEADERS.tsv")
    with open(f"{b}/PROBES.tsv", "w") as o:
        o.write("class\tvftable\tverdict\tslots\tnotes\n" + "".join(PS[c] + "\n" for c in cls if c in PS))
    open(f"{b}/EQUIV_CHECK.tsv", "w").write(EQ); open(f"{b}/EQUIV_CONTROL.tsv", "w").write(CT)
print(collections.Counter(x[1] for x in st))
