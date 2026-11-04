"""Assemble candidate_names.tsv and vt_confirmations.tsv.
usage: make_candidates.py <work_dir> <functions.tsv> <out_candidates> <out_confirm>
"""
import sys, os
W, FT, OUT, CONF = sys.argv[1:5]
cat = {}
for l in open(FT, encoding="utf-8", errors="replace"):
    f = l.rstrip("\n").split("\t")
    if len(f) > 2 and f[0] != "address": cat[f[0].lower()] = "%s::%s" % (f[2], f[1])
slots = {}
for l in open(os.path.join(W, "tlc_vts.tsv")):
    va, n, s = l.rstrip("\n").split("\t"); slots[va] = s.split(",") if s else []
REJECT = {"012d6e5c"}  # NUISystem::CManager: slots are CSuspendableProcessMicrothread; string overlap is a false positive
def rows(fn):
    for l in list(open(os.path.join(W, fn)))[1:]:
        yield l.rstrip("\n").split("\t")
seen = set(); out = []; conf = []
def add(r, method, confidence):
    va, ns, ann, catc, verdict, ev = r[:6]
    s0 = slots.get(va, ["?"])[0]
    if verdict == "AGREE":
        conf.append((va, ann, catc, method, ev)); return
    if va in seen: return
    seen.add(va)
    c = "REJECT (false positive on review)" if va in REJECT else confidence
    out.append((ann, "TLC vtable 0x%s (%s slots) <-> Anniversary RTTI %s; %s; %s" % (va.upper(), ns, ev.split("ann_vt=")[-1] if "ann_vt=" in ev else "", method, ev),
                "vtable 0x%s; slot0 0x%s now '%s'; catalog class '%s' (%s)" % (va.upper(), s0.upper(), cat.get(s0, "?"), catc, verdict), c))
for r in rows("vt_xcheck.tsv"): add(r, "string-literal match of own slot functions (validation 257/258)", "high")
for r in rows("vt_xcheck_gapstrict.tsv"): add(r, "strict order alignment between anchors (validation 79/79; catalog agreement 111/111)", "high")
for r in rows("vt_xcheck_gap.tsv"): add(r, "loose order alignment between anchors (validation 201/204; catalog agreement 351/395)", "medium" if r[4] == "NEW_NAME" else "low (disagrees with catalog)")
# Anniversary-only fable-ish RTTI classes
extra = []
for l in open(os.path.join(W, "rtti_classified.tsv")):
    cat_, n, m, rv = l.rstrip("\n").split("\t")
    if cat_ == "new_fableish":
        extra.append((n, "Anniversary RTTI %s; absent from fablewin/egor PDB names and Ego_r struct layouts" % m, "none found in TLC (Anniversary port layer or new UI)", "n/a"))
with open(OUT, "w") as f:
    f.write("name\tsource_evidence\tlikely_tlc_match\tconfidence\n")
    order = {"high": 0, "medium": 1}
    for r in sorted(out, key=lambda r: (order.get(r[3], 2), r[0])): f.write("\t".join(r) + "\n")
    for r in extra: f.write("\t".join(r) + "\n")
with open(CONF, "w") as f:
    f.write("tlc_vtable\tanniversary_rtti_class\tcatalog_class\tmethod\tevidence\n")
    seenc = set()
    for r in conf:
        if r[0] in seenc: continue
        seenc.add(r[0]); f.write("\t".join(r) + "\n")
print("candidates", len(out), "extra", len(extra), "confirmations", len(seenc))
import collections
print(collections.Counter(r[3] for r in out))
