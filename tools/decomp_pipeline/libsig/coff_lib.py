#!/usr/bin/env python3
"""Read Microsoft COFF archives (.lib) and i386 COFF objects -- stdlib only.

    python3 coff_lib.py <lib> [--members] [--code] [--imports] [--member NAME]

An archive is `!<arch>\\n` followed by members (60-byte header, data padded to an even size):
the first and second linker members (both named `/`), the longnames member `//`, then one
member per object. An object member is either a COFF object (IMAGE_FILE_HEADER, sections,
relocations, symbol table, string table) or a short import record (`00 00 FF FF`, version 0:
the linker synthesises `__imp_X` and, for code imports, the `jmp dword ptr [__imp_X]` thunk).
A record with that signature and version >= 1 is an anonymous object (/GL, bigobj): listed,
not parsed.

What the signature tools use:
  Archive(path).members            -> [Member(name, offset, data)]
  Member.kind                      -> 'coff' | 'import' | 'anon' | 'other'
  Member.coff()                    -> CoffObject (sections, symbols, relocations)
  Member.short_import()            -> ShortImport(symbol, dll, export_name, type, name_type, hint)
  CoffObject.code_sections()       -> [CodeSection(index, name, data, relocs, symbols, comdat)]
Relocation targets are symbol names; `CodeSection.symbols` are the (offset, name, storage
class) defined inside the section, so a matched section gives every symbol's retail address.
"""
from __future__ import annotations

import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_LNK_COMDAT = 0x1000
IMAGE_SCN_MEM_EXECUTE = 0x20000000
# i386 relocation types (winnt.h)
REL_I386 = {0x0000: "ABSOLUTE", 0x0001: "DIR16", 0x0002: "REL16", 0x0006: "DIR32", 0x0007: "DIR32NB",
            0x0009: "SEG12", 0x000A: "SECTION", 0x000B: "SECREL", 0x000C: "TOKEN", 0x000D: "SECREL7",
            0x0014: "REL32"}
# bytes each relocation type patches (the masked span)
REL_SIZE = {0x0006: 4, 0x0007: 4, 0x000B: 4, 0x0014: 4, 0x000C: 4, 0x000A: 2, 0x0001: 2, 0x0002: 2, 0x000D: 1}
IMPORT_TYPES = {0: "CODE", 1: "DATA", 2: "CONST"}
NAME_TYPES = {0: "ORDINAL", 1: "NAME", 2: "NAME_NOPREFIX", 3: "NAME_UNDECORATE"}
STORAGE = {2: "EXTERNAL", 3: "STATIC", 6: "LABEL", 101: "FUNCTION", 103: "FILE", 105: "WEAK_EXTERNAL"}


@dataclass
class Reloc:
    offset: int          # offset inside the section
    type: int            # REL_I386 key
    symbol: str          # target symbol name
    symbol_index: int

    @property
    def type_name(self):
        return REL_I386.get(self.type, f"0x{self.type:x}")

    @property
    def size(self):
        return REL_SIZE.get(self.type, 4)


@dataclass
class Symbol:
    index: int
    name: str
    value: int
    section: int         # 1-based; 0 undefined, -1 absolute, -2 debug
    type: int
    storage: int
    naux: int
    aux: bytes = b""


@dataclass
class CodeSection:
    index: int           # 1-based section number
    name: str
    characteristics: int
    data: bytes
    relocs: list = field(default_factory=list)
    symbols: list = field(default_factory=list)   # [(offset, name, storage)] defined in this section
    comdat: int = 0      # COMDAT selection (0 = not COMDAT)

    @property
    def primary(self):
        """The section's leading external symbol (the COMDAT symbol, else the first external at offset 0)."""
        ext = [s for s in self.symbols if s[2] == 2]
        at0 = [s for s in ext if s[0] == 0]
        return (at0 or ext or self.symbols or [(0, self.name, 0)])[0][1]


