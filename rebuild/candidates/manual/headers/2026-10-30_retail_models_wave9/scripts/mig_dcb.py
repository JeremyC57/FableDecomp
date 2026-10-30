#!/usr/bin/env python3
"""Wave 1 family DCB: rows with a row-local CDefClassBase stand-in (opaque `_body[0x25]`, a data-only copy of the retail
members, or the nested CSubDefInfo only) plus optional local CIVCountedPointeeBase / CResource / CDefPointeeBase /
CSubDefClassBase. Rewrite: the FABLE_RETAIL_DEF_CLASS_BASE shared headers (engine/CDefClassBase.h, engine/CSubDefClassBase.h)
and the derived def classes get their real overrides (Transfer, Copy, GetSizeofClass) with the retail slot addresses from
the def vtable scan (work/def_vtables.tsv). Only the accepted local shapes are rewritten (anything with invented slots,
helper methods or a local CParentDefClassBase / CDefBase is left for wave 2). Idempotent.
usage: mig_dcb.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
S = os.environ["S"]
VT = {}
for l in open(S + "/mig9/work/def_vtables.tsv"):
    q = l.rstrip("\n").split("\t")
    m = re.match(r"\.\?AV([A-Za-z_0-9]+)@(?:[A-Za-z_0-9]+@)*@$", q[1])
    if not m and q[1] == "None": m = re.match(r"([A-Za-z_0-9]+)::Transfer$", q[6])
    if m and q[3] != "00bfea08": VT[m.group(1)] = dict(vt=q[0], dtor=q[2], tr=q[3], cp=q[4], sz=q[5])
    if q[0] == "012fb5c8":   # no RTTI, no label: the landed CScriptDef ctor 0x00F28F90 stores it (row 0x00F29F40)
        VT["CScriptDef"] = dict(vt=q[0], dtor=q[2], tr=q[3], cp=q[4], sz=q[5])

def body_of(nb):
    i = nb.index("{"); return nb[:i].strip(), nb[i+1:nb.rindex("}")].strip()

DCB_ITEMS = [r"class CSubDefInfo \{ public: long DefIndex; long OriginalParentDefIndex; \};", r"public:",
             r"CDefinitionManager\* PDefManager;", r"unsigned long GlobalIndex;", r"unsigned char Setup : 1;",
             r"unsigned char Template : 1;", r"unsigned char DefaultValsApplied : 1;",
             r"unsigned char _(?:body|base)\[0x2[58]\];", r"unsigned char _body\[0x25 - 4\];", r"unsigned char _body\[0x24\];",
             r"virtual void v00\(\);", r"virtual ~CDefClassBase\(\);",
             r"CDefClassBase& operator=\(const CDefClassBase& (?:other|rhs)?\);"]
def ok_dcb(nb):
    head, body = body_of(nb)
    if head not in ("class CDefClassBase", "class CDefClassBase : public CDefPointeeBase"): return False
    for it in DCB_ITEMS: body = re.sub(it, " ", body)
    if body.strip(): return False
    # a vptr-less opaque base must not be paired with the dtor-only shape etc.: the opaque forms all total 0x25 bytes
    return True
def ok_simple(cls, nb, allowed_heads, items):
    head, body = body_of(nb)
    if head not in allowed_heads: return False
    for it in items: body = re.sub(it, " ", body)
    return not body.strip()
RES_ITEMS = [r"public:", r"CResource\(\);", r"virtual ~CResource\(\);", r"unsigned long IVRefCount;",
             r"CResourceList\* ResourceList;", r"CResource\* PrevResource;", r"CResource\* NextResource;",
             r"unsigned long ResourceSize;", r"unsigned long LastUsedFrame;"]
IV_ITEMS = [r"public:", r"virtual ~CIVCountedPointeeBase\(\);", r"virtual void Delete\(\) const;", r"unsigned long IVRefCount;"]
DPB_ITEMS = [r"public:", r"virtual ~CDefPointeeBase\(\) \{ ?\}"]
SDC_ITEMS = [r"public:", r"CSubDefClassBase\(\);"]

def fix(t):
    b = find_block(t, "CDefClassBase")
    if not b and '#include "engine/CDefClassBase.h"' in t and "#define FABLE_RETAIL_DEF_CLASS_BASE" in t:
        return overrides(t)
    if not b: return None, "noblock"
    if not ok_dcb(norm(t[b[0]:b[1]])): return None, "dcb-shape"
    for bad in ("CParentDefClassBase", "CDefBase"):
        bb = find_block(t, bad)
        if bb: return None, "local " + bad
    cut = [b]
    for cls, heads, items in (("CIVCountedPointeeBase", ("class CIVCountedPointeeBase",), IV_ITEMS),
                              ("CResource", ("class CResource", "class CResource : public CIVCountedPointeeBase"), RES_ITEMS),
                              ("CDefPointeeBase", ("class CDefPointeeBase : public CResource",), DPB_ITEMS),
                              ("CSubDefClassBase", ("class CSubDefClassBase : public CDefClassBase",), SDC_ITEMS)):
        bb = find_block(t, cls)
        if not bb: continue
        if not ok_simple(cls, norm(t[bb[0]:bb[1]]), heads, items): return None, "shape " + cls
        cut.append(bb)
    if re.search(r'#include "engine/(CDefClassBase|CResource|CDefPointeeBase|CIVCountedPointeeBase|CSubDefClassBase)\.h"', t):
        return None, "already includes a def-chain header"
    need_sub = any(t[s:e].lstrip().startswith("class CSubDefClassBase") for s, e in cut) or re.search(r"public CSubDefClassBase\b", t)
    cut.sort()
    first = cut[0][0]
    inc = '#define FABLE_RETAIL_DEF_CLASS_BASE\n#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)\n'
    if need_sub: inc += '#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54\n'
    pushes = [m.start() for m in re.finditer(r'#pragma pack\(push, ?1\)\n', t[:first])]
    if pushes and "#pragma pack(pop)" not in t[pushes[-1]:first]:
        ins = pushes[-1]
    else:
        ins = first
    out = t[:ins] + inc + t[ins:first]
    pos = first
    for s, e in cut:
        out += t[pos:s] if s > pos else ""
        pos = e
    out += t[pos:]
    # a #pragma pack(push, 1) region left empty by the cut goes too
    out = re.sub(r'#pragma pack\(push, ?1\)\n((?:[ \t]*(?://[^\n]*)?\n)*)#pragma pack\(pop\)\n', r'\1', out)
    # move the define above any earlier engine include
    m = re.search(r'#include "engine/', out)
    if m and m.start() < out.index('#define FABLE_RETAIL_DEF_CLASS_BASE'):
        out = out.replace('#define FABLE_RETAIL_DEF_CLASS_BASE\n', '', 1)
        out = out[:m.start()] + '#define FABLE_RETAIL_DEF_CLASS_BASE\n' + out[m.start():]
    return overrides(out)

def overrides(out):
    """derived def classes: add the real overrides that are missing"""
    notes = []
    res = out
    for m in list(re.finditer(r'^class ([A-Za-z_0-9]+) : public C(?:Sub)?DefClassBase \{', out, re.M)):
        name = m.group(1)
        v = VT.get(name)
        bb = find_block(res, name)
        if not bb or not v:
            notes.append("no-vtable:" + name); continue
        blk = res[bb[0]:bb[1]]
        add = []
        if not re.search(r"\bTransfer\s*\(\s*CPersistContext\s*&", blk): add.append(f"    virtual void Transfer(CPersistContext& context);         // slot 18 0x{v['tr'].upper()}")
        if not re.search(r"\bCopy\s*\(\s*const\s+CDefClassBase\s*\*", blk): add.append(f"    virtual void Copy(const CDefClassBase* other);           // slot 19 0x{v['cp'].upper()}")
        if not re.search(r"\bGetSizeofClass\s*\(\s*\)", blk): add.append(f"    virtual unsigned long GetSizeofClass();                  // slot 20 0x{v['sz'].upper()}")
        if not add: continue
        j = blk.rindex("};")
        pre = blk[:j]
        if not pre.endswith("\n"): pre += "\n"
        # keep the overrides public
        acc = "public:\n" if re.search(r"^\s*(private|protected):", pre, re.M) else ""
        nb = pre + acc + "\n".join(add) + "\n" + blk[j:]
        if "// def vtable 0x" not in res[max(0, bb[0]-300):bb[0]]:
            nb = f"// def vtable 0x{v['vt'].upper()}: slot 0 0x{v['dtor'].upper()} (implicit inline dtor); real overrides from the def vtable scan\n" + nb
        res = res[:bb[0]] + nb + res[bb[1]:]
    return res, ";".join(notes) or "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if a != "--dry"]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if r == t: print("same", f); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
