#!/usr/bin/env python3
"""pdb_module_tables.py <Ego_r|FableWin> <dir with <name>.symbols.txt / .publics.txt / .sections.txt> <out_prefix>

Builds two lookup tables from a donor PDB's LIVE module streams (Ego_r.pdb, FableWin.pdb: both intact, unlike Ego_d's carved ones):
  <out_prefix>_obj_functions.tsv   every S_GPROC32 / S_LPROC32 / S_THUNK32 in module order:
                                   object, index in object, va, code size, kind, PDB proc name, decorated public at that va
                                   (the same-TU / object-order evidence ghidra_out/egod_carved_functions.tsv gives for Ego_d,
                                   but complete)
  <out_prefix>_publics_noproc.tsv  function-flagged S_PUB32 at addresses with no proc record (no debug info: CRT / library
                                   objects, compiler-generated ??_G / ??_E deleting destructors, import thunks); the Ghidra
                                   name and signature tables leave these as FUN_<va>.
Inputs are llvm-pdbutil text dumps. Both PDBs need repacking to 4 KB blocks first (llvm-pdbutil: "Too many directory blocks"):
  python3 tools/decomp_pipeline/linux_lane/lane_tools/anniv/repack4k.py X.pdb X.4k.pdb
  llvm-pdbutil dump -symbols X.4k.pdb > X.symbols.txt; llvm-pdbutil dump -publics X.4k.pdb > X.publics.txt
  llvm-pdbutil dump -section-headers X.4k.pdb > X.sections.txt
Donor addresses (Ego_r.exe / FableWin.exe), never retail addresses. Ego_r and FableWin are VC10 builds (backend 10.0.40219).
"""
import collections
import re
import sys

name, d, out = sys.argv[1:4]
IMAGE_BASE = 0x400000

sec = {}
cur = None
for l in open(f"{d}/{name}.sections.txt", errors="replace"):
    m = re.search(r"SECTION HEADER #(\d+)", l)
    if m:
        cur = int(m[1])
        continue
    m = re.search(r"^\s*([0-9A-F]+) virtual address", l)
    if m and cur:
        sec[cur] = int(m[1], 16)


def va(s, o):
    s = int(s)
    return IMAGE_BASE + sec[s] + int(o) if s in sec else None


pub = collections.defaultdict(list)
fpub = {}
pname = None
for l in open(f"{d}/{name}.publics.txt", errors="replace"):
    m = re.search(r"S_PUB32 \[size = \d+\] `(.*)`$", l)
    if m:
        pname = m[1]
        continue
    m = re.search(r"flags = (.*), addr = (\d+):(\d+)", l)
    if m and pname is not None:
        a = va(m[2], m[3])
        if a is not None:
            pub[a].append(pname)
            if "function" in m[1]:
                fpub.setdefault(a, pname)
        pname = None

rows = []
obj = None
idx = 0
kind = pn = None
rx_mod = re.compile(r"^\s*Mod (\d+) \| `(.*)`:")
rx_sym = re.compile(r"\| (S_[GL]PROC32|S_THUNK32) \[size = \d+\] `(.*)`$")
for l in open(f"{d}/{name}.symbols.txt", errors="replace"):
    m = rx_mod.match(l)
    if m:
        obj, idx = m[2], 0
        continue
    m = rx_sym.search(l)
    if m:
        kind, pn = m.groups()
        continue
    if kind:
        m = re.search(r"addr = (\d+):(\d+), (?:code size|length) = (\d+)", l)
        if m:
            a = va(m[1], m[2])
            idx += 1
            dec = next((p for p in pub.get(a, []) if p.startswith(("?", "_", "@"))), "") if a is not None else ""
            rows.append((obj, idx, a, int(m[3]), kind, pn, dec))
            kind = None

with open(f"{out}_obj_functions.tsv", "w") as f:
    f.write(f"# {name}.pdb live module streams: object\tindex_in_object\tva\tcode_size\tkind\tproc_name\tdecorated_public\n")
    for o, i, a, s, k, n, p in rows:
        f.write(f"{o}\t{i}\t{a:08x}\t{s}\t{k}\t{n}\t{p}\n" if a is not None else f"{o}\t{i}\t-\t{s}\t{k}\t{n}\t{p}\n")
procs = {r[2] for r in rows}
noproc = sorted((a, n) for a, n in fpub.items() if a not in procs)
with open(f"{out}_publics_noproc.tsv", "w") as f:
    f.write(f"# {name}.pdb function publics with no proc record (no debug info): va\tdecorated_public\n")
    for a, n in noproc:
        f.write(f"{a:08x}\t{n}\n")
print(f"{name}: {len(rows)} procs in {len({r[0] for r in rows})} objects; {len(noproc)} public-only functions")
