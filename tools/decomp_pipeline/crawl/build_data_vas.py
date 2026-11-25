#!/usr/bin/env python3
"""build_data_vas.py <Fable.exe> [--out rebuild/oracles/data_vas.tsv]

Builds the committed key list for the `baked_address` grade (advisor decision, owner delegated, 2026-11-25): retail VAs a row may
reference only through a symbol -- vftables, .data/.bss globals, IAT slots -- each verified against the retail image:

  vftable  RTTI: the dword before the VA points to a CompleteObjectLocator (.rdata, signature 0) whose type descriptor (.data) names
           a `.?AV`/`.?AU` class. Every RTTI vftable in the image is listed.
  bound    rebuild/oracles/reloc_bindings.tsv binds a non-function symbol to a .rdata/.data/.idata VA (not a string literal in
           string_vas.tsv, not a function entry) and the retail bytes of the witness's catalog function (the function containing
           the witness address, per auto-re-candidates.tsv) hold the VA as a little-endian dword. Catches vftables without RTTI (libpk), globals, float pool entries, IAT slots by name.
  iat      a slot of the import address table (import directory of the image).
  manual   NF415 HOLD_ROWS.tsv verdict HOLD lines (per-row manual proof against the retail bytes, 2026-11-25) not covered above.
  code     .text addresses a landed row types in (CODE_ROWS below, from the 2026-11-25 .text rescan: 74 rows hold a hex token in
           the .text range, 71 are flag / size constants or unused macro arguments), each verified: the row's retail bytes hold
           the VA as a little-endian dword.

String literals stay in string_vas.tsv (baked_string). Function entries are keyed only through CODE_ROWS: a blanket .text key
would flag every `flags & 0x800000` (0x00800000 is a function entry). Audit: rebuild/candidates/manual/audits/2026-11-25_baked_address/.
Only VAs are written (no retail bytes).
"""
import argparse
import bisect
import os
import struct
import sys
from pathlib import Path

ROOT = Path(os.environ.get("FABLE_ROOT") or Path(__file__).resolve().parents[3])
BASE = 0x400000
# (row, VA) pairs from the 2026-11-25 .text rescan: a function pointer passed as an integer (0x00864600, 0x00644627) and a catch
# funclet's resume address returned as a constant (0x009F5DE9).
CODE_ROWS = [("00864600", 0x00643B3B), ("00644627", 0x0041BE70), ("009f5de9", 0x009F5DEF)]


class Image:
    def __init__(self, path):
        b = Path(path).read_bytes()
        pe = struct.unpack_from("<I", b, 0x3C)[0]
        n = struct.unpack_from("<H", b, pe + 6)[0]
        opt = struct.unpack_from("<H", b, pe + 20)[0]
        self.b = b
        self.secs = []
        o = pe + 24 + opt
        for i in range(n):
            nm, vs, va, rs, ro = struct.unpack_from("<8sIIII", b, o + 40 * i)
            self.secs.append((nm.rstrip(b"\0").decode(), BASE + va, BASE + va + max(vs, rs), ro, rs))
        self.imp_rva, self.imp_size = struct.unpack_from("<II", b, pe + 24 + 96 + 8)   # data directory 1: import table

    def sec(self, va):
        for nm, lo, hi, _ro, _rs in self.secs:
            if lo <= va < hi:
                return nm
        return None

    def read(self, va, n):
        for _nm, lo, hi, ro, rs in self.secs:
            if lo <= va and va + n <= hi:
                off = va - lo
                if off + n > rs:
                    return b"\0" * n          # .bss tail: zero-filled
                return self.b[ro + off: ro + off + n]
        return None

    def dword(self, va):
        r = self.read(va, 4)
        return struct.unpack("<I", r)[0] if r else None

    def cstr(self, va, limit=512):
        r = self.read(va, limit) or b""
        return r.split(b"\0", 1)[0]


def rtti_vftables(img):
    out = {}
    _nm, lo, hi, _ro, _rs = next(s for s in img.secs if s[0] == ".rdata")
    for va in range(lo + 4, hi - 4, 4):
        col = img.dword(va - 4)
        if col is None or img.sec(col) != ".rdata" or img.dword(col) != 0:
            continue
        td = img.dword(col + 12)
        if td is None or img.sec(td) != ".data":
            continue
        name = img.cstr(td + 8, 256)
        if name.startswith((b".?AV", b".?AU")) and img.sec(img.dword(va) or 0) == ".text":
            out[va] = "RTTI " + name.decode("latin-1")
    return out


