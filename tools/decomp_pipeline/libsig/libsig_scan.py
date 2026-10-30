#!/usr/bin/env python3
"""Signature-match the era VC7.1 library archives against retail Fable.exe -> rebuild/oracles/library_signatures.tsv

    python3 tools/decomp_pipeline/libsig/libsig_scan.py [--out rebuild/oracles/library_signatures.tsv]
        [--libdir TAG=DIR ...] [--implibdir DIR ...] [--min-len 8] [--log DIR]

Bytes in retail `.text` that the linker copied from a prebuilt library are not Fable source: the CRT objects
pulled from msvcrt.lib (sehprolg.obj, secchk.obj, crtexew.obj, ...) and the `jmp dword ptr [__imp_X]` thunks the
linker synthesises for import-library members. This tool finds them two ways:

  coff_section  every code section (>= --min-len bytes) of every object in every archive (and loose .obj) under
                the library directories is masked at its relocation fields and searched in retail .text
                (byte-exact outside the relocations). Each relocation is then resolved against retail: `__imp_X`
                must be the IAT slot the import directory names X, a code import must be the `jmp [__imp_X]`
                thunk of X, a symbol defined by another matched section must be that section's address, a
                section-relative fixup must land in the right retail section. One row per function symbol the
                matched section defines (a section with two symbols, sehprolg.obj's __SEH_prolog +
                __SEH_epilog, gives two rows).
  import_thunk  every `FF 25 <IAT slot>` in .text that is a thunk (a direct call/jmp target, a manifest
                function start, or adjacent to another thunk), named from the import directory and tied to the
                import-library member (short import record) that makes the linker emit it.

Library directories (first listed wins when two give identical bytes; the others go in `notes`):
  FABLE_LIBSIG_LIBDIRS (os.pathsep-separated TAG=DIR) or, by default,
    vs2003rtm=$FABLE_VS2003_LIB   VS .NET 2003 Vc7/lib (cl 13.10.3077 RTM: the CRT retail links, MSVCR71/MSVCP71
                                  imports; fetch with tools/decomp_pipeline/libsig/fetch_vs2003_libs.sh)
    ddk3790=$FABLE_VC71/lib       the lane's WinDDK 3790.1830 CRT (msvcrt.dll imports, bufferoverflowu /GS)
  import-name libraries: FABLE_LIBSIG_IMPLIBDIRS, default $FABLE_WINDDK_LIB (WinDDK wnet/i386 import libs)
Retail exe: FABLE_EXE.

Confidence: high = unique in .text, no relocation contradicts retail and (>= 16 bytes or >= 1 relocation
confirmed); medium = unique, no contradiction, short and unconfirmed; low = several retail copies or a
contradicted relocation (kept for the record, never used to land). A match that is not at a manifest function
start, a padding boundary or next to another library match is dropped unless it is >= 32 bytes (logged).
"""
from __future__ import annotations

import argparse
import bisect
import csv
import os
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import coff_lib as C  # noqa: E402

ROOT = Path(os.environ.get("FABLE_ROOT", HERE.parents[2]))
IMAGE_BASE = 0x400000


