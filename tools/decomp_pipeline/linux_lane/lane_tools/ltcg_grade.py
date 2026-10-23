#!/usr/bin/env python3
"""Grade a candidate under whole-program optimisation: /GL compile, /LTCG link into a DLL, extract the function bytes.

usage: ltcg_grade.py <addr> <src.cpp> <leaf> [--export REGEX] [--internal REGEX] [--flags ...] [-v]

The ordinary grader reads .obj text, which under /GL holds IL instead of code, so the `ltcg` gap class
(callee register knowledge, custom register arguments) can only be checked from a linked image.

- The target function is always exported so LTCG keeps the standard convention, the way retail's copy
  (called from other modules / through vtables) was kept.
- --export REGEX also exports every mapped symbol matching REGEX (callees retail calls with the standard
  convention). Symbols NOT exported stay internal: LTCG may see their bodies, learn their register use and
  give them custom conventions -- the same-TU stand-ins become retail's whole-program knowledge.
- Unresolved externals are allowed (/FORCE:UNRESOLVED); their call displacements are masked like relocations.

Masking: DIR32 fixups from the DLL's .reloc table, plus rel32 call/jmp displacements whose target leaves the
function, on the built side; the same opcode positions on the retail side. Status: MATCH / RELOCATION_MATCH /
DIFFER(built v retail) / SRC_FAIL / LINK_FAIL.
"""
import sys, os, re, csv, glob, struct
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
import verify_and_land as V

ROOT = Path(__file__).resolve().parents[4]


def oracle(addr):
    S = os.environ.get("S", "")
    files = [ROOT / "rebuild/oracles/auto-re-candidates.tsv"] + sorted(glob.glob(f"{S}/batch/*_oracle.tsv")) \
        + sorted(glob.glob(str(ROOT / "rebuild/candidates/manual/batches/*/oracle.tsv")))
    for f in files:
        try:
            rows = csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t")
            for r in rows:
                if r.get("address", "").lower().replace("0x", "") == addr:
                    b = bytes.fromhex(r["bytes"])
                    while b and b[-1] in (0xCC, 0x90):
                        b = b[:-1]
                    return b, r.get("name", "")
        except (OSError, KeyError, ValueError):
            continue
    return None, None


def pe_sections(img):
    pe = struct.unpack_from("<I", img, 0x3C)[0]
    nsec = struct.unpack_from("<H", img, pe + 6)[0]
    optsz = struct.unpack_from("<H", img, pe + 20)[0]
    base = struct.unpack_from("<I", img, pe + 24 + 28)[0]
    reloc_rva, reloc_sz = struct.unpack_from("<II", img, pe + 24 + 96 + 5 * 8)
    secs = []
    for i in range(nsec):
        o = pe + 24 + optsz + i * 40
        name = img[o:o + 8].rstrip(b"\0").decode()
        vsize, va, rsize, raw = struct.unpack_from("<IIII", img, o + 8)
        secs.append((name, va, vsize, raw, rsize))
    return base, secs, reloc_rva, reloc_sz


def rva_to_off(secs, rva):
    for _, va, vs, raw, rs in secs:
        if va <= rva < va + max(vs, rs):
            return raw + rva - va
    raise ValueError(hex(rva))


def dir32_fixups(img, secs, reloc_rva, reloc_sz):
    out = set()
    if not reloc_rva:
        return out
    o = rva_to_off(secs, reloc_rva); end = o + reloc_sz
    while o < end:
        page, size = struct.unpack_from("<II", img, o)
        if size < 8:
            break
        for k in range(8, size, 2):
            e = struct.unpack_from("<H", img, o + k)[0]
            if e >> 12 == 3:
                out.add(page + (e & 0xFFF))
        o += size
    return out


def rel32_slots(code, start_va):
    """Offsets of rel32 displacements of call/jmp that leave [start, start+len)."""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    out = []
    for i in md.disasm(code, start_va):
        if i.bytes[0] in (0xE8, 0xE9) and i.size == 5:
            tgt = start_va + (i.address - start_va) + 5 + struct.unpack_from("<i", i.bytes, 1)[0]
            if not (start_va <= tgt < start_va + len(code)):
                out.append(i.address - start_va + 1)
    return out


