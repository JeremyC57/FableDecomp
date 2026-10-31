#!/usr/bin/env python3
"""Re-prove a toolchain-library row: re-extract the library member, mask its relocations, compare with Fable.exe.

    python3 tools/decomp_pipeline/libsig/lib_verify.py <addr> [<addr> ...]
    python3 tools/decomp_pipeline/libsig/lib_verify.py --source rebuild/src/compiled/00/40/Global_SEHProlog_0040138c.cpp

Prints one line per address: `MATCH` or `MISMATCH` (exit status 1 when any row fails).

The claim comes from the landed stub's header line (--source, or the catalog Source of the address when the stub
exists) or else from rebuild/oracles/library_signatures.tsv:
    // Toolchain-library: <lib>(<member>) <symbol> -- <n> bytes verified against Fable.exe 0x<ADDR>
A COFF claim is checked by finding <lib> in the library directories (libsig_scan.default_libdirs: the VS .NET 2003
Vc7/lib copy first, then $FABLE_VC71/lib), taking the code section of <member> that defines <symbol>, masking its
relocation fields and comparing it byte for byte with retail at the section's address (the symbol's address minus
its offset in the section); the symbol's own <n> bytes must be inside the compared range, and every relocation
that can be resolved (IAT slot, import thunk, RTTI vftable, same-section label) must agree with retail. An import
thunk claim (<member> is the DLL name of a short import record) is checked as `FF 25 <slot>` where the import
directory names <slot> as the DLL export the record produces. Import libraries are looked up in the library
directories and then in the import-library directories (libsig_scan.default_implibdirs: the WinDDK 3790 wnet/i386
libraries the scan named the Platform SDK thunks from).
An import library that is not on disk (NOT_ON_DISK: the DirectX SDK April 2005 d3dx9.lib for d3dx9_25.dll) cannot
show its import record, so its thunk claim is checked against the retail import directory alone: `FF 25 <slot>`
where <slot> is <member>!<export> and <export> is the claim symbol without its `_` / `@N` decoration. The printed
reason says so; every other missing library is a MISMATCH.
A third-party assembled-source claim (<lib> is a source tree such as `lzo-1.08`, <member> a MASM file inside it such as
`src/i386/d_asm2/lzo1x_f1.asm`; owner decision 2026-10-31: bytes retail got by assembling a vendor's shipped .asm are
landed the retail way, proof-verified like a library member) is checked from the third-party source directories
(FABLE_THIRDPARTY, pathsep-separated; default $S/thirdparty, searched one level deep: $S/thirdparty/lzo/lzo-1.08): the
file's `db` byte table must be exactly the claimed <n> bytes and equal retail at the address, and when the lane's
MASM (`$FABLE_VC71/bin/ml.exe`, ML 7.10 under wine) is available the file is assembled and the object's code section
that defines <symbol> must hold the same bytes with no relocations. The reason names which of the two proofs ran.
"""
from __future__ import annotations

import csv
import glob
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import coff_lib as C  # noqa: E402
import libsig_scan as LS  # noqa: E402

ROOT = LS.ROOT
CLAIM = re.compile(r"^//\s*Toolchain-library:\s*(\S+?)\((.+)\)\s+(\S+)\s+--\s+(\d+)\s+bytes verified against Fable\.exe\s+"
                   r"(0x[0-9A-Fa-f]{8})\s*$", re.M)


def parse_claim(text):
    m = CLAIM.search(text)
    if not m:
        return None
    lib, member, sym, n, addr = m.groups()
    return dict(lib=lib, member=member, symbol=sym, length=int(n), address=int(addr, 16))


def claim_from_oracle(addr):
    p = ROOT / "rebuild" / "oracles" / "library_signatures.tsv"
    if not p.exists():
        return None
    with open(p, encoding="utf-8") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            if int(r["address"], 16) == addr:
                return dict(lib=r["lib"].split("/")[-1], member=r["member"], symbol=r["symbol"],
                            length=int(r["length"]), address=addr, kind=r["kind"])
    return None


def catalog_source(addr):
    ps = ROOT / "rebuild" / "build_candidates.ps1"
    if not ps.exists():
        return None
    key = f"Address = '{addr:08x}'"
    text = ps.read_text(encoding="utf-8", errors="ignore")
    i = text.find(key)
    if i < 0:
        return None
    m = re.search(r"Source = '([^']+)'", text[i:i + 600])
    p = ROOT / "rebuild" / "src" / "compiled" / m.group(1) if m else None
    return p if p and p.exists() else None


# Import libraries the build links that are not on the lane's disk; their thunks are proven from the retail
# import directory only (verify_import_iat).
NOT_ON_DISK = {"d3dx9.lib": "DirectX SDK (April 2005) import library for d3dx9_25.dll"}


