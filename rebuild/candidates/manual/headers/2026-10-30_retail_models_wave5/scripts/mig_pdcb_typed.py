#!/usr/bin/env python3
"""Wave 4 family PDCB-typed: a row that defines the whole local def chain (any of CIVCountedPointeeBase / CResource /
CDefPointeeBase / CDefClassBase / CParentDefClassBase) is moved onto the shared retail chain (FABLE_RETAIL_DEF_CLASS_BASE:
engine/CParentDefClassBase.h, vtable 0x01230CBC, 0x39 bytes packed). When the row defines CDefString and CVectorMap
itself, the header's typed members are used (FABLE_PDCB_TYPED_MEMBERS: CDefString InstantiationName +0x25, CVectorMap
SubDefInfoMap +0x29) and the include goes after those definitions; a CVectorMap defined inside a pack(1) region is wrapped
in pack(push, 8) (wave 3 LESSONS item 1). Rows whose retail bytes inline the chain ctor (a call to CResource() 0x009FBEC0
outside 0x00430370 itself) are skipped: the shared CDefClassBase has no default ctor (CParentDefClassBase() is the
out-of-line 0x00430370). Every other row is left for mcheck. Idempotent.
usage: mig_pdcb_typed.py [--dry] <file>..."""
import sys, re, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
R = os.environ["FABLE_ROOT"]
CHAIN = ("CIVCountedPointeeBase", "CResource", "CDefPointeeBase", "CDefClassBase", "CParentDefClassBase")
cat = {}
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat[q[0].lower()] = q
def calls(a):
    if a not in cat: return set()
    b = bytes.fromhex(cat[a][3]); va = int(a, 16); out = set()
    for i in range(len(b) - 4):
        if b[i] == 0xE8: out.add((va + i + 5 + struct.unpack_from("<i", b, i + 1)[0]) & 0xFFFFFFFF)
    return out
def packdepth1(t, pos):
    st = []
    for m in re.finditer(r"#pragma\s+pack\s*\(\s*(push\s*,\s*(\d+)|pop)\s*\)", t[:pos]):
        if m.group(1).startswith("push"): st.append(int(m.group(2)))
        elif st: st.pop()
    return bool(st) and st[-1] == 1
def fix(t, a):
    if "engine/CParentDefClassBase.h" in t: return None, "already includes"
    if a != "00430370" and 0x009FBEC0 in calls(a): return None, "retail inlines the chain ctor"
    blocks = []
    for c in CHAIN:
        b = find_block(t, c)
        if b: blocks.append((b[0], b[1], c))
    if not any(c == "CParentDefClassBase" for _, _, c in blocks): return None, "no local CParentDefClassBase"
    typed = bool(find_block(t, "CDefString")) and bool(find_block(t, "CVectorMap"))
    pdcb = [b for b in blocks if b[2] == "CParentDefClassBase"][0]
    if typed:
        ends = [find_block(t, c)[1] for c in ("CDefString", "CVectorMap", "CArray", "CKeyPairCompareLess") if find_block(t, c)]
        if max(ends) > pdcb[0]: return None, "typed member types defined after the local CParentDefClassBase"
    out = t
    # wrap a pack(1)-region CVectorMap in pack(8)
    if typed:
        vb = find_block(out, "CVectorMap")
        if packdepth1(out, vb[0]):
            out = out[:vb[0]] + "#pragma pack(push, 8)\n" + out[vb[0]:vb[1]] + "#pragma pack(pop)\n" + out[vb[1]:]
    inc = ("// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:\n"
           "//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,\n"
           "//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0)"
           + (";\n//   FABLE_PDCB_TYPED_MEMBERS: InstantiationName / SubDefInfoMap are typed with this row's CDefString / CVectorMap.\n" if typed else ".\n")
           + "#define FABLE_RETAIL_DEF_CLASS_BASE\n" + ("#define FABLE_PDCB_TYPED_MEMBERS\n" if typed else "")
           + "#include \"engine/CParentDefClassBase.h\"\n")
    # remove the chain blocks from the last to the first; the include replaces the CParentDefClassBase block
    for c in reversed(CHAIN):
        b = find_block(out, c)
        if not b: continue
        s, e = b
        pre = out.rfind("\n", 0, s - 1) + 1
        if re.fullmatch(r"#pragma\s+pack\s*\(\s*push\s*,\s*1\s*\)\s*\n", out[pre:s]) and re.match(r"#pragma\s+pack\s*\(\s*pop\s*\)[^\n]*\n", out[e:]):
            e += re.match(r"#pragma\s+pack\s*\(\s*pop\s*\)[^\n]*\n", out[e:]).end(); s = pre
        out = out[:s] + (inc if c == "CParentDefClassBase" else "") + out[e:]
    return out, "ok typed" if typed else "ok untyped"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [x for x in sys.argv[1:] if not x.startswith("--")]:
        a = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
        t = open(f, encoding="utf-8").read()
        r, why = fix(t, a)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