def main():
    a = sys.argv[1:]
    verbose = "-v" in a
    a = [x for x in a if x != "-v"]
    flags = []
    if "--flags" in a:
        i = a.index("--flags"); flags = a[i + 1:]; a = a[:i]
    exp = inter = None
    if "--export" in a:
        i = a.index("--export"); exp = a[i + 1]; del a[i:i + 2]
    if "--internal" in a:
        i = a.index("--internal"); inter = a[i + 1]; del a[i:i + 2]
    addr = a[0].lower().replace("0x", ""); src = Path(a[1]).read_text(encoding="utf-8"); leaf = a[2]
    retail, name = oracle(addr)
    if retail is None:
        sys.exit("no oracle for " + addr)
    work = Path(os.environ.get("S", "/tmp")) / "wine" / "ltcg" / addr
    work.mkdir(parents=True, exist_ok=True)
    e = V.env()
    cpp = work / "t.cpp"; cpp.write_text(V.vc71(src), encoding="utf-8")
    obj = work / "t.obj"; obj.unlink(missing_ok=True)
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + ["/GL"] + flags + [f"/Fo{obj}", str(cpp)], e)
    if cp.returncode or not obj.exists():
        print(addr, name, "SRC_FAIL"); print(cp.stdout[-2000:]); return
    link = V.VC / "bin" / "link.exe"
    dll, mp = work / "t.dll", work / "t.map"

    def do_link(exports):
        for p in (dll, mp):
            p.unlink(missing_ok=True)
        args = ["/nologo", "/DLL", "/NOENTRY", "/LTCG", "/FORCE:UNRESOLVED", "/OPT:NOREF", "/OPT:NOICF",
                f"/MAP:{V.wpath(mp)}", f"/out:{dll}"] + [f"/EXPORT:{s}" for s in exports] + [str(obj)]
        r = V.run_tool(link, args, e)
        return dll.exists() and mp.exists(), r

    ok, r = do_link([])
    if not ok:
        print(addr, name, "LINK_FAIL"); print(r.stdout[-2000:]); return
    rows = re.findall(r"^\s*\w+:\w+\s+(\S+)\s+([0-9a-f]{8}) f (?:i )?\s*(\S+)\s*$", mp.read_text(errors="replace"), re.M)
    syms = [(s, v) for s, v, _ in rows]
    own = [s for s, _, o in rows if o.lower() == "t.obj"]   # only our TU's symbols can be exported; CRT ones fail
    target = [s for s in own if leaf in s]
    if not target:
        print(addr, name, "NO_SYMBOL", leaf); return
    exports = {target[0]}
    for s in own:
        if exp and re.search(exp, s) and not (inter and re.search(inter, s)):
            exports.add(s)
    ok, r = do_link(sorted(exports))
    if not ok:
        print(addr, name, "LINK_FAIL"); print(r.stdout[-2000:]); return
    m = re.search(r"^\s*\w+:\w+\s+" + re.escape(target[0]) + r"\s+([0-9a-f]{8}) f ", mp.read_text(errors="replace"), re.M)
    va = int(m.group(1), 16)
    img = dll.read_bytes()
    base, secs, rrva, rsz = pe_sections(img)
    off = rva_to_off(secs, va - base)
    # Compare on the retail length. Map symbol spacing is not a length: /FORCE:UNRESOLVED points every missing
    # external at one stub, and exported functions are packed with no padding. Retail's last instruction is a
    # ret/jmp, so a byte-equal prefix of retail's length is the whole built function.
    built = img[off:off + len(retail)]
    fix = {f - (va - base) for f in dir32_fixups(img, secs, rrva, rsz) if 0 <= f - (va - base) < len(built)}
    slots = set(rel32_slots(built, va))
    msk = set()
    for f in fix | slots:
        msk |= {f, f + 1, f + 2, f + 3}
    mb = bytes(0 if i in msk else c for i, c in enumerate(built))
    mr = bytes(0 if i in msk else c for i, c in enumerate(retail))
    if built == retail:
        st = "MATCH"
    elif len(built) == len(retail) and mb == mr:
        st = "RELOCATION_MATCH"
    else:
        st = f"DIFFER({len(built)}v{len(retail)})"
    print(addr, name, st, "ltcg exports=", len(exports))
    if verbose or not st.endswith("MATCH"):
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32)
        print("--- retail (%d bytes)" % len(retail))
        print("\n".join(f"{i.address:08x}  {i.bytes.hex():22} {i.mnemonic} {i.op_str}" for i in md.disasm(retail, int(addr, 16))))
        print("--- built (%d bytes)" % len(built))
        print("\n".join(f"{i.address:08x}  {i.bytes.hex():22} {i.mnemonic} {i.op_str}" for i in md.disasm(built, int(addr, 16))))


if __name__ == "__main__":
    main()
