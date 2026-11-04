"""Build class-name sets and compare Anniversary RTTI to TLC PDB name sets.
usage: classes.py <work_dir> <repo_dir>
needs: work/rtti_demangled.tsv (mangled \t demangled), work/pdb_demangled_*.txt
"""
import sys, re, os, collections
W, R = sys.argv[1], sys.argv[2]

def strip_cls(s):
    s = s.strip()
    s = re.sub(r"^const ", "", s)
    s = s.replace("::`vftable'", "")
    s = re.sub(r"\b(class|struct|union|enum) ", "", s)
    return s.replace(" ", "")

ann = {}
for line in open(os.path.join(W, "rtti_demangled.tsv"), encoding="latin-1"):
    m, d = line.rstrip("\n").split("\t")[:2]
    ann[strip_cls(d)] = m

# TLC classes: scopes from demangled PDB function names + struct_layouts names
def scopes_from_demangled(path):
    out = set()
    for line in open(path, encoding="latin-1"):
        line = line.strip()
        if not line or line.startswith("?"): continue
        # remove return type/calling conv: find the qualified name before '('
        p = line.find("(")
        head = line if p < 0 else line[:p]
        # remove leading access/static/virtual/type words: take last token that contains ::
        # handle templates: walk with depth
        depth = 0; start = 0
        for i, ch in enumerate(head):
            if ch == '<': depth += 1
            elif ch == '>': depth -= 1
            elif ch == ' ' and depth == 0: start = i + 1
        q = head[start:]
        # split on :: at depth 0
        parts = []; depth = 0; cur = ""
        i = 0
        while i < len(q):
            ch = q[i]
            if ch == '<': depth += 1
            elif ch == '>': depth -= 1
            if depth == 0 and q.startswith("::", i):
                parts.append(cur); cur = ""; i += 2; continue
            cur += ch; i += 1
        parts.append(cur)
        for k in range(1, len(parts)):
            out.add(re.sub(r"\b(class|struct|union|enum) ", "", "::".join(parts[:k])).replace(" ", ""))
    return out

tlc = set()
for f in ("pdb_demangled_fablewin.txt", "pdb_demangled_egor.txt"):
    tlc |= scopes_from_demangled(os.path.join(W, f))
structs = set()
for line in open(os.path.join(R, "ghidra_out/struct_layouts_egor.tsv"), encoding="latin-1"):
    if line.startswith("@STRUCT\t"):
        structs.add(line.split("\t")[1].replace("_", " ").replace(" ", "") if "<" in line.split("\t")[1] else line.split("\t")[1])
tlc_all = tlc | structs
rv = set()
for line in open(os.path.join(R, "rebuild/manifest/rtti-vtables.tsv"), encoding="latin-1"):
    c = line.split("\t")[0]
    if c != "class": rv.add(c)

def is_unreal(n):
    top = n.split("::")[0].split("<")[0]
    if re.match(r"^[UAFT][A-Z]", top): return True
    if top.startswith(("Scaleform", "AK", "physx", "Nx", "Apex", "wx", "nv", "Gfx", "Steam", "IUnknown")): return True
    return False

def is_std(n):
    return n.startswith(("std::", "stdext::", "type_info", "Concurrency", "<lambda", "`"))

cats = collections.Counter()
rows = []
for n, m in sorted(ann.items()):
    if is_std(n): cat = "std"
    elif is_unreal(n): cat = "unreal_or_3rdparty"
    elif n in tlc_all: cat = "in_tlc_pdb"
    else:
        # try ignoring namespace prefixes
        base = n.split("::")[-1]
        cat = "new_fableish" if re.match(r"^(C|N|I|S|E)[A-Z]", n.split("::")[0]) else "other"
    cats[cat] += 1
    rows.append((cat, n, m, "Y" if n in rv else ""))
print(cats)
print("TLC class/scope set size", len(tlc), "structs", len(structs), "rtti-vtables classes", len(rv))
with open(os.path.join(W, "rtti_classified.tsv"), "w") as f:
    for r in rows: f.write("\t".join(r) + "\n")
fable = [r for r in rows if r[0] in ("in_tlc_pdb", "new_fableish")]
print("fable-ish total", len(fable), "in rtti-vtables.tsv", sum(1 for r in fable if r[3]))