# ------------------------------------------------------------------ retail PE
class PE:
    def __init__(self, path):
        self.path = Path(path)
        d = self.data = self.path.read_bytes()
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        n = struct.unpack_from("<H", d, pe + 6)[0]
        opt = struct.unpack_from("<H", d, pe + 20)[0]
        self.image_base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
        self.sections = []
        for i in range(n):
            o = pe + 24 + opt + 40 * i
            name = d[o:o + 8].rstrip(b"\0").decode("latin-1")
            vs, va, rs, ro = struct.unpack_from("<IIII", d, o + 8)
            self.sections.append(dict(name=name, va=va, vs=vs, rs=rs, ro=ro))
        t = self.section(".text")
        self.text_va = self.image_base + t["va"]
        self.text = d[t["ro"]:t["ro"] + min(t["vs"], t["rs"])]
        imp_rva, imp_size = struct.unpack_from("<II", d, pe + 24 + 104)
        self.iat = {}                       # slot VA -> (dll, name)
        o = self.off(imp_rva)
        while True:
            oft, _, _, name_rva, ft = struct.unpack_from("<IIIII", d, o)
            if ft == 0:
                break
            dll = d[self.off(name_rva):].split(b"\0", 1)[0].decode("latin-1")
            t_ = self.off(oft or ft)
            a = ft
            while True:
                e = struct.unpack_from("<I", d, t_)[0]
                if e == 0:
                    break
                nm = f"#{e & 0xFFFF}" if e & 0x80000000 else d[self.off(e) + 2:].split(b"\0", 1)[0].decode("latin-1")
                self.iat[self.image_base + a] = (dll, nm)
                t_ += 4
                a += 4
            o += 20

    def section(self, name):
        return next(s for s in self.sections if s["name"] == name)

    def off(self, rva):
        for s in self.sections:
            if s["va"] <= rva < s["va"] + max(s["vs"], s["rs"]):
                return s["ro"] + rva - s["va"]
        raise ValueError(f"rva 0x{rva:x} outside the image")

    def section_of(self, va):
        rva = va - self.image_base
        for s in self.sections:
            if s["va"] <= rva < s["va"] + max(s["vs"], s["rs"]):
                return s["name"]
        return None

    def u32(self, va):
        if self.text_va <= va <= self.text_va + len(self.text) - 4:
            return struct.unpack_from("<I", self.text, va - self.text_va)[0]
        return struct.unpack_from("<I", self.data, self.off(va - self.image_base))[0]

    def cstr(self, va, limit=512):
        o = self.off(va - self.image_base)
        return self.data[o:o + limit].split(b"\0", 1)[0].decode("latin-1")

    def rtti_name(self, vftable_va):
        """TypeDescriptor name of the class whose vftable is at `vftable_va` (MSVC RTTI), or None."""
        try:
            col = self.u32(vftable_va - 4)
            if self.section_of(col) not in (".rdata", ".data"):
                return None
            td = self.u32(col + 12)
            if self.section_of(td) not in (".rdata", ".data"):
                return None
            return self.cstr(td + 8)
        except (ValueError, struct.error):
            return None


# ------------------------------------------------------------------ library side
def vs2003_libdir():
    """The VS .NET 2003 Vc7/lib copy: $FABLE_VS2003_LIB, else $S/wine/vs2003lib, else $S/libsig/work/vs2003_lib
    (tools/decomp_pipeline/libsig/fetch_vs2003_libs.sh fills one from the archive.org item the lane's RTM cl.exe
    came from)."""
    cands = [os.environ.get("FABLE_VS2003_LIB")]
    if os.environ.get("S"):
        cands += [str(Path(os.environ["S"]) / "wine" / "vs2003lib"), str(Path(os.environ["S"]) / "libsig" / "work" / "vs2003_lib")]
    return next((c for c in cands if c and (Path(c) / "msvcrt.lib").is_file()), None)


def default_libdirs():
    env = os.environ.get("FABLE_LIBSIG_LIBDIRS")
    if env:
        return [tuple(p.split("=", 1)) for p in env.split(os.pathsep) if "=" in p]
    out = []
    vs = vs2003_libdir()
    if vs:
        out.append(("vs2003rtm", vs))
    vc = os.environ.get("FABLE_VC71")
    if vc and (Path(vc) / "lib").is_dir():
        out.append(("ddk3790", str(Path(vc) / "lib")))
    return out


def default_implibdirs():
    env = os.environ.get("FABLE_LIBSIG_IMPLIBDIRS")
    if env:
        return [p for p in env.split(os.pathsep) if p]
    d = os.environ.get("FABLE_WINDDK_LIB")
    if not d and os.environ.get("S"):
        d = str(Path(os.environ["S"]) / "wine" / "winddk" / "3790.1830" / "lib" / "wnet" / "i386")
    return [d] if d and Path(d).is_dir() else []


# What retail links (Fable.exe imports MSVCR71.dll / MSVCP71.dll / KERNEL32.dll, so the /MD import libraries and
# their static members; the loose Vc7/lib objects are ordinary linker inputs). libc*/libcp* and the debug libraries
# are not linked: a match found only there is header-template code a Fable TU compiled (or an identical small body),
# written to the unlinked log, never to the oracle.
LINKED = {"msvcrt.lib", "msvcprt.lib", "kernel32.lib", "oldnames.lib", "comsupp.lib", "chkstk.obj", "fp10.obj",
          "binmode.obj", "commode.obj", "newmode.obj", "setargv.obj", "wsetargv.obj", "thrownew.obj", "nochkclr.obj"}