class CoffObject:
    def __init__(self, data: bytes, name: str = "?"):
        self.name = name
        self.data = data
        (self.machine, self.nsections, self.timestamp, self.symptr, self.nsyms, self.optsize,
         self.characteristics) = struct.unpack_from("<HHIIIHH", data, 0)
        if self.machine not in (0x14C, 0):
            raise ValueError(f"{name}: machine 0x{self.machine:x} is not i386")
        self._strtab_off = self.symptr + 18 * self.nsyms
        self.sections = []
        base = 20 + self.optsize
        for i in range(self.nsections):
            o = base + 40 * i
            raw = data[o:o + 8].rstrip(b"\0")
            vsize, vaddr, size, ptr, prel, plin, nrel, nlin, ch = struct.unpack_from("<IIIIIIHHI", data, o + 8)
            nm = raw.decode("latin-1")
            if nm.startswith("/") and nm[1:].isdigit():
                nm = self._string(int(nm[1:]))
            self.sections.append(dict(name=nm, size=size, ptr=ptr, prel=prel, nrel=nrel, ch=ch))
        self.symbols = []
        i = 0
        while i < self.nsyms:
            o = self.symptr + 18 * i
            raw = data[o:o + 8]
            value, secnum, typ, storage, naux = struct.unpack_from("<IhHBB", data, o + 8)
            if raw[:4] == b"\0\0\0\0":
                nm = self._string(struct.unpack_from("<I", raw, 4)[0])
            else:
                nm = raw.rstrip(b"\0").decode("latin-1")
            aux = data[o + 18:o + 18 + 18 * naux]
            self.symbols.append(Symbol(i, nm, value, secnum, typ, storage, naux, aux))
            i += 1 + naux
        self._by_index = {s.index: s for s in self.symbols}

    def _string(self, off: int) -> str:
        p = self._strtab_off + off
        e = self.data.index(b"\0", p)
        return self.data[p:e].decode("latin-1")

    def symbol(self, index: int) -> Symbol | None:
        return self._by_index.get(index)

    def section_bytes(self, sec: dict) -> bytes:
        if not sec["ptr"]:
            return b"\0" * sec["size"]       # .bss-like
        return self.data[sec["ptr"]:sec["ptr"] + sec["size"]]

    def relocs(self, sec: dict) -> list:
        out = []
        for k in range(sec["nrel"]):
            off, idx, typ = struct.unpack_from("<IIH", self.data, sec["prel"] + 10 * k)
            s = self.symbol(idx)
            out.append(Reloc(off, typ, s.name if s else f"#{idx}", idx))
        return out

    def comdat_selection(self, secnum: int) -> int:
        for s in self.symbols:
            if s.section == secnum and s.storage == 3 and s.naux and s.name == self.sections[secnum - 1]["name"]:
                if len(s.aux) >= 15:
                    return s.aux[14]
        return 0

    def code_sections(self, min_size: int = 1) -> list:
        out = []
        for n, sec in enumerate(self.sections, 1):
            if not (sec["ch"] & (IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE)) or sec["size"] < min_size:
                continue
            syms = sorted((s.value, s.name, s.storage) for s in self.symbols
                          if s.section == n and s.storage in (2, 3, 6, 101) and s.name != sec["name"]
                          and not s.name.startswith("$"))
            cs = CodeSection(n, sec["name"], sec["ch"], self.section_bytes(sec), self.relocs(sec), syms,
                             self.comdat_selection(n) if sec["ch"] & IMAGE_SCN_LNK_COMDAT else 0)
            out.append(cs)
        return out

    def comp_ids(self) -> list:
        """@comp.id values: (prodid, build) of the compiler/assembler that produced the object."""
        out = []
        for s in self.symbols:
            if s.name == "@comp.id":
                out.append((s.value >> 16, s.value & 0xFFFF))
        return out


@dataclass
class ShortImport:
    symbol: str
    dll: str
    type: int
    name_type: int
    hint: int

    @property
    def type_name(self):
        return IMPORT_TYPES.get(self.type, str(self.type))

    @property
    def export_name(self) -> str:
        """The name the DLL exports (what the import directory records), per the name type."""
        s = self.symbol
        if self.name_type == 0:
            return f"#{self.hint}"
        if self.name_type == 1:
            return s
        if s[:1] in "?@_":
            s = s[1:]
        if self.name_type == 3:
            s = s.split("@", 1)[0]
        return s


