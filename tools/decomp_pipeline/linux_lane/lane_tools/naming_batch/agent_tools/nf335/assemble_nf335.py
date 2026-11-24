#!/usr/bin/env python3
"""assemble_nf335.py (NF335; repo root, A exported): hand-back tables from the triage (work/tri2/GEN.tsv), the final gen (work/g2/GEN.tsv)
and the exported batch(es) batches/b*: STATUS.tsv, APPLIED.tsv, RENAMES.tsv, REJECTED.tsv, TABLE_CHANGES.tsv, NAME_FIXES.tsv, rows/
(<addr>.cpp + <addr>_test.cpp), rows.txt. Reasons for the hand-checked skips come from work/skip_reasons.tsv (address<TAB>status<TAB>reason)."""
import os, sys, glob, shutil, re
sys.path.insert(0, "tools/decomp_pipeline"); sys.argv = ["x"]
import naming_status as ns
A = os.environ["A"]; ent = ns.landed_rows()
catnm = {q[0].lower(): q[1] for q in (l.split("\t", 2) for l in open("rebuild/oracles/auto-re-candidates.tsv", errors="replace")) if len(q) > 1}
def tsv(p): return [l.split("\t") for l in open(p).read().splitlines()[1:] if l.strip()]
tri = {r[0]: r for r in tsv(f"{A}/work/tri2/GEN.tsv")}
fin = {r[0]: r for r in tsv(f"{A}/work/g2/GEN.tsv")}
man = {r[0]: r for r in tsv(f"{A}/work/skip_reasons.tsv")} if os.path.exists(f"{A}/work/skip_reasons.tsv") else {}
acc = {}
for b in sorted(glob.glob(f"{A}/batches/b*")):
    for r in tsv(f"{b}/RESULTS.tsv"):
        if r[1] == "ACCEPT": acc[r[0]] = (os.path.basename(b), r)
st, ap, rn = [], [], []
for a, r in tri.items():
    P = r[3]
    if a in acc:
        b, x = acc[a]; st.append((a, "done", P, f"batches/{b}: {x[4]}"))
        f = ent[a]["Source"]; t = ent[a]["TestSource"]
        ap.append((("0x" + a.upper()), f, f"flatten v6 (NF335): {x[2]} -> {x[3]} ({x[4]}); test {t} moved with the row (NF324 Q2 option 1)"))
        m = re.match(r'\?(\w+)@(\w+)@@', P)
        rn.append(("0x" + a.upper(), m.group(2), m.group(1), P, catnm.get(a, ""), x[2],
                   "RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names " + P + " here + PDB name set; flatten_pilot v6 (NF335) " + b))
    elif a in man: st.append((a, man[a][1], P, man[a][2]))
    elif a in fin and fin[a][1] == "SKIP": st.append((a, "skipped-port", P, fin[a][5]))
    elif r[1] == "SKIP":
        why = r[5]
        if why.startswith("identity: another landed source defines"): st.append((a, "skipped-held", P, "unit: " + why))
        else: st.append((a, "skipped-port", P, why))
    else: st.append((a, "pending", P, "GEN in the triage, not graded"))
def w(name, head, rows):
    with open(f"{A}/{name}", "w") as o: o.write(head + "\n" + "".join("\t".join(x) + "\n" for x in rows))
w("STATUS.tsv", "address\tstatus\tpdb_spelling\treason", sorted(st))
w("APPLIED.tsv", "address\tfile\tchange", sorted(ap))
w("RENAMES.tsv", "address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence", sorted(rn))
w("REJECTED.tsv", "address\treason", [])
w("NAME_FIXES.tsv", "address\tcorrect_symbol\tevidence\twrong_spellings_seen", [])
w("TABLE_CHANGES.tsv", "action\tsymbol\ttarget\twitness\tnote", [])
os.makedirs(f"{A}/rows", exist_ok=True)
for a, (b, x) in acc.items():
    shutil.copy(f"{A}/batches/{b}/rows/{a}.cpp", f"{A}/rows/{a}.cpp"); shutil.copy(f"{A}/batches/{b}/tests/{a}_test.cpp", f"{A}/rows/{a}_test.cpp")
open(f"{A}/rows.txt", "w").write("".join(a + "\n" for a in sorted(acc)))
import collections; print(collections.Counter(s[1] for s in st))
