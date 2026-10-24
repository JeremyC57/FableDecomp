#!/usr/bin/env python3
"""Apply evidenced label renames to landed catalog rows without changing what gets graded.

    python apply_names.py <renames.tsv> <labels_out.tsv> [--write]

renames.tsv: address, new_name, confidence, evidence (header row). For each landed row the catalog
name (rebuild/oracles/auto-re-candidates.tsv col 2) and the ps1 Module change; the source gets a
`// Leaf: <old selector>` line when it has none, because the graders fall back to the catalog name
to pick the object symbol (see lane_tools/recheck.py mangled_leaf) and the source still defines the
old symbol. Bytes, paths and tests are untouched; a later source-edit pass renames the symbols.
"""
import csv, re, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools/decomp_pipeline/linux_lane/lane_tools"))
CAT = ROOT / "rebuild/oracles/auto-re-candidates.tsv"; PS = ROOT / "rebuild/build_candidates.ps1"
OPS = {"operator=": "??4", "operator==": "??8", "operator!=": "??9", "operator[]": "??A", "operator()": "??R",
       "operator<": "??M", "operator new": "??2", "operator delete": "??3", "operator->": "??C"}


def mangled_leaf(leafname, mod):   # same rule as lane_tools/recheck.py
    cls = mod.rsplit("::", 1)[-1]
    def head(n): return ("?$" + n.split("<")[0]) if "<" in n else n
    if leafname.startswith("~"): return "??1" + head(leafname[1:]) + "@"
    if leafname == cls or (leafname.split("<")[0] == cls.split("<")[0] and mod != "_global"):
        return "??0" + head(cls) + "@"
    if leafname in OPS and mod != "_global": return OPS[leafname] + head(cls) + "@"
    return leafname


def main():
    src_tsv, out_tsv = sys.argv[1], sys.argv[2]; write = "--write" in sys.argv
    want = {r["address"].lower(): r for r in csv.DictReader(open(src_tsv, encoding="utf-8"), delimiter="\t")}
    lines = CAT.read_text(encoding="utf-8").split("\n")
    ps = PS.read_text(encoding="utf-8-sig")
    blocks = {m.group(2).lower(): m for m in re.finditer(r"(\[pscustomobject\]@\{\s*Address = '([0-9a-fA-F]+)'.*?\n\s*\})", ps, re.S)}
    done = pinned = 0; out = ["address\tname\tevidence"]; edits = []
    for i, l in enumerate(lines):
        q = l.split("\t"); a = q[0].lower()
        if a not in want or len(q) < 4: continue
        new = want[a]["new_name"]; old = q[1]
        if new == old or a not in blocks: continue
        blk = blocks[a].group(1); srcrel = re.search(r"Source = '([^']+)'", blk).group(1)
        sp = ROOT / "rebuild/src/compiled" / srcrel; text = sp.read_text(encoding="utf-8")
        if not re.search(r"^//\s*Leaf:\s*\S+", text, re.M):
            mod, leafname = old.rsplit("::", 1) if "::" in old else ("_global", old)
            first, _, rest = text.partition("\n")
            text = f"{first}\n// Leaf: {mangled_leaf(leafname, mod)}\n{rest}"; pinned += 1
            if write: sp.write_text(text, encoding="utf-8")
        q[1] = new; lines[i] = "\t".join(q)
        nmod = new.rsplit("::", 1)[0] if "::" in new else "_global"
        edits.append((blk, re.sub(r"Module = '[^']*'", f"Module = '{nmod}'", blk, count=1)))
        out.append(f"{a}\t{new}\t{want[a]['confidence']}: {want[a]['evidence']} (was {old})"); done += 1
    for o, n in edits: ps = ps.replace(o, n, 1)
    print(f"renamed {done}, Leaf lines pinned {pinned}")
    if write:
        CAT.write_text("\n".join(lines), encoding="utf-8"); PS.write_text(ps, encoding="utf-8")
        Path(out_tsv).write_text("\n".join(out) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
