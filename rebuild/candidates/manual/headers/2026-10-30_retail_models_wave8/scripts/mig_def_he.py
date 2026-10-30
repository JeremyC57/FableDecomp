#!/usr/bin/env python3
"""Wave 1 family DEF-HE: rows that pulled the flattened def headers in under renaming macros (`#define CDefClassBase
CDefClassBase_HeaderEvidence`) and then re-modelled CIVCountedPointeeBase/CResource/CDefPointeeBase plus a packed (0x25)
CDefClassBase with invented UnknownDefinitionSlotN virtuals. Rewrite: the FABLE_RETAIL_DEF_CLASS_BASE shared header, the
derived def classes keep their members and get their real overrides (Transfer, Copy, GetSizeofClass; the dtor stays implicit) with the retail
slot addresses from the def vtable scan (work/def_vtables.tsv). Idempotent: a file without the macro block is left alone.
usage: mig_def_he.py <file>..."""
import sys, re, os
S = os.environ["S"]
VT = {}
for l in open(S + "/mig8/work/def_vtables.tsv"):
    q = l.rstrip("\n").split("\t")
    m = re.match(r"(?:\?Transfer@)?([A-Za-z_0-9]+)(?:::Transfer|@@)", q[6])
    if m: VT[m.group(1)] = dict(vt=q[0], dtor=q[2], tr=q[3], cp=q[4], sz=q[5])
def fix(t):
    if "CDefClassBase_HeaderEvidence" not in t: return None
    for c in ("CIVCountedPointeeBase", "CResource", "CDefClassBase"):
        t = t.replace(f'#define {c} {c}_HeaderEvidence\n#include "engine/{c}.h"\n#undef {c}\n', "")
    t = re.sub(r'^class CIVCountedPointeeBase\{[^\n]*\n', '', t, flags=re.M)
    t = re.sub(r'^class CResourceList;class CResource:public CIVCountedPointeeBase\{.*?\};class CDefPointeeBase:public CResource\{public:virtual void Delete\(\)const;\};',
               '', t, flags=re.M)
    t, n = re.subn(r'#pragma pack\(push,1\)\nclass CDefClassBase:public CDefPointeeBase\{.*?\};\n#pragma pack\(pop\)\n',
                   '#define FABLE_RETAIL_DEF_CLASS_BASE\n#include "engine/CDefClassBase.h"\n', t, count=1, flags=re.S)
    if n != 1: raise SystemExit("no packed CDefClassBase block")
    # derived def classes: add the real overrides that are missing
    def derived(m):
        name, body = m.group(1), m.group(2)
        v = VT.get(name)
        if not v: raise SystemExit("no def vtable for " + name)
        add = []
        if "Transfer(CPersistContext&)" not in body: add.append("virtual void Transfer(CPersistContext&);")
        if "Copy(const CDefClassBase*)" not in body: add.append("virtual void Copy(const CDefClassBase*);")
        if "GetSizeofClass()" not in body: add.append("virtual unsigned long GetSizeofClass();")
        note = (f"   // def vtable 0x{v['vt'].upper()} (no RTTI): slot 0 0x{v['dtor'].upper()} (implicit inline dtor), Transfer 0x{v['tr'].upper()},"
                f" Copy 0x{v['cp'].upper()}, GetSizeofClass 0x{v['sz'].upper()}")
        return f"class {name}:public CDefClassBase{{{body}{''.join(add)}}};{note}"
    t = re.sub(r'(?<=[\n;])class ([A-Za-z_0-9]+):public CDefClassBase\{(.*?)\};(?=\n)', derived, t, flags=re.M | re.S)
    t = t.replace("packed37 ancestry, private default Def ctor with required original derived friendship, public copy ctor",
                  "retail ancestry from the shared FABLE_RETAIL_DEF_CLASS_BASE headers (CDefClassBase 0x25 packed, vtable 0x01230BCC)")
    t = t.replace("True CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase", "CIVCountedPointeeBase/CResource/CDefPointeeBase/CDefClassBase")
    t = t.replace("packed Def37 and flags+36", "CDefClassBase 0x25 packed (FABLE_RETAIL_DEF_CLASS_BASE shared header) and flags +0x24")
    t = t.replace("Unknown unused definition slots2–17 are explicit interface projections; actual Transfer slot18.",
                  "Definition slots 0-24 come from the shared header (Transfer slot 18).")
    return t
if __name__ == "__main__":
    for f in sys.argv[1:]:
        t = open(f, encoding="utf-8").read()
        try: r = fix(t)
        except SystemExit as ex: print("FAIL", f, ex); continue
        if r is None: print("skip", f); continue
        if r != t: open(f, "w", encoding="utf-8").write(r); print("edit", f)