LINKED_TAGS = {"vs2003rtm"}
DEBUG_LIBS = {"msvcrtd.lib", "msvcprtd.lib", "libcd.lib", "libcmtd.lib", "libcpd.lib", "libcpmtd.lib", "libcid.lib",
              "libcimtd.lib", "msvcirtd.lib"}


def lib_rank(tag_order, prov):
    n = prov["lib"].lower()
    r = 0 if n == "msvcrt.lib" else 1 if n == "msvcprt.lib" else 2 if n in LINKED else 4 if n in DEBUG_LIBS else 3
    return (tag_order.get(prov["tag"], 99), r, n, prov["member"])


def is_linked(prov):
    return prov["tag"] in LINKED_TAGS and prov["lib"].lower() in LINKED


def lib_files(d):
    p = Path(d)
    return sorted([*p.glob("*.lib"), *p.glob("*.obj")], key=lambda x: x.name.lower())


def is_function_symbol(name, storage, typ):
    if name.startswith((".", "$", "??_C@", "__real@", "__xmm@")) or name in ("@comp.id", "@feat.00"):
        return False
    return storage == 2 or (storage == 3 and (typ & 0x20))


def load_sections(libdirs, min_len, log):
    """-> {masked key: [provider]}; provider = dict(lib, member, tag, sec, data, relocs, syms, comp)."""
    pats = defaultdict(list)
    nsec = 0
    for tag, d in libdirs:
        for f in lib_files(d):
            try:
                members = ([(m.name, m) for m in C.Archive(f).members if m.kind == "coff"]
                           if f.suffix.lower() == ".lib" else [(f.name, C.Member(f.name, 0, f.read_bytes()))])
            except (ValueError, struct.error, IndexError) as ex:
                log.write(f"SKIP {tag}/{f.name}: {ex}\n")
                continue
            for mname, mem in members:
                try:
                    obj = mem.coff()
                    secs = obj.code_sections(min_len)
                except (struct.error, IndexError, ValueError) as ex:
                    log.write(f"SKIP {tag}/{f.name}({mname}): {ex}\n")
                    continue
                syms_full = {s.name: s for s in obj.symbols}
                for cs in secs:
                    mask = bytearray(len(cs.data))
                    for r in cs.relocs:
                        for k in range(r.offset, min(r.offset + r.size, len(mask))):
                            mask[k] = 1
                    key = bytes(b if not m else 0 for b, m in zip(cs.data, mask)) + b"|" + bytes(mask)
                    fsyms = [(o, n) for o, n, st in cs.symbols
                             if is_function_symbol(n, st, syms_full[n].type if n in syms_full else 0)]
                    pats[key].append(dict(lib=f.name, member=mname, tag=tag, sec=cs.index, secname=cs.name,
                                          data=cs.data, mask=bytes(mask), relocs=cs.relocs, syms=fsyms,
                                          comdat=cs.comdat, comp=obj.comp_ids(), obj=obj))
                    nsec += 1
    log.write(f"library code sections >= {min_len} bytes: {nsec}, distinct masked patterns: {len(pats)}\n")
    return pats


def load_imports(libdirs, implibdirs):
    """(dll lower, export name) -> (lib tag/file, member, symbol); also __imp_ symbol -> (dll, export)."""
    by_export, by_imp, code_syms = {}, {}, {}
    dirs = [(t, d) for t, d in libdirs] + [("implib", d) for d in implibdirs]
    for tag, d in dirs:
        for f in lib_files(d):
            if f.suffix.lower() != ".lib":
                continue
            try:
                arc = C.Archive(f)
            except (ValueError, struct.error):
                continue
            for m, imp in arc.imports():
                k = (imp.dll.lower(), imp.export_name)
                by_export.setdefault(k, (f"{tag}/{f.name}", m.name, imp.symbol, imp.type_name))
                by_imp.setdefault("__imp_" + imp.symbol, k)
                if imp.type == 0:
                    code_syms.setdefault(imp.symbol, k)
    return by_export, by_imp, code_syms