def iat_slots(img):
    out = {}
    d = BASE + img.imp_rva
    while True:
        oft, _ts, _fc, name, ft = struct.unpack("<IIIII", img.read(d, 20))
        if not (oft or name or ft):
            break
        dll = img.cstr(BASE + name).decode("latin-1")
        k = 0
        while True:
            thunk = img.dword(BASE + (oft or ft) + 4 * k)
            if not thunk:
                break
            sym = "#%d" % (thunk & 0xFFFF) if thunk & 0x80000000 else img.cstr(BASE + thunk + 2).decode("latin-1")
            out[BASE + ft + 4 * k] = "IAT %s!%s" % (dll, sym)
            k += 1
        d += 20
    return out


def tsv(p):
    lines = Path(p).read_text(encoding="utf-8").splitlines()
    return [l.split("\t") for l in lines[1:] if l.strip()]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("--out", default=str(ROOT / "rebuild" / "oracles" / "data_vas.tsv"))
    a = ap.parse_args()
    img = Image(a.exe)
    strings = {int(f[0], 16) for f in tsv(ROOT / "rebuild" / "oracles" / "string_vas.tsv")}
    cat = {int(f[0], 16): int(f[2]) for f in tsv(ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv")
           if len(f) > 2 and f[2].isdigit()}
    funcs = set(cat)
    starts = sorted(cat)

    def body(w):
        i = bisect.bisect_right(starts, w) - 1
        if i < 0 or w >= starts[i] + cat[starts[i]]:
            return b""
        return img.read(starts[i], cat[starts[i]]) or b""
    keys = {}

    def add(va, kind, ev):
        if va in strings or va in funcs or img.sec(va) not in (".rdata", ".data", ".idata"):
            return False
        keys.setdefault(va, (kind, ev))
        return True

    for va, ev in rtti_vftables(img).items():
        add(va, "vftable", ev)
    nrtti = len(keys)
    for va, ev in iat_slots(img).items():
        add(va, "iat", ev)
    niat = len(keys) - nrtti
    bad = 0
    for f in tsv(ROOT / "rebuild" / "oracles" / "reloc_bindings.tsv"):
        if len(f) < 3 or f[0].startswith(("??_C@", "$SG")):
            continue
        va, w = int(f[1], 16), int(f[2], 16)
        if va in keys or img.sec(va) not in (".rdata", ".data", ".idata"):
            continue
        if struct.pack("<I", va) not in body(w):
            bad += 1
            continue
        add(va, "bound", "%s (witness %08x)" % (f[0], w))
    nbound = len(keys) - nrtti - niat
    hold = ROOT / "rebuild" / "candidates" / "manual" / "pending" / "2026-11-25_nf415_handback" / "HOLD_ROWS.tsv"
    for f in tsv(hold):
        if len(f) > 6 and f[6] == "HOLD":
            va = int(f[2], 16)
            if va not in keys:
                add(va, "manual", "NF415 HOLD_ROWS %s: %s" % (f[0], f[4]))
    for row, va in CODE_ROWS:
        if struct.pack("<I", va) not in body(int(row, 16)):
            raise SystemExit("build_data_vas: retail bytes of %s do not hold 0x%08X" % (row, va))
        keys.setdefault(va, ("code", "row %s retail bytes hold the VA (.text rescan 2026-11-25)" % row))
    with open(a.out, "w", encoding="utf-8", newline="\n") as o:
        o.write("va\tsection\tkind\tevidence\n")
        for va in sorted(keys):
            o.write("0x%08X\t%s\t%s\t%s\n" % (va, img.sec(va), keys[va][0], keys[va][1]))
    print("data_vas: %d (rtti vftables %d, iat %d, bound %d, manual+code %d; bindings whose witness lacks the VA: %d) -> %s"
          % (len(keys), nrtti, niat, nbound, len(keys) - nrtti - niat - nbound, bad, a.out))


if __name__ == "__main__":
    sys.exit(main())
