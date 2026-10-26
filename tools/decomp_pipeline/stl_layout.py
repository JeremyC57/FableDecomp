#!/usr/bin/env python3
"""Retail (STLport 4.6 / VC7.1) re-layout of Ego_r donor structs (used by gen_class_headers.py).

Donor = ghidra_out/struct_layouts_egor.tsv (Dinkumware containers). Retail container sizes:
  list 4 (0xC), vector 0xC (0x10), vector<bool> 0xC (0x14: retail is a byte vector), map/set/multimap/multiset 0xC (0x10),
  basic_string 0xC (0x1C), deque/queue/stack 0x28 (0x18), CVectorMap 0x10 (0x14), CArray 0xC (0x10),
  deque iterator 0x10 (0xC), bitset<N> 4*ceil(N/32) (8*ceil(N/64): Ego_r uses 64-bit words),
  CDefPointer 4, CIntelligentPointer/CCountedPointer 8 (align 4); everything else recursively.
Bases: retail RTTI ClassHierarchyDescriptors (ghidra_out/rtti_bases.tsv, written by
tools/decomp_pipeline/rtti_bases.py from Fable.exe).

Each struct is laid out twice with one algorithm: pass D (donor sizes) must reproduce the donor
offsets (model check), pass N (retail sizes) gives the corrected offsets.
"""
from __future__ import annotations
import re, sys, collections
from dataclasses import dataclass, field
from pathlib import Path


def _last(n: str) -> str:
    """last top-level '::' component of a (possibly templated) name"""
    d = cut = i = 0
    while i < len(n):
        c = n[i]
        if c == "<": d += 1
        elif c == ">": d -= 1
        elif c == ":" and d == 0 and n[i:i + 2] == "::": cut = i + 2; i += 1
        i += 1
    return n[cut:]


def _norm(n: str) -> str:
    return re.sub(r"\b(class|struct)\b|[ _]", "", n)


SCAL = {"int": 4, "uint": 4, "long": 4, "ulong": 4, "short": 2, "ushort": 2, "char": 1, "uchar": 1,
        "schar": 1, "byte": 1, "bool": 1, "float": 4, "double": 8, "ulong64": 8, "long64": 8,
        "longlong": 8, "ulonglong": 8, "DWORD": 4, "WORD": 2, "BYTE": 1, "wchar_t": 2,
        "undefined4": 4, "undefined2": 2, "undefined1": 1, "undefined": 1, "pointer": 4,
        "void *": 4, "HWND": 4, "HANDLE": 4, "dword": 4, "word": 2, "qword": 8, "LONG": 4,
        "UINT": 4, "SIZE_T": 4, "CHAR": 1, "ULONG_PTR": 4, "ptrdiff_t": 4, "__time32_t": 4,
        "__ehstate_t": 4, "ImageBaseOffset32": 4, "LPCWSTR": 4, "LPWSTR": 4, "HICON": 4,
        "unsigned___int64": 8, "__int64": 8, "ULONG": 4, "USHORT": 2, "UCHAR": 1, "INT": 4,
        "BOOL": 4, "HRESULT": 4, "LPVOID": 4, "LPSTR": 4, "LPCSTR": 4, "HINSTANCE": 4,
        "HMODULE": 4, "HDC": 4, "HKEY": 4, "HBITMAP": 4, "HFONT": 4, "HMENU": 4, "HCURSOR": 4,
        "HBRUSH": 4, "HGLOBAL": 4, "WPARAM": 4, "LPARAM": 4, "LRESULT": 4, "time_t": 4,
        "size_t": 4, "ushort16": 2}
# 8-byte types whose alignment is 4 (two dwords) vs 8
KNOWN = {"_LARGE_INTEGER": (8, 8), "_ULARGE_INTEGER": (8, 8), "LARGE_INTEGER": (8, 8),
         "ULARGE_INTEGER": (8, 8), "FILETIME": (8, 4), "_FILETIME": (8, 4)}

# head -> (donor size, retail size)
LEAF = {"list": (0xC, 4), "map": (0x10, 0xC), "set": (0x10, 0xC), "multimap": (0x10, 0xC),
        "multiset": (0x10, 0xC), "basic_string": (0x1C, 0xC), "deque": (0x18, 0x28),
        "CVectorMap": (0x14, 0x10), "CArray": (0x10, 0xC), "vector": (0x10, 0xC),
        "_Deque_iterator": (0xC, 0x10), "_Deque_const_iterator": (0xC, 0x10)}
# Retail size overrides where the TLC layout drifted from Ego_r past a verified prefix:
# class -> (retail sizeof, end of the verified prefix in retail coordinates, evidence)
OVERRIDES = {}


