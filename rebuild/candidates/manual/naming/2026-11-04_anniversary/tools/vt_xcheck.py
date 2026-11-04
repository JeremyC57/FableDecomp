"""Cross-check Anniversary-derived class names for TLC RTTI-less vtables against the TLC catalog.
usage: vt_xcheck.py <work_dir> <functions.tsv> <match.tsv> <out.tsv>
"""
import sys, os, collections, subprocess, re
W, FT, M, OUT = sys.argv[1:5]
cat = {}
for l in open(FT, encoding="utf-8", errors="replace"):
    f = l.rstrip("\n").split("\t")
    if len(f) > 2 and f[0] != "address": cat[f[0].lower()] = (f[2], f[1])
vts = {}
for l in open(os.path.join(W, "tlc_vts.tsv")):
    va, ns, slots = l.rstrip("\n").split("\t"); vts[va] = slots.split(",") if slots else []
cnt = collections.Counter(s for v in vts.values() for s in set(v))
rows = [l.rstrip("\n").split("\t") for l in open(M)]
mangled = sorted(set(r[3] for r in rows))
vft = ["??_7" + m[4:] + "6B@" for m in mangled]
p = subprocess.run(["llvm-undname"], input="\n".join(vft) + "\n", capture_output=True, text=True)
lines = [x for x in p.stdout.split("\n")]
dem = {}
i = 0
for m in mangled:
    # output: mangled line, demangled line, blank
    while i < len(lines) and not lines[i].startswith("??_7"): i += 1
    d = lines[i + 1] if i + 1 < len(lines) else ""
    d = re.sub(r"^const ", "", d).replace("::`vftable'", "")
    d = re.sub(r"\b(class|struct|union|enum) ", "", d)
    dem[m] = d; i += 2
def norm(s): return s.replace(" ", "")
agree = disagree = unnamed = 0
with open(OUT, "w") as fo:
    fo.write("tlc_vtable\tnslots\tann_class\tcatalog_class(own slots)\tverdict\tevidence\n")
    for r in rows:
        va = r[0]; ann = dem[r[3]]
        own = [s for s in vts[va] if cnt[s] == 1]
        mods = collections.Counter(cat.get(s, ("?", ""))[0] for s in own)
        top = mods.most_common(1)[0][0] if mods else "?"
        s0 = cat.get(vts[va][0], ("?", ""))[0] if vts[va] else "?"
        if not (s0.startswith("CVtable_") or s0 in ("?", "_global", "_unattributed", "_gapscan")): top = s0
        if top.startswith("CVtable_") or top in ("?", "_global", "_unattributed", "_gapscan"):
            v = "NEW_NAME"; unnamed += 1
        elif norm(top) == norm(ann) or norm(ann).endswith("::" + norm(top)) or norm(top).endswith(norm(ann).split("::")[-1]):
            v = "AGREE"; agree += 1
        else:
            v = "DISAGREE"; disagree += 1
        fo.write("%s\t%s\t%s\t%s\t%s\tshared-strings=%s second=%s own-strings=%s ann_vt=%s\n" % (va, r[1], ann, top, v, r[4], r[5], r[6], r[2]))
print("agree", agree, "disagree", disagree, "catalog-placeholder (new name)", unnamed)
