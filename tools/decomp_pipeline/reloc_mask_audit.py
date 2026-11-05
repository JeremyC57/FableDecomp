#!/usr/bin/env python3
"""reloc_mask_audit.py INDEX_DIR [--exe Fable.exe]

Finds landed rows that are reloc-masked false matches: the row's object has an absolute (dir32) relocation where retail has
a plain immediate, e.g. a stand-in vtable store `mov [ecx],offset ??_7T@@6B@` graded against retail's `mov [ecx],0`. The
grader masks relocation fields, so such a row grades RELOCATION_MATCH although its code is wrong.

Input is the symbol index (tools: $S/plan/symindex2.py): BIND.tsv records, for every dir32 relocation of a landed row, the
dword retail has at that position (`target`). A dir32 target outside the image (ImageBase .. ImageBase+SizeOfImage) means
retail stores a constant there, not an address.

Excluded as legitimate or as index artefacts:
  - `__except_list` (the absolute fs:[0] SEH symbol, value 0 by design);
  - rows whose indexed length differs from the catalog length (funclet rows indexed as their parent function, whose
    relocation offsets do not line up with the retail row).
Each remaining row must be checked by hand (compile it and read its relocations): 0x00A8E160 is listed but is clean (the
index took that reloc from another section of the object).

First run 2026-11-05: 0x009F1720 (CGameEventPackageSet::Clear) and 0x007E37C0 (CDefString::CDefString) were false matches and
were re-landed; batches/2026-11-05_relocmask_fix/README.md.
"""
import csv
import os
import struct
import sys

LEGIT_ABSOLUTE = {"__except_list"}


def image_range(exe):
    d = open(exe, "rb").read(4096)
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    base = struct.unpack_from("<I", d, pe + 0x34)[0]
    size = struct.unpack_from("<I", d, pe + 0x50)[0]
    return base, base + size


def main():
    idx = sys.argv[1]
    exe = sys.argv[sys.argv.index("--exe") + 1] if "--exe" in sys.argv else os.path.join(
        os.environ.get("S", ""), "dl/game/Fable The Lost Chapters/Fable.exe")
    lo, hi = image_range(exe)
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    cat = {r["address"].lower(): (r["name"], int(r["length"])) for r in
           csv.DictReader(open(os.path.join(root, "rebuild/oracles/auto-re-candidates.tsv")), delimiter="\t")}
    ilen = {}
    for line in open(os.path.join(idx, "INDEX.tsv")).read().splitlines()[1:]:
        q = line.split("\t")
        ilen[q[0]] = int(q[3] or 0)
    hits = {}
    for line in open(os.path.join(idx, "BIND.tsv")).read().splitlines()[1:]:
        caller, sym, target, kind = line.split("\t")
        if kind != "dir32" or sym in LEGIT_ABSOLUTE or lo <= int(target, 16) < hi:
            continue
        name, clen = cat.get(caller, ("?", -1))
        if ilen.get(caller) != clen:
            continue
        hits.setdefault(caller, []).append((sym, target))
    print("address\tcatalog_name\tsymbol\tretail_dword")
    for a in sorted(hits):
        for sym, t in hits[a]:
            print("%s\t%s\t%s\t%s" % (a, cat[a][0], sym, t))
    print("suspects: %d rows" % len(hits), file=sys.stderr)


if __name__ == "__main__":
    main()