def apply_override(L, rsize, prefix_end, ev):
    keep = [e for e in L.elems if e.noff + max(e.nsize, 0) <= prefix_end]
    t = Elem(-1, -1, rsize - prefix_end, 1, "retail_tail", "tail", note=ev)
    t.noff = prefix_end
    dend = max((e.doff + e.dsize for e in keep), default=0)
    t.doff, t.dsize = dend, L.dsize - dend
    L2 = Lay(L.name, L.dsize, rsize, L.align, keep + [t], set(L.flags) | {"retail_override"})
    L2.changed = True
    return L2


# Ego_r members absent from the retail (TLC) object, with the landed evidence that proves it
DROP = {
    "CBankFile": (["Dependencies", "CurrentEntryDependencies"],
                  "landed CBankFile::ClearEntries 0x009CCC00 model (every member to WarningFlag +0xF4 at the "
                  "corrected offsets); CBankFileAsync ctor 0x009D5F80 base boundary 0x110 = ChecksumCache "
                  "+0xF8 + 0x18; RTTI mdisp CResourceBank@0x164 (= 0x110 + CBankFileAsync's 0x54)"),
    "CLipSyncDataBank": (["SpeechAnalyser"], "landed ctor 0x00A42B80: Entries +0x164, ResourceBank +0x170, "
                         "SkipUpdate +0x178, PhonemeBank +0x17C, sizeof 0x180 (= operator new 0x180 at 0x00991B09)"),
}


# Ego_r members whose retail (TLC) size differs for a non-container reason, with the evidence that proves it
RESIZE = {
    "CEngineLandscapeRenderer": ({"MeshBuilder": 0x484},
        "operator new 0x6B4 (newsize.tsv); landed models put SortedSubPatchList at +0x5E4 (0x00B677A0), the shadowed "
        "vertex-shader pointers at +0x544/+0x548/+0x54C (0x00BF5CA0/0x00BF5CD0/0x00BF5D00), Settings flags at "
        "+0x610/+0x614/+0x63C (0x00B6B0B0/0x00B670E0/0x00B68610) and MemoryManager at +0x6B0 (0x00BE5D80): every "
        "member after MeshBuilder sits 0x83C below the STLport layout"),
}


# non-polymorphic hierarchies (no retail RTTI): base named by the donor's leading storage run
# matching the base record's size exactly (CEditWorldMap's first own member is at +0xfc = sizeof CWorldMap)
BASE_BY_NAME = {"CEditWorldMap": "CWorldMap"}
STD_NS = re.compile(r"^(?:std::|_STL::)")


def split_args(s: str) -> tuple[str, list[str]]:
    i = s.find("<")
    if i < 0 or not s.rstrip().endswith(">"):
        return s.strip(), []
    head = s[:i].strip(); body = s[i + 1:s.rstrip().rfind(">")]
    out, d, cur = [], 0, ""
    for ch in body:
        if ch == "<": d += 1
        elif ch == ">": d -= 1
        if ch == "," and d == 0:
            out.append(cur.strip(" _")); cur = ""
        else:
            cur += ch
    if cur.strip(" _"): out.append(cur.strip(" _"))
    return head, out


@dataclass
class Elem:
    doff: int                 # donor offset
    dsize: int
    nsize: int
    align: int
    label: str
    kind: str                 # member / base / vptr / gap / union
    noff: int = -1
    names: list = field(default_factory=list)
    note: str = ""


@dataclass
class Lay:
    name: str
    dsize: int
    nsize: int
    align: int
    elems: list
    flags: set = field(default_factory=set)
    changed: bool = False


