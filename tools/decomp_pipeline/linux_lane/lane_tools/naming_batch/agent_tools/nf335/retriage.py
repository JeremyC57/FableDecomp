#!/usr/bin/env python3
# retriage.py (NF329, 2026-11-24): rebuild NF324's Q1 list (TRIAGE bucket "no polymorphic class model (port)") against the CURRENT tree.
# Run from the repo root. Writes $A/work/RETRIAGE.tsv:
#   addr pdb_spelling class rtti_direct_base self_leaf_now state source note
# state: done-now (landed Leaf == PDB spelling) / open / lock:<owner> / reserved:<why> / nf326 / scriptthing
import os, re, sys, collections
sys.path.insert(0, "tools/decomp_pipeline")
import naming_status as ns
A = os.environ["A"]; S = os.environ["S"]
H = "rebuild/candidates/manual/pending/2026-11-24_nf324_handback"
tri = [l.rstrip("\n").split("\t") for l in open(H + "/TRIAGE.tsv")][1:]
q1 = [r[0] for r in tri if "no polymorphic class model" in r[5]]
st = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(H + "/STATUS.tsv")}
ent = ns.landed_rows()
bases = {}
for l in open("ghidra_out/rtti_bases.tsv"):
    p = l.rstrip("\n").split("\t")
    if len(p) > 3: bases[p[1]] = p[3].split(";")[0].split("@")[0]
# reservations
t3 = collections.defaultdict(list)
for l in list(open("rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv"))[1:]:
    r = l.rstrip("\n").split("\t")
    if len(r) > 2:
        t3[r[1].lower().replace("0x", "")].append(r[0])
        for x in re.findall(r"0x([0-9a-fA-F]{8})", r[2]): t3[x.lower()].append(r[0] + ":unit")
res = set()
import glob
for f in glob.glob("rebuild/candidates/manual/pending/*codex_pass2[678]*/RESERVED.tsv"):
    for l in list(open(f))[1:]:
        q = l.rstrip("\n").split("\t")
        if q and q[0] and (len(q) < 4 or "no longer reserved" not in q[3]): res.add(q[0].lower().replace("0x", ""))
B = set(x.lower().replace("0x", "") for x in "0x00E20600 0x00D09010 0x00E93EB0 0x00D01630 0x00975BB0 0x00D11930 0x00F1EED0 0x00D0C410 0x00839940 0x00D0D910 0x00DC93F0 0x00DB3FA0 0x012133C0 0x00407A70 0x00407BA0 0x00402510 0x00BE4FE0 0x0070D940 0x00BE3970 0x00BE4660".split())
nf326 = set()
p = "rebuild/candidates/manual/pending/2026-11-24_nf326_handback/rows.txt"
if os.path.exists(p): nf326 = {l.strip().lower() for l in open(p) if l.strip()}
def owner(a):
    try: return open(f"{S}/w22/nflocks/{a}/owner").read().strip()
    except OSError: return ""
out = []
for a in q1:
    pdb = st.get(a, ["", "", ""])[2]
    m = re.match(r"\?[^@]+@(\w+)@@", pdb); cls = m.group(1) if m else ""
    e = ent.get(a, {}); src = e.get("Source", "")
    leaf = ns.leaf_of(src) if src else ""
    text = open("rebuild/src/compiled/" + src, errors="replace").read() if src else ""
    note = []
    if "CScriptThing" in text and ("~CScriptThing" in text or "CScriptThing(const CScriptThing" in text): note.append("scriptthing-text")
    o = owner(a)
    if not src: state = "no-landed-row"
    elif leaf == pdb: state = "done-now"
    elif a in nf326: state = "nf326"
    elif a in B: state = "acctB"
    elif a in res: state = "reserved:codex"
    elif o and o != "released" and o != "NF329": state = "lock:" + o
    else: state = "open"
    other_t3 = [x for x in t3.get(a, []) if not x.startswith("nf324")]
    if other_t3: note.append("t3:" + ",".join(other_t3))
    out.append([a, pdb, cls, bases.get(cls, "?"), leaf, state, src, " ".join(note)])
with open(A + "/work/RETRIAGE.tsv", "w") as f:
    f.write("addr\tpdb_spelling\tclass\tbase\tself_leaf_now\tstate\tsource\tnote\n")
    for r in out: f.write("\t".join(r) + "\n")
c = collections.Counter(r[5].split(":")[0] for r in out); print(dict(c))
c2 = collections.Counter((r[3], r[5].split(":")[0]) for r in out)
for k, v in sorted(c2.items(), key=lambda x: -x[1])[:30]: print(v, k)
