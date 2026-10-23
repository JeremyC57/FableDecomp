#!/usr/bin/env python3
"""Relocation-conflict cleanup: make every mangled symbol bind ONE retail address catalog-wide.

A landed row RELOCATION_MATCHes with its relocations masked, so two rows can each declare
`extern "C" void ForwardTarget();` and call two different retail functions through it. Every
row still byte-matches, but a relink would resolve both calls to one definition. This tool
renames the source identifier (never the bytes) in the rows that bind a non-canonical target.

    reloc_rename.py scan  --addrs F --out scan.jsonl [--src-root DIR] [--work DIR]
    reloc_rename.py plan  --scan scan.jsonl[,more] --conflicts conflicts.json --out plan.json
    reloc_rename.py apply --plan plan.json --out-root DIR --manifest M.tsv --failed F.tsv [--only syms]
    reloc_rename.py check --scan scan.jsonl[,more]          (global symbol->target consistency)

Policy (canonical owner of a symbol X):
  * a landed row AT one of X's targets whose own defining symbol is X owns X;
  * otherwise the target the most rows bind X to owns it (ties: lowest address);
  * every other target gets a unique name: free function/global `X` -> `X_<target8hex>`, a member
    of a model struct the same; constructors/destructors/operators cannot be renamed, so their
    model class is moved into a file-local `namespace N_<target>` (only when the row's own leaf
    is not a member of that class).
Each patched file is recompiled with the row's catalog flags/compiler; it must stay MATCH or
RELOCATION_MATCH, the renamed symbol must bind the intended target, every other binding of the
row must be unchanged, and reloc_identity.check() must report no in-row hard error.
"""
import csv, json, os, re, sys, struct, subprocess
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import verify_and_land as V, reloc_identity as RI

ROOT = V.ROOT
COMPILED = ROOT / "rebuild" / "src" / "compiled"

# ---------------------------------------------------------------- catalog
_ENTS = None; _ORC = None
def catalog():
    global _ENTS
    if _ENTS is None:
        text = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8-sig"); _ENTS = {}
        for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
            d = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", m.group(1)))
            if "Address" in d: _ENTS[d["Address"].lower()] = d
    return _ENTS

def oracle():
    global _ORC
    if _ORC is None:
        _ORC = {r["address"].lower(): r for r in csv.DictReader(open(ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    return _ORC

def leaf_of(src, addr):
    m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
    return m.group(1) if m else oracle()[addr]["name"].rsplit("::", 1)[-1]

def flags_of(addr):
    d = catalog()[addr]
    return [f for f in d.get("CompilerFlags", "").split() if f not in V.BASE_FLAGS], d.get("Compiler", "")

# ---------------------------------------------------------------- compile + bindings
def function_block(objpath, leaf, length):
    """(block symbol, [(offset, kind, symbol)]) like RI.function_relocs, keeping the block name."""
    out = subprocess.run([V.OBJDUMP, "-dr", "-w", str(objpath)], capture_output=True, text=True).stdout
    best = None
    for bl in re.split(r"\n(?=[0-9a-fA-F]{8} <)", out):
        m = re.match(r"([0-9a-fA-F]{8}) <([^>]+)>:", bl)
        if not m: continue
        start = int(m.group(1), 16)
        insn = re.findall(r"^\s+([0-9a-fA-F]+):\t((?:[0-9a-fA-F]{2} )+)", bl, re.M)
        if not insn: continue
        last = insn[-1]; ln = int(last[0], 16) + len(last[1].split()) - start
        if ln != length: continue
        rel = [(int(x, 16) - start, k, s) for x, k, s in re.findall(r"\t([0-9a-fA-F]+): (DISP32|dir32|DIR32|REL32)\t(\S+)", bl)]
        if best is None or (leaf and leaf in m.group(2) and leaf not in best[0]):
            best = (m.group(2), rel)
    return best

def symtab(objpath):
    """{symbol: 'static'|'external'} for symbols DEFINED in the object (section != UNDEF)."""
    out = subprocess.run([V.OBJDUMP, "-t", str(objpath)], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines():
        m = re.match(r"\[\s*\d+\]\(sec\s+(-?\d+)\)\(fl 0x\w+\)\(ty\s+\w+\)\(scl\s+(\d+)\) \(nx \d+\) 0x\w+ (\S+)", l)
        if m and int(m.group(1)) > 0:
            d[m.group(3)] = "static" if m.group(2) in ("3",) else "external"
    return d

def compile_row(addr, src, work, e):
    """Compile `src` as row `addr` -> dict(st, leafsym, pairs, hard, defined)."""
    extra, comp = flags_of(addr)
    if comp == "qfe4035": V.use_qfe()
    else: V.use_default()
    retail = bytes.fromhex(oracle()[addr]["bytes"])
    leaf = leaf_of(src, addr)
    work.mkdir(parents=True, exist_ok=True)
    try:
        st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
    except Exception as ex:
        return {"addr": addr, "st": f"ERROR {ex}"}
    r = {"addr": addr, "st": st, "leaf": leaf}
    if not st.startswith(("MATCH", "RELOCATION")): return r
    obj = work / f"{addr}.obj"
    try:
        hard, warn, pairs = RI.check(ROOT, V.OBJDUMP, obj, leaf, addr, retail, built=built)
    except Exception as ex:
        r["st"] += f" CHECKERR {ex}"; return r
    blk = function_block(obj, leaf, len(retail))
    r["leafsym"] = blk[0] if blk else None
    r["pairs"] = sorted({(p[2], f"{p[3]:08x}") for p in pairs})
    r["hard"] = hard
    st_ = symtab(obj)
    r["static"] = sorted(s for s, k in st_.items() if k == "static")
    r["defined"] = sorted(s for s, k in st_.items() if k == "external")
    return r

def cmd_scan(args):
    addrs = [l.split()[0].lower() for l in open(args["--addrs"]) if l.strip()]
    out = Path(args["--out"]); src_root = Path(args.get("--src-root", COMPILED))
    work = Path(args.get("--work", Path(os.environ["S"]) / "relink" / "work" / "scan"))
    done = set()
    if out.exists():
        for l in open(out): done.add(json.loads(l)["addr"])
    e = V.env()
    with open(out, "a") as f:
        for a in addrs:
            if a in done or a not in catalog() or a not in oracle(): continue
            if catalog()[a].get("Grade") == "asm_bake": continue
            p = src_root / catalog()[a]["Source"]
            if not p.exists(): continue
            r = compile_row(a, p.read_text(encoding="utf-8", errors="replace"), work / a[:4] / a, e)
            f.write(json.dumps(r) + "\n"); f.flush()

def load_scan(paths):
    rows = {}
    for p in paths.split(","):
        for l in open(p):
            r = json.loads(l); rows[r["addr"]] = r
    return rows

def parse_args(argv):
    d = {}; i = 0
    while i < len(argv):
        if argv[i].startswith("--"):
            if i + 1 < len(argv) and not argv[i + 1].startswith("--"): d[argv[i]] = argv[i + 1]; i += 2
            else: d[argv[i]] = True; i += 1
        else: d.setdefault("_", []).append(argv[i]); i += 1
    return d

if __name__ == "__main__":
    cmd = sys.argv[1]; args = parse_args(sys.argv[2:])
    {"scan": cmd_scan}[cmd](args)