# ------------------------------------------------------------------ search
def segments(mask):
    """Unmasked runs [(start, end)] of a pattern."""
    out, s = [], None
    for i, m in enumerate(mask):
        if not m and s is None:
            s = i
        elif m and s is not None:
            out.append((s, i))
            s = None
    if s is not None:
        out.append((s, len(mask)))
    return out


def search(text, data, mask, cap=4000):
    segs = segments(mask)
    if not segs:
        return [], "all_masked"
    a0, a1 = max(segs, key=lambda x: x[1] - x[0])
    if a1 - a0 < 4:
        return [], "anchor<4"
    a1 = min(a1, a0 + 48)
    anchor = data[a0:a1]
    hits = []
    p = text.find(anchor)
    n = 0
    while p >= 0:
        n += 1
        if n > cap:
            return hits, "too_common"
        st = p - a0
        if st >= 0 and st + len(data) <= len(text):
            if all(text[st + s:st + e] == data[s:e] for s, e in segs):
                hits.append(st)
        p = text.find(anchor, p + 1)
    return hits, ""


def call_targets(text, base):
    """Direct E8/E9 rel32 targets inside .text (evidence for thunks and function starts)."""
    out = set()
    n = len(text)
    for op in (b"\xe8", b"\xe9"):
        p = text.find(op)
        while p >= 0:
            if p + 5 <= n:
                t = base + p + 5 + struct.unpack_from("<i", text, p + 1)[0]
                if base <= t < base + n:
                    out.add(t)
            p = text.find(op, p + 1)
    return out


def load_catalog(root):
    landed = {}
    p = root / "rebuild" / "oracles" / "auto-re-candidates.tsv"
    with open(p, encoding="utf-8-sig") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            landed[int(r["address"], 16)] = (r["name"], r.get("length", ""))
    known = {}
    with open(root / "rebuild" / "manifest" / "functions.tsv", encoding="utf-8-sig") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            known[int(r["address"], 16)] = r["name"]
    parked = set()
    pend = root / "rebuild" / "candidates" / "manual" / "pending"
    if pend.is_dir():
        for st in pend.glob("*/STATUS.tsv"):
            for line in open(st, encoding="utf-8", errors="replace"):
                a = line.split("\t", 1)[0].strip().lower().replace("0x", "")
                if re.fullmatch(r"[0-9a-f]{8}", a):
                    parked.add(int(a, 16))
        for dp in pend.glob("*/differ/*.cpp"):
            m = re.match(r"([0-9a-fA-F]{8})", dp.name)
            if m:
                parked.add(int(m.group(1), 16))
    return landed, known, parked


def catalog_bucket(a, landed, known):
    if a in landed:
        return "landed"
    if a in known:
        return "unlanded"
    return "outside"


# ------------------------------------------------------------------ relocation resolution
def resolve(pe, va, prov, by_imp, code_syms, thunks, symaddr, names=None):
    """-> (list of 'off:TYPE:sym=0xT[ok|bad|sec|?]', n_ok, n_bad). `ok` is a proof (IAT slot, import thunk, RTTI
    vftable, same-section label, another matched library symbol); `sec` only says the target is in the right
    retail section (weak, not counted)."""
    names = names or {}
    out, ok, bad = [], 0, 0
    obj = prov["obj"]
    for r in prov["relocs"]:
        if r.offset + 4 > len(prov["data"]) or r.size != 4:
            out.append(f"+{r.offset:x}:{r.type_name}:{r.symbol}=?")
            continue
        addend = struct.unpack_from("<i", prov["data"], r.offset)[0]
        field_va = va + r.offset
        raw = pe.u32(field_va)
        if r.type == 0x14:          # REL32
            tgt = (field_va + 4 + struct.unpack_from("<i", struct.pack("<I", raw))[0] - addend) & 0xFFFFFFFF
        elif r.type == 0x06:        # DIR32
            tgt = (raw - addend) & 0xFFFFFFFF
        else:
            out.append(f"+{r.offset:x}:{r.type_name}:{r.symbol}=?")
            continue
        verdict = "?"
        sym = r.symbol
        s = obj.symbol(r.symbol_index)
        if sym.startswith("??_7") and sym.endswith("6B@"):
            cls = sym[4:-3]
            got = pe.rtti_name(tgt)
            verdict = "?" if got is None else ("ok" if got in (".?AV" + cls, ".?AU" + cls) else "bad")
            if got and verdict == "bad":
                sym += f"(rtti {got})"
        elif sym in by_imp:
            want = by_imp[sym]
            got = pe.iat.get(tgt)
            verdict = "ok" if got and (got[0].lower(), got[1]) == want else "bad"
        elif sym in code_syms:
            got = thunks.get(tgt)
            verdict = "ok" if got and (got[0].lower(), got[1]) == code_syms[sym] else ("bad" if tgt in pe.iat or got else "?")
        elif s is not None and s.section > 0 and s.storage == 3 and s.section == prov["sec"]:
            verdict = "ok" if tgt == va + s.value else "bad"
        elif s is not None and s.section > 0:
            want_sec = obj.sections[s.section - 1]["name"].split("$")[0]
            got_sec = pe.section_of(tgt) or "?"
            if sym in symaddr:
                verdict = "ok" if tgt in symaddr[sym] else "bad"
            elif want_sec in (".text",):
                verdict = "sec" if got_sec == ".text" else "bad"
            elif want_sec in (".data", ".bss", ".rdata", ".CRT", ".xdata"):
                verdict = "sec" if got_sec in (".data", ".rdata") else "bad"
        elif sym in symaddr:
            verdict = "ok" if tgt in symaddr[sym] else "bad"
        if tgt in names and verdict in ("?", "sec"):
            sym += f"(catalog {names[tgt]})"
        if verdict == "ok":
            ok += 1
        elif verdict == "bad":
            bad += 1
        out.append(f"+{r.offset:x}:{r.type_name}:{sym}=0x{tgt:08X}[{verdict}]")
    return out, ok, bad