@dataclass
class Member:
    name: str
    offset: int          # file offset of the member header
    data: bytes

    @property
    def kind(self) -> str:
        if len(self.data) >= 20 and self.data[:4] == b"\0\0\xff\xff":
            return "import" if struct.unpack_from("<H", self.data, 4)[0] == 0 else "anon"
        if len(self.data) >= 20 and struct.unpack_from("<H", self.data, 0)[0] in (0x14C, 0):
            return "coff"           # machine 0: alias-only objects (weak externals, no code)
        return "other"

    def coff(self) -> CoffObject:
        return CoffObject(self.data, self.name)

    def short_import(self) -> ShortImport:
        _s1, _s2, _ver, _mach, _ts, _size, hint, bits = struct.unpack_from("<HHHHIIHH", self.data, 0)
        rest = self.data[20:]
        sym, dll = rest.split(b"\0")[:2]
        return ShortImport(sym.decode("latin-1"), dll.decode("latin-1"), bits & 3, (bits >> 2) & 7, hint)


class Archive:
    def __init__(self, path):
        self.path = Path(path)
        data = self.path.read_bytes()
        if data[:8] != b"!<arch>\n":
            raise ValueError(f"{path}: not a COFF archive")
        self.members = []
        longnames = b""
        o = 8
        while o + 60 <= len(data):
            hdr = data[o:o + 60]
            if hdr[58:60] != b"`\n":
                raise ValueError(f"{path}: bad member header at 0x{o:x}")
            raw = hdr[:16].decode("latin-1").rstrip()
            size = int(re.match(rb"\s*(\d+)", hdr[48:58]).group(1))   # some import libs pad the field with NULs
            body = data[o + 60:o + 60 + size]
            if raw == "/" or raw == "/               ".rstrip():
                pass                                    # linker members
            elif raw == "//":
                longnames = body
            else:
                if raw.startswith("/") and raw[1:].isdigit():
                    p = int(raw[1:])
                    e = p
                    while e < len(longnames) and longnames[e] not in (0, 10):
                        e += 1
                    nm = longnames[p:e].decode("latin-1").rstrip("/")
                else:
                    nm = raw.rstrip("/")
                self.members.append(Member(nm, o, body))
            o += 60 + size + (size & 1)

    def objects(self):
        for m in self.members:
            if m.kind == "coff":
                try:
                    yield m, m.coff()
                except (ValueError, struct.error, IndexError):
                    continue

    def imports(self):
        for m in self.members:
            if m.kind == "import":
                yield m, m.short_import()


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    lib = Archive(argv[0])
    only = argv[argv.index("--member") + 1] if "--member" in argv else None
    kinds = {}
    for m in lib.members:
        kinds[m.kind] = kinds.get(m.kind, 0) + 1
    print(f"{lib.path.name}: {len(lib.members)} members {kinds}")
    if "--members" in argv:
        for m in lib.members:
            print(f"  {m.kind:6} {len(m.data):8} {m.name}")
    if "--imports" in argv:
        for m, imp in lib.imports():
            print(f"  {imp.dll:16} {imp.type_name:5} {NAME_TYPES.get(imp.name_type)} {imp.symbol} -> {imp.export_name}")
    if "--code" in argv or only:
        for m, obj in lib.objects():
            if only and m.name != only and not m.name.endswith("\\" + only) and not m.name.endswith("/" + only):
                continue
            for cs in obj.code_sections():
                rel = ", ".join(f"+{r.offset:x}:{r.type_name}:{r.symbol}" for r in cs.relocs)
                print(f"  {m.name} sec{cs.index} {cs.name} len={len(cs.data)} comdat={cs.comdat} "
                      f"syms={[(hex(o), n) for o, n, _ in cs.symbols]} relocs=[{rel}] comp={obj.comp_ids()}")
                if only:
                    print("    " + cs.data.hex())
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