def default_thirdparty_dirs():
    env = os.environ.get("FABLE_THIRDPARTY")
    if env:
        return [Path(p) for p in env.split(os.pathsep) if p and Path(p).is_dir()]
    out = []
    if os.environ.get("S"):
        out.append(Path(os.environ["S"]) / "thirdparty")
    return [d for d in out if d.is_dir()]


def find_thirdparty_source(claim):
    """<dir>/<lib>/<member> or <dir>/*/<lib>/<member> in the third-party source directories."""
    for d in default_thirdparty_dirs():
        for cand in [d / claim["lib"] / claim["member"]] + \
                [Path(x) / claim["member"] for x in glob.glob(str(d / "*" / claim["lib"]))]:
            if cand.is_file():
                return cand
    return None


def asm_db_bytes(text):
    """The bytes of every `db` statement of a MASM file, in file order (decimal or trailing-h hex operands)."""
    out = bytearray()
    for line in text.splitlines():
        s = line.split(";", 1)[0].strip()
        if not s.lower().startswith("db "):
            continue
        for tok in s[3:].split(","):
            tok = tok.strip().lower()
            out.append(int(tok[:-1], 16) if tok.endswith("h") else int(tok, 0))
    return bytes(out)


def assemble_masm(asm_path, workdir):
    """Assemble with the lane's ML 7.10 (`$FABLE_VC71/bin/ml.exe`, through wine off Windows). (obj path, reason)."""
    vc = os.environ.get("FABLE_VC71")
    ml = Path(vc) / "bin" / "ml.exe" if vc else None
    if not ml or not ml.is_file():
        return None, "ML unavailable (no $FABLE_VC71/bin/ml.exe)"
    src = Path(workdir) / asm_path.name
    shutil.copy(asm_path, src)
    obj = Path(workdir) / (asm_path.stem + ".obj")
    cmd = [str(ml), "/nologo", "/c", "/Fo", obj.name, src.name]
    if os.name != "nt":
        cmd = [os.environ.get("FABLE_WINE", "wine")] + cmd
    try:
        r = subprocess.run(cmd, cwd=str(workdir), capture_output=True, text=True, timeout=300)
    except Exception as ex:  # wine missing, timeout
        return None, f"ML not run ({type(ex).__name__})"
    if r.returncode or not obj.is_file():
        return None, "ML failed: " + (r.stdout + r.stderr).strip().replace("\n", " ")[:200]
    return obj, "assembled"


def verify_thirdparty_asm(pe, claim, src_path):
    """A vendor .asm whose assembled bytes retail links: db table == retail, and (when ML runs) the object agrees."""
    text = src_path.read_text(encoding="utf-8", errors="ignore")
    if not re.search(r"^\s*public\s+" + re.escape(claim["symbol"]) + r"\s*$", text, re.M) or \
            not re.search(r"^\s*" + re.escape(claim["symbol"]) + r"\s*:", text, re.M):
        return False, f"{claim['symbol']} is not a public label of {src_path.name}"
    db = asm_db_bytes(text)
    if len(db) != claim["length"]:
        return False, f"db table is {len(db)} bytes, claim says {claim['length']}"
    o = claim["address"] - pe.text_va
    retail = pe.text[o:o + len(db)]
    diff = [i for i in range(len(db)) if db[i] != retail[i]]
    if diff:
        return False, f"{len(diff)} byte(s) differ, first +0x{diff[0]:x} (asm {db[diff[0]]:02x} retail {retail[diff[0]]:02x})"
    with tempfile.TemporaryDirectory() as wd:
        obj, why = assemble_masm(src_path, wd)
        if obj is None:
            return True, f"third-party {claim['lib']}/{claim['member']} db table {len(db)} bytes == retail; {why}"
        co = C.CoffObject(obj.read_bytes())
        for cs in co.code_sections():
            off = next((o_ for o_, n, _ in cs.symbols if n == claim["symbol"]), None)
            if off is None:
                continue
            if cs.relocs:
                return False, f"assembled section has {len(cs.relocs)} relocations; the claim allows none"
            if off != 0 or len(cs.data) != len(db) or bytes(cs.data) != db:
                return False, f"assembled section ({len(cs.data)} bytes at +{off}) differs from the db table"
            return True, (f"third-party {claim['lib']}/{claim['member']} assembled by ML ({obj.name} @comp.id "
                          f"{co.comp_ids()}): section {cs.index} {cs.name} {len(cs.data)} bytes == db table == retail, 0 relocations")
        return False, f"{claim['symbol']} is not defined in a code section of the assembled object"


def find_libs(name):
    out = []
    for tag, d in LS.default_libdirs():
        p = Path(d) / name
        if p.is_file():
            out.append((tag, p))
    for d in LS.default_implibdirs():
        p = Path(d) / name
        if p.is_file():
            out.append(("implib", p))
    return out