# ------------------------------------------------------------------ main
def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--out", default=str(ROOT / "rebuild" / "oracles" / "library_signatures.tsv"))
    ap.add_argument("--libdir", action="append", default=None, help="TAG=DIR (repeatable; first wins)")
    ap.add_argument("--implibdir", action="append", default=None)
    ap.add_argument("--min-len", type=int, default=8)
    ap.add_argument("--log", default=None, help="directory for scan logs")
    ap.add_argument("--exe", default=os.environ.get("FABLE_EXE"))
    a = ap.parse_args(argv)
    if not a.exe:
        raise SystemExit("set FABLE_EXE or pass --exe")
    libdirs = [tuple(x.split("=", 1)) for x in a.libdir] if a.libdir else default_libdirs()
    implibdirs = a.implibdir if a.implibdir is not None else default_implibdirs()
    if not libdirs:
        raise SystemExit("no library directory (set FABLE_VS2003_LIB / FABLE_VC71 or pass --libdir)")
    logdir = Path(a.log) if a.log else None
    if logdir:
        logdir.mkdir(parents=True, exist_ok=True)
    log = open(logdir / "scan.log", "w", encoding="utf-8") if logdir else open(os.devnull, "w")
    log.write(f"exe {a.exe}\nlibdirs {libdirs}\nimplibdirs {implibdirs}\n")

    pe = PE(a.exe)
    text, base = pe.text, pe.text_va
    landed, known, parked = load_catalog(ROOT)
    starts = sorted(known)
    by_export, by_imp, code_syms = load_imports(libdirs, implibdirs)
    targets = call_targets(text, base)
    log.write(f".text 0x{base:08X}+0x{len(text):X}, IAT slots {len(pe.iat)}, call/jmp targets {len(targets)}\n")

    # ---- import thunks
    raw_thunks = {}
    p = text.find(b"\xff\x25")
    while p >= 0:
        slot = struct.unpack_from("<I", text, p + 2)[0] if p + 6 <= len(text) else 0
        if slot in pe.iat:
            raw_thunks[base + p] = slot
        p = text.find(b"\xff\x25", p + 1)
    thunks = {}
    for va, slot in raw_thunks.items():
        nb = (va - 6 in raw_thunks) or (va + 6 in raw_thunks)
        if va in targets or va in known or nb:
            thunks[va] = pe.iat[slot]
    log.write(f"FF25->IAT sites {len(raw_thunks)}, classified thunks {len(thunks)}, "
              f"in-function IAT jumps {len(raw_thunks) - len(thunks)}\n")
    for va in sorted(set(raw_thunks) - set(thunks)):
        log.write(f"  not a thunk (mid-function jmp [IAT]) 0x{va:08X} -> {pe.iat[raw_thunks[va]]}\n")

    # ---- library sections
    pats = load_sections(libdirs, a.min_len, log)
    tag_order = {t: i for i, (t, _) in enumerate(libdirs)}
    for key in pats:
        pats[key].sort(key=lambda pv: lib_rank(tag_order, pv))
    matches = []        # (va, key, hits)
    stats = defaultdict(int)
    for key, provs in pats.items():
        pv = provs[0]
        hits, why = search(text, pv["data"], pv["mask"])
        if why:
            stats[why] += 1
        if hits:
            matches.append((key, [base + h for h in hits]))
    log.write(f"patterns with >= 1 retail hit: {len(matches)}; skipped {dict(stats)}\n")

    # a pattern only unlinked libraries give is not linked library code: log it (with its evidence) and drop it
    unlinked = [(k, v) for k, v in matches if not any(is_linked(pv) for pv in pats[k])]
    matches = [(k, v) for k, v in matches if any(is_linked(pv) for pv in pats[k])]
    for key in [k for k, _ in matches]:
        pats[key] = [pv for pv in pats[key] if is_linked(pv)] + [pv for pv in pats[key] if not is_linked(pv)]
    log.write(f"patterns in linked libraries with a hit: {len(matches)}; only in unlinked libraries: {len(unlinked)}\n")
    names = {a_: n_ for a_, (n_, _) in landed.items()}
    for a_, n_ in known.items():
        names.setdefault(a_, n_)
    symaddr = defaultdict(set)          # symbol -> retail addresses (thunks now, matched sections below)
    for t, (dll, nm) in thunks.items():
        k = (dll.lower(), nm)
        if k in by_export:
            symaddr[by_export[k][2]].add(t)

    # ---- assign each retail hit one library section (one retail address = one library symbol)
    cand = {}                           # va -> (n, provider, copies, note)
    ambiguous = []
    for key, vas in matches:
        provs, seen_ = [], set()
        for pv in pats[key]:
            if is_linked(pv) and (pv["lib"], pv["member"], pv["sec"]) not in seen_:
                seen_.add((pv["lib"], pv["member"], pv["sec"]))
                provs.append(pv)
        n = len(pats[key][0]["data"])
        score = {}
        for va in vas:
            for i, pv in enumerate(provs):
                _, ok, bad = resolve(pe, va, pv, by_imp, code_syms, thunks, symaddr, names)
                score[(va, i)] = (ok, bad)
        if len(vas) == 1:
            va = vas[0]
            i = max(range(len(provs)), key=lambda j: (score[(va, j)][0] - 10 * score[(va, j)][1], -j))
            cand[va] = (n, provs[i], 1, "", key)
            continue
        proven = {}
        for va in vas:
            best = [i for i in range(len(provs)) if score[(va, i)][0] >= 1 and score[(va, i)][1] == 0]
            if best:
                proven[va] = max(best, key=lambda j: (score[(va, j)][0], -j))
        used = list(proven.values())
        if proven and len(used) == len(set(used)):
            for va, i in proven.items():
                cand[va] = (n, provs[i], len(vas), f"{len(vas)} retail copies of these bytes; relocations prove this one", key)
            for va in vas:
                if va not in proven:
                    ambiguous.append((va, n, len(vas), provs[0], "copy without relocation proof"))
            continue
        same_member = len({(pv["lib"], pv["member"]) for pv in provs}) == 1
        if len(provs) == len(vas) > 1 and same_member and \
                all(score[(va, i)][1] == 0 for i, va in enumerate(sorted(vas))):
            ordered = sorted(provs, key=lambda pv: pv["sec"])
            for va, pv in zip(sorted(vas), ordered):
                cand[va] = (n, pv, len(vas), f"{len(vas)} sections of {pv['member']} share these bytes; "
                                             "assigned in object section order (the linker keeps it)", key)
            continue
        for va in vas:
            ambiguous.append((va, n, len(vas), provs[0], "several retail copies, no relocation proof"))

    ends = {va + c[0] for va, c in cand.items()} | {t + 6 for t in thunks}
    lib_starts = set(cand) | set(thunks)

    def boundary_ok(va, n):
        o = va - base
        if va in known or va in targets:
            return True
        if va in ends or (va + n) in lib_starts:
            return True
        prev = text[o - 1:o] if o > 0 else b""
        return prev in (b"\xcc", b"\x90", b"\xc3") or (o >= 3 and text[o - 3] == 0xC2)

    chosen, dropped = {}, []
    for va, c in cand.items():
        if not boundary_ok(va, c[0]) and c[0] < 32:
            dropped.append((va, c[0], c[1]))
            continue
        chosen[va] = c
    # drop a match fully inside a longer chosen one (a small section matching inside a bigger library body)
    vs_sorted = sorted(chosen)
    inner = set()
    for i, va in enumerate(vs_sorted):
        n = chosen[va][0]
        j = i + 1
        while j < len(vs_sorted) and vs_sorted[j] < va + n:
            vb = vs_sorted[j]
            if vb + chosen[vb][0] <= va + n:
                inner.add(vb)
            j += 1
    for vb in inner:
        dropped.append((vb, chosen[vb][0], chosen[vb][1]))
        del chosen[vb]
    log.write(f"section matches kept {len(chosen)}, dropped (no boundary / nested) {len(dropped)}\n")
    for va, n, pv in sorted(dropped, key=lambda x: x[0]):
        log.write(f"  dropped 0x{va:08X} {n} {pv['tag']}/{pv['lib']}({pv['member']}) {pv['syms'][:2]}\n")
    # identical bytes in several members of one library (check_managed_app is in crtexe.obj and crtexew.obj): take
    # the member another matched section already proves was linked (crtexew.obj gives _WinMainCRTStartup)
    def mkey(p_):
        return (p_["tag"], p_["lib"], p_["member"])
    for va in sorted(chosen):
        n, pv, nh, note, key = chosen[va]
        others = {mkey(c[1]) for vb, c in chosen.items() if vb != va}
        if mkey(pv) in others:
            continue
        alt = next((p_ for p_ in pats[key] if is_linked(p_) and mkey(p_) in others), None)
        if alt is not None:
            chosen[va] = (n, alt, nh, "; ".join(x for x in (note, f"same bytes in {pv['member']}; {alt['member']} "
                                                              "chosen: another of its sections is matched") if x), key)
    for va, (n, pv, nh, note, _k) in chosen.items():
        for o, s in pv["syms"]:
            symaddr[s].add(va + o)
    # a thunk that sits right after / right before a matched library section is a thunk too (0x0040136E
    # `jmp [__imp____setusermatherr]` follows fp8.obj's __setdefaultprecision and is reached only through the IAT)
    sec_ends = {va + c[0] for va, c in chosen.items()}
    for va in sorted(set(raw_thunks) - set(thunks)):
        if va in sec_ends or (va + 6) in chosen:
            thunks[va] = pe.iat[raw_thunks[va]]
            log.write(f"  thunk by adjacency to a library section 0x{va:08X} -> {thunks[va]}\n")
            k = (thunks[va][0].lower(), thunks[va][1])
            if k in by_export:
                symaddr[by_export[k][2]].add(va)

    # unlinked-only matches: evidence log (header-template code compiled by Fable TUs, or identical small bodies)
    with open(logdir / "unlinked_matches.tsv" if logdir else os.devnull, "w", encoding="utf-8") as uf:
        uf.write("address\tlength\tcopies\tlib\tmember\tsymbol\tcatalog\tcatalog_name\treloc_ok\treloc_bad\trelocations\n")
        for key, vas in sorted(unlinked, key=lambda x: x[1][0]):
            pv = pats[key][0]
            for va in vas[:50]:
                rel, ok, bad = resolve(pe, va, pv, by_imp, code_syms, thunks, symaddr, names)
                s0 = pv["syms"][0][1] if pv["syms"] else pv["secname"]
                uf.write(f"{va:08x}\t{len(pv['data'])}\t{len(vas)}\t{pv['tag']}/{pv['lib']}\t{pv['member']}\t{s0}\t"
                         f"{catalog_bucket(va, landed, known)}\t{names.get(va, '')}\t{ok}\t{bad}\t{' '.join(rel)}\n")
    rows = []
    for va, (n, pv, nh, note, key) in sorted(chosen.items()):
        rel, ok, bad = resolve(pe, va, pv, by_imp, code_syms, thunks, symaddr, names)
        conf = "low" if bad else ("high" if (ok or (n >= 24 and not pv["relocs"])) else "medium")
        alts = sorted({f"{p['tag']}/{p['lib']}({p['member']})" for p in pats[key]
                       if p is not pv and (p["lib"], p["member"]) != (pv["lib"], pv["member"])})
        alt_note = ("same bytes: " + ", ".join(alts[:6]) + (f" (+{len(alts) - 6})" if len(alts) > 6 else "")) if alts else ""
        syms = pv["syms"] or [(0, f"{pv['secname']}@{pv['member']}")]
        offs = sorted({o for o, _ in syms})
        for o in offs:
            if o >= n:
                continue
            nxt = next((x for x in offs if x > o), n)
            sva = va + o
            cname = landed.get(sva, (known.get(sva, ""),))[0]
            here = [s_ for o_, s_ in syms if o_ == o]
            # aliases at one offset (chkstk.obj: __alloca_probe = __chkstk): one row, the catalog's spelling first
            here.sort(key=lambda s_: (s_.lstrip("_") != cname.lstrip("_").split("::")[-1], s_))
            alias = f"aliases at this address: {', '.join(here[1:])}" if len(here) > 1 else ""
            rows.append(dict(
                address=f"{sva:08x}", length=nxt - o, lib=f"{pv['tag']}/{pv['lib']}", member=pv["member"],
                symbol=here[0], kind="coff_section", section_address=f"{va:08x}", section_length=n,
                relocations=" ".join(rel), reloc_ok=ok, reloc_bad=bad, retail_copies=nh, confidence=conf,
                catalog=catalog_bucket(sva, landed, known), catalog_name=cname,
                parked="yes" if sva in parked else "", comp_id=" ".join(f"{p_}.{b_}" for p_, b_ in pv["comp"][:2]),
                notes="; ".join(x for x in (alias, note, alt_note) if x)))
    for t, (dll, nm) in sorted(thunks.items()):
        k = (dll.lower(), nm)
        src = by_export.get(k)
        slot = raw_thunks[t]
        rows.append(dict(
            address=f"{t:08x}", length=6, lib=src[0] if src else f"(not on disk) {dll.rsplit('.', 1)[0].lower()}.lib",
            member=src[1] if src else dll, symbol=(src[2] if src else nm), kind="import_thunk",
            section_address=f"{t:08x}", section_length=6,
            relocations=f"+2:DIR32:__imp_{src[2] if src else nm}=0x{slot:08X}[ok] ({dll}!{nm})", reloc_ok=1, reloc_bad=0,
            retail_copies=1, confidence="high", catalog=catalog_bucket(t, landed, known),
            catalog_name=landed.get(t, (known.get(t, ""),))[0], parked="yes" if t in parked else "",
            comp_id="", notes=f"import {dll}!{nm}" + ("" if src else "; import library not on disk")))
    log.write(f"ambiguous (several retail copies, no relocation proof): {len(ambiguous)}\n")
    for va, n, nh, pv, why in sorted(ambiguous, key=lambda x: x[0]):
        log.write(f"  ambiguous 0x{va:08X} {n} copies={nh} {pv['tag']}/{pv['lib']}({pv['member']}) {pv['syms'][:1]} {why}\n")
    rows.sort(key=lambda r: int(r["address"], 16))
    # one retail address = one library symbol (a thunk and a section never share an address; keep the first)
    seen, final = set(), []
    for r in rows:
        if r["address"] in seen:
            continue
        seen.add(r["address"])
        final.append(r)
    cols = ["address", "length", "lib", "member", "symbol", "kind", "confidence", "catalog", "catalog_name", "parked",
            "section_address", "section_length", "retail_copies", "reloc_ok", "reloc_bad", "relocations", "comp_id", "notes"]
    out = Path(a.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    with open(out, "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=cols, delimiter="\t", lineterminator="\n")
        w.writeheader()
        for r in final:
            w.writerow({c: r.get(c, "") for c in cols})
    summ = defaultdict(int)
    for r in final:
        summ[(r["kind"], r["catalog"], r["confidence"])] += 1
    print(f"wrote {out}: {len(final)} rows")
    for k in sorted(summ):
        print(f"  {k[0]:13} {k[1]:9} {k[2]:7} {summ[k]}")
    log.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