class Engine:
    def __init__(self, donor_path: Path, rtti_path: Path | None):
        self.structs: dict[str, tuple[int, list]] = {}
        cur = None; tmp = {}; self.alts = {}
        for line in open(donor_path, encoding="utf-8", errors="ignore"):
            p = line.rstrip("\n").split("\t")
            if p[0] == "@STRUCT":
                cur = [p[1], int(p[2]), []]
                tmp.setdefault(p[1], []).append(cur)
                continue
            if cur is None or len(p) < 5:
                continue
            cur[2].append((int(p[2]), p[3].strip(), p[4]))
        # same dedup rule as gen_class_headers.load_donor: first, replaced by one with more members
        for n, lst in tmp.items():
            best = lst[0]
            for c in lst[1:]:
                if len(c[2]) > len(best[2]): best = c
            self.structs[n] = (best[1], sorted(best[2], key=lambda m: m[0]))
            # distinct records sharing an unqualified name (NCameraModes::CBase vs
            # NHeroInformationScreens::CBase): keep each size as `name#0xSIZE`
            sizes = {}
            for c in lst:
                if c[1] not in sizes or len(c[2]) > len(sizes[c[1]][2]): sizes[c[1]] = c
            if len(sizes) > 1:
                self.alts[n] = []
                for sz, c in sizes.items():
                    k = f"{n}#0x{sz:x}"
                    self.structs[k] = (sz, sorted(c[2], key=lambda m: m[0]))
                    self.alts[n].append(k)
            if len({c[1] for c in lst}) > 1:
                self.dupsize = getattr(self, "dupsize", set()); self.dupsize.add(n)
        self.bases: dict[str, list[tuple[str, int]]] = {}
        self.rtti_short = collections.defaultdict(list)
        self.rtti_norm: dict[str, list[str]] = {}
        # donor names normalised (spaces / underscores / class-struct keywords dropped) for template lookups:
        # RTTI spells `CIntelligentPointer<CThing const >`, Ego_r `CIntelligentPointer<CThing_const_>`
        self.snorm: dict[str, list[str]] = {}
        for k in self.structs:
            if "#" in k: continue
            self.snorm.setdefault(_norm(k), []).append(k)
            if _last(k) != k: self.snorm.setdefault(_norm(_last(k)), []).append(k)
        if rtti_path:
            for line in open(rtti_path).read().splitlines()[1:]:
                td, cls, nb, direct, _ = line.split("\t")
                bl = []
                for b in direct.split(";") if direct else []:
                    nm, md = b.rsplit("@", 1)
                    bl.append((nm, int(md.split(",")[0])))
                self.bases[cls] = bl
                self.rtti_short[_last(cls)].append(cls)
                self.rtti_norm.setdefault(_norm(_last(cls)), []).append(cls)
        self.memo: dict[str, Lay] = {}
        self.tmemo: dict[str, tuple] = {}
        self.stack: set[str] = set()

    # ---------------------------------------------------------------- bases
    def rtti_bases(self, name: str):
        name = name.split("#")[0]
        if name in self.bases:
            return self.bases[name], name
        c = self.rtti_short.get(_last(name), [])
        if not c and "<" in name:
            c = self.rtti_norm.get(_norm(_last(name)), [])
        if len(c) == 1:
            return self.bases[c[0]], c[0]
        if len(c) > 1:
            return None, "AMBIG"
        return None, None

    def struct_key(self, nm: str, want: int | None = None) -> str | None:
        k = nm if nm in self.structs else (_last(nm) if _last(nm) in self.structs else None)
        if k is None and "<" in nm:
            c = self.snorm.get(_norm(nm)) or self.snorm.get(_norm(_last(nm)))
            if c and len(set(c)) == 1: k = c[0]
        if k is not None and want is not None and k in self.alts:
            for a in self.alts[k]:
                if self.structs[a][0] == want: return a
        return k

    # ---------------------------------------------------------------- types
    def tinfo(self, typ: str, span: int | None = None):
        """-> (dsize, nsize, align, flags:set)"""
        t = typ.strip()
        key = (t, span if t not in self.structs else None)
        if key in self.tmemo: return self.tmemo[key]
        r = self._tinfo(t, span)
        self.tmemo[key] = r
        return r

    def _tinfo(self, t: str, span):
        if t.endswith("*") or t.endswith("&"):
            return 4, 4, 4, set()
        m = re.match(r"^(.*?):(\d+)$", t)
        if m and "<" not in t[m.start(2) - 1:]:
            d, n, a, f = self.tinfo(m.group(1))
            return d, n, a, f | {"bitfield"}
        m = re.match(r"^(.*)\[(\d+)\]$", t)
        if m:
            d, n, a, f = self.tinfo(m.group(1))
            k = int(m.group(2))
            return d * k, n * k, a, f
        if t in SCAL:
            s = SCAL[t]; return s, s, min(s, 8), set()
        if t in KNOWN:
            s, a = KNOWN[t]; return s, s, a, set()
        t2 = STD_NS.sub("", t)
        head, args = split_args(t2)
        if head in ("vector",) and args and args[0] == "bool":
            # retail STLport vector<bool> is an ordinary byte vector (12 bytes): landed CList
            # +0x1c8 -> next member +0x1d4 (sizeof 0x1fc = operator new at 0x0041D4A1),
            # CPlayerInventoryDef GetSizeofClass 0x70, CTCShopKeeper/CPlayerInventoryDef models
            return self._check(t, 0x14, 0xC, "vector<bool>")
        if head == "CArray" and args and args[0] == "bool":
            return self._check(t, 0x14, 0xC, "CArray<bool>")
        if head in ("CDefPointer", "scoped_ptr", "auto_ptr"):
            return 4, 4, 4, set()
        if head in ("CIntelligentPointer", "CCountedPointer"):
            return 8, 8, 4, set()
        if head == "bitset" and args and args[0].isdigit():
            # STLport bitset<N> stores ceil(N/32) unsigned longs; Ego_r's Dinkumware uses 64-bit
            # words (bitset<274> = ulong64[5] = 0x28 vs retail 9 dwords = 0x24: landed CThing
            # models m_presentBits[9] +0x20, interfaces +0x44, ThingTypeFlags +0x6c, Dead +0x91)
            nb = int(args[0]); n = max(1, (nb + 31) // 32) * 4
            sk = self.struct_key(t)
            d = self.structs[sk][0] if sk else (max(1, (nb + 63) // 64) * 8 if nb > 32 else 4)
            return d, n, 4, ({"stl"} if d != n else set())
        if head in LEAF:
            d, n = LEAF[head]
            return self._check(t, d, n, head)
        if head in ("queue", "stack"):
            seq = args[1] if len(args) > 1 else "deque<" + (args[0] if args else "") + ">"
            d, n, a, f = self.tinfo(seq)
            return self._check(t, d, n, head, f)
        if head == "priority_queue":
            seq = args[1] if len(args) > 1 else "vector<x>"
            d, n, a, f = self.tinfo(seq)
            return self._check(t, d + 4, n + 4, head, f)
        if head == "reverse_iterator" and args:
            d, n, a, f = self.tinfo(args[0])
            return d, n, a, f
        sk = self.struct_key(t, span)
        if head == "pair" and len(args) == 2 and (sk is None or not self.structs[sk][1]):
            L = self.layout_pair(t, args)
            if sk is not None and L.dsize != self.structs[sk][0]:
                ds = self.structs[sk][0]
                return ds, ds, 4, {f"REFUSE:pair:donor0x{ds:x}!=0x{L.dsize:x}"}
            return L.dsize, L.nsize, L.align, set(L.flags)
        if sk is not None and not self.structs[sk][1] and self.structs[sk][0] == 1 and span and span > 1 \
                and head not in ("allocator", "less", "greater"):
            # empty-looking record used with a wider slot (tWAVEFORMATEX): keep the donor slot
            return span, span, (4 if span % 4 == 0 else 1), {"unknown:" + t}
        if sk is not None:
            if not self.structs[sk][1] and self.structs[sk][0] >= 0xC and sk not in self.bases \
                    and not self.rtti_bases(sk)[0]:
                ds = self.structs[sk][0]
                return ds, ds, (4 if ds % 4 == 0 else 1), {f"opaque:{sk}"}
            L = self.layout(sk)
            if L is not None:
                return L.dsize, L.nsize, L.align, set(L.flags)
        if head == "pair" and len(args) == 2:
            L = self.layout_pair(t, args)
            return L.dsize, L.nsize, L.align, set(L.flags)
        # unknown (enum, typedef): natural size unknown -> donor span; dword alignment
        if span is None:
            return 4, 4, 4, {"unknown:" + t}
        a = 4 if span % 4 == 0 else (2 if span % 2 == 0 else 1)
        return span, span, min(a, 4), {"unknown:" + t}

    def _check(self, t, d, n, head, f=frozenset()):
        sk = self.struct_key(t)
        if sk is not None and self.structs[sk][0] not in (d, 0, 1):
            # stateful comparator/allocator or an unexpected instantiation: refuse
            ds = self.structs[sk][0]
            return ds, ds, 4, set(f) | {f"REFUSE:{head}:donor0x{ds:x}!=0x{d:x}"}
        return d, n, 4, set(f) | ({"stl"} if d != n else set())

    # ---------------------------------------------------------------- layout
    def layout_pair(self, t, args):
        mem = []
        d1, n1, a1, f1 = self.tinfo(args[0]); d2, n2, a2, f2 = self.tinfo(args[1])
        o2 = (d1 + a2 - 1) // a2 * a2
        mem = [(0, args[0], "first"), (o2, args[1], "second")]
        al = max(a1, a2)
        size = (o2 + d2 + al - 1) // al * al
        return self._layout(t, size, mem)

    def layout(self, name: str) -> Lay | None:
        if name in self.memo: return self.memo[name]
        if name in self.stack:
            return None
        if name not in self.structs:
            return None
        self.stack.add(name)
        size, mem = self.structs[name]
        L = self._layout(name, size, mem)
        if name in DROP: L.flags.add("retail_drop:" + ",".join(DROP[name][0]))
        if name in RESIZE: L.flags.add("retail_resize:" + ",".join(RESIZE[name][0]))
        if any(f.startswith("member_in_base") for f in L.flags):
            # the RTTI class of this name is a different type than this donor record
            # (NTCScriptedControl::CScriptConversation is 8 bytes; the 0x34 record is another)
            L = self._layout(name, size, mem, no_bases=True)
            L.flags.add("rtti_not_this_record")
        if size > 0 and not mem and any(e.kind == "base" and e.doff + e.dsize > size for e in L.elems):
            # memberless donor record (CGameDefinitionManager: size 1): nothing to re-lay-out
            L = Lay(name, size, size, 1, [], {"donor_memberless_skip"})
        if name in OVERRIDES:
            L = apply_override(L, *OVERRIDES[name])
        self.stack.discard(name)
        self.memo[name] = L
        return L

    def _layout(self, name, size, mem, no_bases=False) -> Lay:
        flags: set = set()
        elems: list[Elem] = []
        # ---- bases from retail RTTI
        bl, rname = self.rtti_bases(name) if not no_bases else (None, None)
        fo0 = min([o for o, t, n in mem if n != "_padding_"] + [size])
        if rname == "AMBIG":
            # several RTTI classes share the unqualified name: keep the one whose bases' donor
            # storage ends exactly where this record's own members start
            fits = []
            for cand in self.rtti_short.get(_last(name.split("#")[0]), []):
                b2 = sorted(self.bases[cand], key=lambda b: b[1]); cur = 4 if b2 and b2[0][1] > 0 else 0
                ok = True
                for bn, md in b2:
                    k = self.struct_key(bn)
                    B = self.layout(k) if k else None
                    if B is None: ok = False; break
                    cur = (cur + B.align - 1) // B.align * B.align + B.dsize
                if ok and b2 and (cur == fo0 or (fo0 - cur) in (1, 2, 3)):
                    fits.append(cand)
                elif ok and not b2 and fo0 <= 4:
                    fits.append(cand)
            if len(fits) == 1:
                bl = self.bases[fits[0]]; flags.add("rtti_fit:" + fits[0])
            else:
                flags.add("rtti_ambiguous")
        if bl is None and fo0 > 4 and name in BASE_BY_NAME:
            bl = [(BASE_BY_NAME[name], 0)]; flags.add("base_by_name:" + BASE_BY_NAME[name])
        base_end_d = 0
        if bl:
            bl = sorted(bl, key=lambda b: b[1])
            if bl[0][1] > 0:
                elems.append(Elem(0, 4, 4, 4, "vptr", "vptr"))
            # pick, per base, the donor record (when one unqualified name has several) whose
            # sizes make the donor base storage end where the derived class's own members start
            fo = min([o for o, t, n in mem if n != "_padding_"] + [size])
            cands = []
            for bn, md in bl:
                k = self.struct_key(bn)
                cands.append(self.alts.get(k, [k]) if k else [None])
            pick = [c[0] if len(c) == 1 else self.struct_key(bn) for c, (bn, md) in zip(cands, bl)]
            if any(len(c) > 1 for c in cands):
                import itertools
                best = None
                for combo in itertools.islice(itertools.product(*cands), 256):
                    cur = 4 if bl[0][1] > 0 else 0; ok = True; mds = True
                    for k, (bn, md) in zip(combo, bl):
                        if k is None: ok = False; break
                        B = self.layout(k)
                        if B is None: ok = False; break
                        cur = (cur + B.align - 1) // B.align * B.align + B.dsize
                    if not ok: continue
                    score = (cur <= fo, -(fo - cur) if cur <= fo else -999999)
                    if best is None or score > best[0]: best = (score, combo)
                if best: pick = list(best[1]); flags.add("base_alt_pick:" + ",".join(p for p in pick if p and "#" in p))
            for (bn, md), sk in zip(bl, pick):
                if sk is None:
                    flags.add(f"base_no_donor:{bn}")
                    elems.append(Elem(-1, -1, -1, 4, bn, "base", note=f"mdisp {md}"))
                    continue
                B = self.layout(sk)
                if B is None:
                    flags.add(f"base_cycle:{bn}"); continue
                flags |= {f for f in B.flags if f.startswith(("REFUSE", "base_", "union_", "mdisp", "rtti"))}
                e = Elem(-1, B.dsize, B.nsize, B.align, bn, "base", note=f"retail mdisp 0x{md:x}")
                e.rmdisp = md; e.key = sk
                elems.append(e)
        # ---- members
        realm = [(o, t, n) for (o, t, n) in mem]
        # donor offset of base storage: sequential with donor sizes
        cur = 0
        for e in elems:
            if e.kind == "vptr": cur = 4; e.doff = 0; continue
            if e.dsize < 0:
                continue
            cur = (cur + e.align - 1) // e.align * e.align
            if e.dsize == 1 and not self.structs.get(self.struct_key(e.label) or "", (0, [1]))[1]:
                # empty base: EBO at the start (MSVC single inheritance)
                e.dsize = e.nsize = 0
            e.doff = cur; cur += e.dsize
        base_end_d = cur
        if any(e.dsize < 0 for e in elems):
            # unknown base size: everything before the first named member is opaque base storage
            first = min((o for o, t, n in realm if n != "_padding_"), default=size)
            elems = [Elem(0, first, first, 4, "bases?", "gap", note="base without donor record")]
            base_end_d = first
            flags.add("base_unsized")
        for o, t, n in realm:
            if o < base_end_d:
                if n == "_padding_":
                    continue
                flags.add(f"member_in_base:{n}@{o:#x}")
                continue
        mem2 = [(o, t, n) for o, t, n in realm if o >= base_end_d]
        if bl is None and mem2 and mem2[0][2] == "_padding_" and mem2[0][0] == 0:
            flags.add("nortti_leading_padding")
        # spans for unknown types
        offs = sorted({o for o, _, _ in mem2}) + [size]
        nxt = {o: offs[i + 1] for i, o in enumerate(offs[:-1])}
        # group union alternatives (same offset or starting inside a previous one)
        groups: list[list] = []
        gend = -1
        for o, t, n in mem2:
            sp = nxt[o] - o
            d, nn, a, f = self.tinfo(t, sp)
            if n == "_padding_":
                f = set(f) | {"padrow"}
            if groups and o < gend:
                groups[-1].append((o, t, n, d, nn, a, f)); gend = max(gend, o + d); continue
            groups.append([(o, t, n, d, nn, a, f)]); gend = o + d
        for g in groups:
            o0 = g[0][0]
            dsz = max(x[0] - o0 + x[3] for x in g)
            nsz = max(x[0] - o0 + x[4] for x in g)
            al = max(x[5] for x in g)
            ff = set().union(*[x[6] for x in g])
            if len(g) > 1 and any(x[3] != x[4] for x in g):
                ff.add("union_changes")
            if len(g) > 1 and any(x[0] != o0 for x in g) and any(x[3] != x[4] for x in g):
                ff.add("union_inner_offsets_change")
            if g[0][2] in DROP.get(name, ((), ""))[0]:
                nsz = 0; al = 1
            if len(g) == 1 and g[0][2] in RESIZE.get(name, ({}, ""))[0]:
                nsz = RESIZE[name][0][g[0][2]]; ff = set(ff) | {"retail_resize"}
            e = Elem(o0, dsz, nsz, al, g[0][2], "union" if len(g) > 1 else "member")
            e.names = [x[2] for x in g]; e.types = [x[1] for x in g]; e.mflags = ff
            elems.append(e)
            for f in ff:
                if f.startswith(("REFUSE", "unknown")): flags.add(f)
                if f in ("union_changes", "union_inner_offsets_change"): flags.add(f + ":" + g[0][2])
        # ---- two passes
        Lal = max([e.align for e in elems if e.dsize > 0] + [1])
        Lal = min(Lal, 8)
        dcur = ncur = 0
        out: list[Elem] = []
        packed = False
        for e in elems:
            if e.dsize < 0: continue
            if e.kind in ("vptr",):
                e.noff = 0; dcur = ncur = 4; out.append(e); continue
            if e.dsize == 0 or e.nsize == 0:
                e.noff = ncur; out.append(e)
                if e.dsize: dcur = e.doff + e.dsize
                continue
            want_d = (dcur + e.align - 1) // e.align * e.align
            if e.kind == "base":
                want_n = (ncur + e.align - 1) // e.align * e.align
                e.noff = want_n
                if hasattr(e, "rmdisp") and e.rmdisp != want_n:
                    flags.add(f"mdisp_mismatch:{e.label}:0x{e.rmdisp:x}!=0x{want_n:x}")
                dcur = e.doff + e.dsize; ncur = want_n + e.nsize; out.append(e); continue
            if e.doff > want_d:
                gap = e.doff - dcur
                g = Elem(dcur, gap, gap, 1, f"gap@{dcur:#x}", "gap"); g.noff = ncur
                out.append(g)
                if gap >= 4: flags.add(f"gap:{dcur:#x}+{gap:#x}")
                ncur += gap; dcur = e.doff
                want_n = ncur if ncur % e.align == 0 else (ncur + e.align - 1) // e.align * e.align
                if want_n != ncur: flags.add(f"gap_align:{e.label}")
            elif e.doff < want_d:
                # packed member (pack(1)/(2)/(4) or a smaller natural alignment than modelled)
                packed = True
                want_n = ncur + (e.doff - dcur) if e.doff >= dcur else ncur
                if e.doff < dcur: flags.add(f"overlap:{e.label}")
            else:
                want_n = (ncur + e.align - 1) // e.align * e.align
                if packed and e.doff == dcur:
                    want_n = ncur
            e.noff = want_n
            dcur = e.doff + e.dsize; ncur = want_n + e.nsize
            out.append(e)
        if packed: flags.add("packed_or_underaligned")
        dend = (dcur + Lal - 1) // Lal * Lal
        if size > dend:
            tail = size - dcur
            # unknown trailing storage (memberless records: vptr-only bases, iterators, opaque
            # wrappers): assume dword alignment like the pointer/int it most likely holds
            if tail >= 4 and Lal < 4 and size % 4 == 0 and not packed:
                Lal = 4
            # unlisted trailing storage: keep its bytes
            nsize = ncur + tail
            nsize = (nsize + Lal - 1) // Lal * Lal if (ncur + tail) % Lal else nsize
            if size - dend >= 4: flags.add(f"tail:{dcur:#x}+{tail:#x}")
        elif size < dend:
            # packed tail (no trailing alignment in donor)
            nsize = ncur + (size - dcur)
            flags.add("tail_short")
        else:
            nsize = (ncur + Lal - 1) // Lal * Lal
        if size == 0: nsize = 0
        L = Lay(name, size, nsize, Lal, out, flags)
        L.changed = nsize != size or any(e.noff != e.doff or e.nsize != e.dsize for e in out)
        return L


def offset_map(L: Lay):
    """[(donor_lo, donor_hi, delta, elem)] for remapping header members inside each element."""
    return [(e.doff, e.doff + e.dsize, e.noff - e.doff, e) for e in L.elems]


def _cli():
    E = Engine(Path(sys.argv[1]), Path(sys.argv[2]))
    want = sys.argv[3:]
    for n in want:
        L = E.layout(n)
        print(f"{n}: donor 0x{L.dsize:x} -> retail 0x{L.nsize:x} align {L.align} flags {sorted(L.flags)}")
        for e in L.elems:
            print(f"   d+0x{e.doff:03x} n+0x{e.noff:03x} {e.dsize:#x}->{e.nsize:#x} a{e.align} {e.kind} {e.label} {getattr(e,'types',[''])[0][:80]} {e.note}")



def atoms(E: Engine, L: Lay, dbase=0, nbase=0, path=""):
    """Flatten a corrected layout into mapping atoms:
       ('shift', d_lo, d_hi, n_lo, label)        bytes move rigidly by n_lo-d_lo
       ('resize', d_lo, d_hi, n_lo, n_hi, label)  container / changed union: no inner map
    Bases and composite members whose own layout changed are recursed into."""
    out = []
    for e in L.elems:
        if e.dsize <= 0 and e.kind != "vptr":
            continue
        d0, n0 = dbase + e.doff, nbase + e.noff
        lab = path + e.label
        if e.dsize > 0 and e.nsize == 0:
            out.append(("drop", d0, d0 + e.dsize, n0, n0, lab)); continue
        if e.dsize == e.nsize and not _inner_changed(E, e):
            out.append(("shift", d0, d0 + e.dsize, n0, lab)); continue
        sub = _sub_layout(E, e)
        if sub is not None:
            L2, count = sub
            for k in range(count):
                out += atoms(E, L2, d0 + k * L2.dsize, n0 + k * L2.nsize, lab + (f"[{k}]" if count > 1 else "") + ".")
            continue
        out.append(("resize", d0, d0 + e.dsize, n0, n0 + e.nsize, lab))
    return out


def _sub_layout(E, e):
    if e.kind == "base":
        sk = getattr(e, "key", None) or E.struct_key(e.label)
        return (E.layout(sk), 1) if sk else None
    if e.kind != "member":
        return None
    t = e.types[0].strip()
    m = re.match(r"^(.*)\[(\d+)\]$", t)
    cnt = 1
    if m:
        t, cnt = m.group(1).strip(), int(m.group(2))
    if re.match(r"^(.*)\[(\d+)\]$", t):
        return None
    sk = E.struct_key(t, e.dsize // cnt if cnt else None)
    if sk is None or not E.structs[sk][1]:
        return None
    L2 = E.layout(sk)
    if L2 is None or not L2.changed or L2.dsize * cnt != e.dsize:
        return None
    return L2, cnt


def _inner_changed(E, e):
    s = _sub_layout(E, e)
    return bool(s and s[0].changed)


def map_off(ats, o):
    """donor offset -> (new offset, atom) or (None, atom/None)"""
    for a in ats:
        if a[1] <= o < a[2]:
            if a[0] == "shift":
                return o - a[1] + a[3], a
            return (a[3] if o == a[1] else None), a
    return None, None


def remap_emitted(E: Engine, cls: str, emitted, mk):
    """Generator hook: move a donor-coordinate member list (gen_class_headers.Emitted, before
    retail reconciliation) onto the retail layout. `mk(off, size, ctype, name, suffix, kind,
    comment, is_pad)` builds an Emitted. Returns (emitted, retail_size, notes) or None when the
    class keeps its donor layout."""
    L = E.layout(cls)
    if L is None or not L.changed:
        return None
    ats = atoms(E, L)
    tops = [e for e in L.elems if e.dsize > 0]
    top = {}
    for k, e in enumerate(tops):
        if e.kind not in ("member", "union"): continue
        dn = tops[k + 1].doff if k + 1 < len(tops) else L.dsize
        nn = tops[k + 1].noff if k + 1 < len(tops) else L.nsize
        top[e.doff] = (e, dn - e.doff, nn - e.noff)
    def shift_atom(o, sz):
        for a in ats:
            if a[0] == "shift" and a[1] <= o and o + sz <= a[2]:
                return a
        return None
    out = []; notes = []; src = {}
    for m in emitted:
        if any(a[0] == "drop" and a[1] <= m.off and m.off + m.size <= a[2] for a in ats):
            notes.append("dropped " + m.name); continue
        if m.kind == "vptr" and m.off == 0:
            out.append(m); continue
        t = top.get(m.off)
        if t is not None and not m.is_pad:
            e, dspan, nspan = t
            if m.size == e.dsize and e.nsize == e.dsize:
                out.append(mk(e.noff, m.size, m.ctype, m.name, m.suffix, m.kind, m.comment, False)); continue
            if m.ctype == "unsigned char" and m.suffix.startswith("[0x"):
                ns = e.nsize if m.size == e.dsize else (nspan if m.size == dspan else None)
                if ns is not None:
                    out.append(mk(e.noff, ns, m.ctype, m.name, f"[0x{ns:x}]", m.kind, m.comment, False)); continue
        a = shift_atom(m.off, m.size)
        if a is not None:
            no = m.off - a[1] + a[3]
            nm = m.name if not m.is_pad or no == m.off else re.sub(r"0x[0-9a-f]+$", f"0x{no:02x}", m.name)
            out.append(mk(no, m.size, m.ctype, nm, m.suffix, m.kind, m.comment, m.is_pad)); continue
        if m.is_pad:
            lo, hi = m.off, m.off + m.size
            pre = (re.match(r"(_[a-z]+_)0x", m.name) or re.match(r"(_pad_)", "_pad_")).group(1)
            first = True
            for a in ats:
                s0, s1 = max(lo, a[1]), min(hi, a[2])
                if s0 < s1 and a[0] == "shift":
                    no = s0 - a[1] + a[3]
                    out.append(mk(no, s1 - s0, "unsigned char", f"{pre if first else '_pad_'}0x{no:02x}",
                                  f"[0x{s1 - s0:x}]", m.kind, m.comment if first else "", True))
                    first = False
                    src[id(out[-1])] = id(m)
            continue
        notes.append(f"unmapped {m.name}@{m.off:#x}")
        return None
    out.sort(key=lambda x: x.off)
    res = []; cur = 0
    for m in out:
        if m.off < cur:
            return None
        if m.off > cur:
            res.append(mk(cur, m.off - cur, "unsigned char", f"_pad_0x{cur:02x}", f"[0x{m.off - cur:x}]", "pad", "", True))
        res.append(m); cur = m.off + m.size
    if cur < L.nsize:
        res.append(mk(cur, L.nsize - cur, "unsigned char", f"_pad_0x{cur:02x}", f"[0x{L.nsize - cur:x}]", "pad", "", True))
    # re-join the pieces of one donor pad that the shift atoms split (pad granularity unchanged)
    merged = []; last = None
    for m in res:
        s_ = src.get(id(m))
        p = merged[-1] if merged else None
        if p is not None and s_ is not None and s_ == last and p.off + p.size == m.off:
            n = p.size + m.size
            merged[-1] = mk(p.off, n, "unsigned char", p.name, f"[0x{n:x}]", p.kind, p.comment, True)
        else:
            merged.append(m)
        last = s_
    return merged, L.nsize, notes


if __name__ == "__main__":
    _cli()