def verify_import_iat(pe, claim):
    """An import thunk whose import library is not on disk: `FF 25 <slot>` with <slot> = <member>!<export>."""
    o = claim["address"] - pe.text_va
    got = pe.text[o:o + 6]
    if claim["length"] != 6 or got[:2] != b"\xff\x25":
        return False, f"retail bytes {got.hex()} are not a 6-byte `jmp dword ptr [slot]` thunk"
    slot = struct.unpack_from("<I", got, 2)[0]
    dll, name = pe.iat.get(slot, ("?", "?"))
    export = re.sub(r"@\d+$", "", claim["symbol"])
    export = export[1:] if export.startswith("_") else export
    if dll.lower() != claim["member"].lower() or name != export:
        return False, f"IAT slot 0x{slot:08X} is {dll}!{name}, the claim gives {claim['member']}!{export}"
    return True, (f"jmp dword ptr [__imp_{claim['symbol']}] ; IAT slot 0x{slot:08X} = {dll}!{name} "
                  f"({claim['lib']} not on disk: {NOT_ON_DISK[claim['lib']]}; thunk proven against the retail "
                  f"import directory only)")


def _definition(arc, obj, sym):
    """(object, symbol) defining `sym` in this object or another member of the archive, or (None, None)."""
    for o in [obj] + [m.coff() for m in arc.members if m.kind == "coff"]:
        for s in o.symbols:
            if s.name == sym and s.section > 0 and s.storage in (2, 3):
                return o, s
    return None, None


def defined_bytes_match(pe, arc, obj, sym, tgt):
    """True/False when the library's definition of `sym` can be compared with retail at `tgt`, else None."""
    o, s = _definition(arc, obj, sym)
    if o is None:
        return None
    sec = o.sections[s.section - 1]
    if not sec["ptr"]:
        return None                                   # .bss: nothing to compare
    data = o.section_bytes(sec)
    masked = set()
    for r in o.relocs(sec):
        masked.update(range(r.offset, r.offset + r.size))
    is_code = bool(sec["ch"] & (C.IMAGE_SCN_CNT_CODE | C.IMAGE_SCN_MEM_EXECUTE))
    lo, hi = (0, len(data)) if is_code else (s.value, min(len(data), s.value + 16))
    base = tgt - s.value
    try:
        rb = pe.data[pe.off(base + lo - pe.image_base):pe.off(base + lo - pe.image_base) + (hi - lo)]
    except ValueError:
        return False
    if len(rb) != hi - lo:
        return False
    return all(data[k] == rb[k - lo] for k in range(lo, hi) if k not in masked)


def verify_import(pe, claim, lib_path):
    arc = C.Archive(lib_path)
    for m, imp in arc.imports():
        if m.name == claim["member"] and imp.symbol == claim["symbol"]:
            o = claim["address"] - pe.text_va
            got = pe.text[o:o + 6]
            if claim["length"] != 6 or got[:2] != b"\xff\x25":
                return False, f"retail bytes {got.hex()} are not a 6-byte `jmp dword ptr [slot]` thunk"
            slot = struct.unpack_from("<I", got, 2)[0]
            dll, name = pe.iat.get(slot, ("?", "?"))
            if dll.lower() != imp.dll.lower() or name != imp.export_name:
                return False, f"IAT slot 0x{slot:08X} is {dll}!{name}, the import record gives {imp.dll}!{imp.export_name}"
            return True, (f"jmp dword ptr [__imp_{imp.symbol}] ; IAT slot 0x{slot:08X} = {dll}!{name} "
                          f"(short import, {imp.type_name}, {C.NAME_TYPES.get(imp.name_type)})")
    return False, f"no short import record {claim['symbol']} in {lib_path.name}({claim['member']})"


