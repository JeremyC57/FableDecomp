#!/usr/bin/env python3
"""Re-bound a catalog row whose retail span was cut wrong (truncated body, missing jump table,
or the next function captured in the row).

    python rebound_landed.py <addr> <new_length> "<evidence>" [--write]

The new bytes are read from the retail Fable.exe ($FABLE_EXE). The old catalog bytes must be a
prefix of the new span (extension) or the new span a prefix of the old one (trim), so a typo in
the address or length cannot replace a row with another function. Every change is appended to
rebuild/oracles/boundary-rebounds.tsv with its evidence. The row's source must then be
re-verified against the new span (`recheck.py` + `vland.py --recheck-landed`).
"""
import csv, datetime, os, struct, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
LEDGER = ROOT / "rebuild" / "oracles" / "boundary-rebounds.tsv"


def retail_bytes(exe, va, n):
    d = exe.read_bytes() if isinstance(exe, Path) else exe
    e = struct.unpack_from("<I", d, 0x3C)[0]; c = e + 4
    nsec = struct.unpack_from("<H", d, c + 2)[0]; opt = struct.unpack_from("<H", d, c + 16)[0]
    for i in range(nsec):
        s = c + 20 + opt + 40 * i
        vs, v, rs, rp = struct.unpack_from("<IIII", d, s + 8)
        rva = va - 0x400000
        if v <= rva < v + max(vs, rs):
            return d[rp + rva - v: rp + rva - v + n]
    raise SystemExit(f"{va:08x} is not in a section")


def main():
    if len(sys.argv) < 4:
        raise SystemExit(__doc__)
    addr = sys.argv[1].lower().replace("0x", "").zfill(8); new_len = int(sys.argv[2], 0); why = sys.argv[3]
    write = "--write" in sys.argv[4:]
    exe = Path(os.environ.get("FABLE_EXE", ""))
    if not exe.is_file():
        raise SystemExit("set FABLE_EXE to the retail Fable.exe")
    lines = CATALOG.read_text(encoding="utf-8").split("\n")
    idx = [i for i, l in enumerate(lines) if l.split("\t", 1)[0].lower() == addr]
    if len(idx) != 1:
        raise SystemExit(f"{addr}: {len(idx)} catalog rows")
    q = lines[idx[0]].split("\t"); old = bytes.fromhex(q[3])
    new = retail_bytes(exe, int(addr, 16), new_len)
    if not (new.startswith(old) or old.startswith(new)):
        raise SystemExit(f"{addr}: catalog bytes are not a prefix of the retail span (or the reverse): wrong address?")
    if len(old) == new_len:
        print(f"{addr}: already {new_len}"); return
    print(f"{addr} {q[1]}: {len(old)} -> {new_len} ({why})")
    if write:
        q[2] = str(new_len); q[3] = new.hex(); lines[idx[0]] = "\t".join(q)
        CATALOG.write_text("\n".join(lines), encoding="utf-8")
        fresh = not LEDGER.exists()
        with open(LEDGER, "a", encoding="utf-8", newline="") as f:
            if fresh: f.write("address\told_length\tnew_length\tdate\tevidence\n")
            f.write(f"{addr}\t{len(old)}\t{new_len}\t{datetime.date.today()}\t{why}\n")


if __name__ == "__main__":
    main()
