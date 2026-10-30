#!/usr/bin/env python3
"""Parse a PE's undocumented 'Rich' header -- the compiler/linker build fingerprint
Microsoft's link.exe stamps between the DOS stub and the PE header. Each entry is
(prodID, buildNumber, count): every MS tool (c1xx.dll frontend, c2.dll backend, masm,
cvtres, link) that contributed object files, with how many objects each produced.

For matching-decomp this is ground truth: it tells you EXACTLY which compiler build(s)
compiled the target, so you can reproduce byte-exact codegen with the same toolset.

Usage: python rich_header.py [path-to-exe]   (defaults to retail Fable.exe)
"""
import struct, sys
from pathlib import Path

DEFAULT = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

# VC prodID -> tool (partial: the ids seen in VC7.1-era binaries). Evidence (2026-10-30 library boundary pass):
#  * the lane's own VS .NET 2003 toolset ($FABLE_VC71/bin) stamps a cl /c of a .c file 0x005F/3077, of a .cpp file
#    0x0060/3077, an ml /c object 0x000F/3077, and a link.exe 7.10.3077 image gets 0x005A/3077 for the linker and
#    0x005E/3052 for the .res object (cvtres.exe 7.10.3052, the cvtres VS .NET 2003 ships);
#  * the VS .NET 2003 Vc7/lib objects retail links agree (tools/decomp_pipeline/libsig/coff_lib.py, @comp.id):
#    MASM sehprolg/chkstk/ftol2/ll*.obj 0x000F/3077, C crtexew/secchk/seccinit/atonexit/fp8.obj 0x005F/3077, C++
#    ti_inst/ehvecdtr/ehvecctr/initsect.obj 0x0060/3077; the import-descriptor objects lib.exe writes into an import
#    library (__IMPORT_DESCRIPTOR_X, X_NULL_THUNK_DATA) are 0x005D (msvcrt.lib/msvcprt.lib 3077, VS 2003 kernel32.lib
#    2179, the WinDDK 3790.1830 import libraries 4035); msvcrt.lib's sdknames\*.obj data-import objects are 0x0069/2067.
#  * retail's Rich header has exactly one 0x005A entry (3077) and one 0x005E entry (3052): one linker, one .rsrc.
# 0x0019 and 0x001C (builds 9210 / 9178, VS .NET 2002 era) are named from the published comp.id lists only.
# Before 2026-10-30 this table called 0x0F "Linker", 0x5D "Utc1310_C (QFE frontend)", 0x5F "Utc1310_CPP" and 0x5A
# "Utc1310_LTCG_C": so "11 objects compiled by 4035" was 4 C + 1 C++ objects plus 6 import descriptors, and the
# linker is 3077 (3052 is cvtres).
PRODID = {
    0x00: "Unknown (unstamped)", 0x01: "Import0 (import-library short records)",
    0x0f: "Masm710", 0x19: "Implib700", 0x1c: "Utc13_C",
    0x5a: "Linker710", 0x5d: "Implib710 (import descriptor objects)", 0x5e: "Cvtres710",
    0x5f: "Utc1310_C", 0x60: "Utc1310_CPP", 0x69: "data-import objects (msvcrt.lib sdknames)",
}
# VC7.1 build numbers of interest.
BUILD_NOTE = {3077: "RTM", 4035: "QFE-4035 <==", 6030: "SP1", 3052: "3052 (VS 2003 cvtres; the Toolkit 2003 cl is pre-RTM 3052)"}


def parse(data: bytes):
    ri = data.find(b"Rich")
    if ri < 0:
        return None
    key = struct.unpack_from("<I", data, ri + 4)[0]
    start = None
    i = ri - 4
    while i >= 0x40:
        if struct.unpack_from("<I", data, i)[0] ^ key == 0x536e6144:  # 'DanS'
            start = i
            break
        i -= 4
    if start is None:
        return None
    rows = []
    p = start + 16  # skip DanS + 3 pad dwords
    while p < ri:
        v = struct.unpack_from("<I", data, p)[0] ^ key
        cnt = struct.unpack_from("<I", data, p + 4)[0] ^ key
        rows.append(((v >> 16) & 0xFFFF, v & 0xFFFF, cnt))
        p += 8
    return {"start": start, "end": ri, "key": key, "entries": rows}


def main():
    exe = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT
    r = parse(exe.read_bytes())
    if not r:
        print("no Rich header"); return 1
    print(f"{exe.name}: Rich @ 0x{r['start']:x}-0x{r['end']:x} key=0x{r['key']:08x}")
    print(f"{'prodID':>7} {'build':>6} {'objs':>6}  tool / note")
    qfe = qfe_lib = 0
    for prod, build, cnt in r["entries"]:
        note = PRODID.get(prod, "")
        bn = BUILD_NOTE.get(build, "")
        if build == 4035:
            if prod in (0x5f, 0x60):
                qfe += cnt
            else:
                qfe_lib += cnt
        print(f"0x{prod:04x} {build:6d} {cnt:6d}  {note} {bn}".rstrip())
    print(f"\nObjects compiled (C/C++) by build 13.10.4035 (QFE): {qfe}; other 4035 objects (import descriptors): {qfe_lib}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