def verify_coff(pe, claim, lib_path, tag):
    arc = C.Archive(lib_path)
    mem = next((m for m in arc.members if m.name == claim["member"] and m.kind == "coff"), None)
    if mem is None:
        return False, f"member {claim['member']} not in {lib_path}"
    obj = mem.coff()
    for cs in obj.code_sections():
        off = next((o for o, n, _ in cs.symbols if n == claim["symbol"]), None)
        if off is None:
            continue
        if off + claim["length"] > len(cs.data):
            return False, f"{claim['symbol']} +{off} with {claim['length']} bytes runs past the {len(cs.data)}-byte section"
        sec_va = claim["address"] - off
        o = sec_va - pe.text_va
        retail = pe.text[o:o + len(cs.data)]
        masked = set()
        for r in cs.relocs:
            masked.update(range(r.offset, r.offset + r.size))
        diff = [i for i in range(len(cs.data)) if i not in masked and cs.data[i] != retail[i]]
        if diff:
            return False, (f"{len(diff)} byte(s) differ outside relocations, first +0x{diff[0]:x} "
                           f"(lib {cs.data[diff[0]]:02x} retail {retail[diff[0]]:02x})")
        # relocation identity against retail (thunks found on the fly, IAT from the import directory)
        by_export, by_imp, code_syms = LS.load_imports(LS.default_libdirs(), LS.default_implibdirs())
        thunks = {}
        for r in cs.relocs:
            sym = r.symbol
            if sym in code_syms:
                raw = pe.u32(sec_va + r.offset)
                addend = struct.unpack_from("<i", cs.data, r.offset)[0]
                t = (sec_va + r.offset + 4 + struct.unpack("<i", struct.pack("<I", raw))[0] - addend) & 0xFFFFFFFF \
                    if r.type == 0x14 else (raw - addend) & 0xFFFFFFFF
                to = t - pe.text_va
                if 0 <= to < len(pe.text) - 6 and pe.text[to:to + 2] == b"\xff\x25":
                    slot = struct.unpack_from("<I", pe.text, to + 2)[0]
                    if slot in pe.iat:
                        thunks[t] = pe.iat[slot]
        prov = dict(obj=obj, data=cs.data, relocs=cs.relocs, sec=cs.index)
        rel, ok, bad = LS.resolve(pe, sec_va, prov, by_imp, code_syms, thunks, {})
        # second look at what resolve() could not prove: a target the same library defines (another section of this
        # object, or another member) must hold that definition's bytes in retail (code masked at its relocations;
        # initialised data compared over its first bytes, e.g. secchk.obj -> seccook.obj __security_cookie 0xBB40E64E)
        for i, r in enumerate(cs.relocs):
            if i >= len(rel) or not (rel[i].endswith("[?]") or rel[i].endswith("[sec]")):
                continue
            tgt = int(rel[i].split("=0x", 1)[1][:8], 16)
            v = defined_bytes_match(pe, arc, obj, r.symbol, tgt)
            if v is not None:
                rel[i] = rel[i].rsplit("[", 1)[0] + ("[ok:lib]" if v else "[bad:lib]")
                ok += v
                bad += not v
        if bad:
            return False, "relocation contradicts retail: " + " ".join(x for x in rel if "[bad]" in x)
        return True, (f"{tag}/{lib_path.name}({claim['member']}) section {cs.index} {cs.name} {len(cs.data)} bytes at "
                      f"0x{sec_va:08X}, {len(masked)} masked relocation bytes, {ok}/{len(cs.relocs)} relocations proven"
                      + (f": {' '.join(rel)}" if rel else ""))
    return False, f"{claim['symbol']} is not defined in a code section of {claim['member']}"


def verify(claim, pe=None):
    pe = pe or LS.PE(os.environ["FABLE_EXE"])
    libs = find_libs(claim["lib"])
    if not libs and claim["lib"] in NOT_ON_DISK and claim["member"].lower().endswith(".dll"):
        return verify_import_iat(pe, claim)
    if not libs:
        src = find_thirdparty_source(claim)
        if src is not None and src.suffix.lower() == ".asm":
            return verify_thirdparty_asm(pe, claim, src)
        return False, (f"{claim['lib']} is not in any library directory ({LS.default_libdirs()}) "
                       f"or third-party source directory ({default_thirdparty_dirs()})")
    tried = []
    for tag, p in libs:
        if claim["member"].lower().endswith(".dll"):
            ok, msg = verify_import(pe, claim, p)
        else:
            ok, msg = verify_coff(pe, claim, p, tag)
        if ok:
            return ok, msg
        tried.append(f"{tag}/{p.name}: {msg}")
    return False, " | ".join(tried)


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    if not os.environ.get("FABLE_EXE"):
        print("set FABLE_EXE to the retail Fable.exe")
        return 2
    pe = LS.PE(os.environ["FABLE_EXE"])
    jobs = []
    if argv[0] == "--source":
        for f in argv[1:]:
            c = parse_claim(Path(f).read_text(encoding="utf-8", errors="ignore"))
            jobs.append((f, c))
    else:
        for a in argv:
            addr = int(a, 16)
            src = catalog_source(addr)
            c = parse_claim(src.read_text(encoding="utf-8", errors="ignore")) if src else None
            jobs.append((f"{addr:08x}", c or claim_from_oracle(addr)))
    fails = 0
    for label, c in jobs:
        if not c:
            print(f"{label} MISMATCH no Toolchain-library claim (stub header or library_signatures.tsv row)")
            fails += 1
            continue
        ok, msg = verify(c, pe)
        print(f"{c['address']:08x} {'MATCH' if ok else 'MISMATCH'} {c['lib']}({c['member']}) {c['symbol']} "
              f"{c['length']} bytes -- {msg}")
        fails += not ok
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
