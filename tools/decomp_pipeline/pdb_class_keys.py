#!/usr/bin/env python3
"""Retail class-keys from the PDB name sets -> ghidra_out/pdb_class_keys.tsv.

MSVC mangles a type reference with its class-key (`V` class, `U` struct, `T` union, `W4` enum), and
VC7.1 takes it from the type's definition (else its first declaration). This table records, for every
user-defined type the Ego_r / FableWin / Ego_d name sets reference, which key(s) retail used and how
often, so the reconstruction can declare each type with retail's key (gen_class_headers.py reads it).

    python tools/decomp_pipeline/pdb_class_keys.py            # needs llvm-undname (or FABLE_UNDNAME)

Columns: type, kind (type | template = a class template's base name), keys, per-PDB V/U counts,
union/enum counts, flag BOTH when one name is keyed both ways, one example mangled name.
"""
import os, subprocess, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
import re, sys, collections
UNDNAME = os.environ.get("FABLE_UNDNAME", "llvm-undname")
wd = tempfile.mkdtemp(prefix="pdbkeys")
for _p in ("egor", "fablewin", "egod"):
    _src = ROOT / "ghidra_out" / f"{_p}_pdb_names.tsv"
    if not _src.exists(): continue
    _names = [l.split("\t")[1] for l in _src.read_text(encoding="utf-8", errors="replace").splitlines() if "\t?" in l]
    _r = subprocess.run([UNDNAME], input="\n".join(_names) + "\n", capture_output=True, text=True, errors="replace")
    Path(wd, f"{_p}.dem").write_text(_r.stdout, encoding="utf-8")
out = sys.argv[1] if len(sys.argv) > 1 else str(ROOT / "ghidra_out" / "pdb_class_keys.tsv")
KW = {"class": "V", "struct": "U", "union": "T", "enum": "W4"}
kre = re.compile(r"(?<![A-Za-z0-9_])(class|struct|union|enum) (?=[A-Za-z_`])")

def read_name(s, i):
    """read a qualified type name starting at i; returns (name, end)."""
    depth = 0; j = i
    while j < len(s):
        c = s[j]
        if c == "<": depth += 1
        elif c == ">":
            if depth == 0: break
            depth -= 1
        elif c == "`":           # `anonymous namespace' etc.
            k = s.find("'", j)
            if k < 0: break
            j = k
        elif depth == 0 and c in " ,)*&(":
            break
        j += 1
    return s[i:j], j

cnt = collections.defaultdict(lambda: collections.Counter())   # (name) -> Counter((pdb,key))
tmpl = collections.defaultdict(lambda: collections.Counter())  # template base name
ex = {}
for pdb in ("egor", "fablewin", "egod"):
    try: lines = open(f"{wd}/{pdb}.dem", encoding="utf-8", errors="replace").read().split("\n")
    except FileNotFoundError: continue
    i = 0
    while i < len(lines) - 1:
        m, d = lines[i], lines[i + 1]
        if not m.startswith("?"): i += 1; continue
        i += 2
        if d.startswith("?") or "invalid" in d.lower(): i -= 1; continue
        for mm in kre.finditer(d):
            name, _ = read_name(d, mm.end())
            if not name: continue
            key = KW[mm.group(1)]
            cnt[name][(pdb, key)] += 1
            if "<" in name:
                tmpl[name.split("<")[0]][(pdb, key)] += 1
            ex.setdefault((name, key), m)

def row(name, c, kind):
    keys = sorted({k for (_, k) in c})
    f = lambda p, k: c.get((p, k), 0)
    return [name, kind, "/".join(keys),
            str(f("egor", "V")), str(f("egor", "U")), str(f("fablewin", "V")), str(f("fablewin", "U")),
            str(sum(v for (p, k), v in c.items() if k == "T")), str(sum(v for (p, k), v in c.items() if k == "W4")),
            "BOTH" if len({k for (_, k) in c if k in "UV"}) > 1 else "",
            ex.get((name, keys[0]), "")]
hdr = ["type", "kind", "keys", "egor_V", "egor_U", "fablewin_V", "fablewin_U", "union_T", "enum_W4", "flag", "example"]
rows = [row(n, c, "type") for n, c in cnt.items() if "<" not in n]
rows += [row(n, c, "template") for n, c in tmpl.items()]
rows.sort(key=lambda r: (r[1], r[0]))
with open(out, "w") as f:
    f.write("\t".join(hdr) + "\n")
    for r in rows: f.write("\t".join(r) + "\n")
print(len(rows), "rows;", sum(1 for r in rows if r[9]), "keyed both ways")
