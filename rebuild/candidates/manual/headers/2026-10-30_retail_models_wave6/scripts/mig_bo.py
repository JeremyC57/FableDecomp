#!/usr/bin/env python3
"""Wave 5 family BO: a row-local CBaseObject (the Fable RTTI class, not DirectShow's strmbase CBaseObject) becomes the
retail engine/CBaseObject.h under FABLE_RETAIL_BASE_OBJECT (: CBaseClass, vtable 0x0129A7D8: slot 0 dtor 0x0099A4D0,
slot 1 Validate() const 0x0099A400; IntelligentPointer +0x04). Accepted block items: access labels, the ctor / copy ctor /
operator= / ClearIntelligentPointers declarations, `(virtual) ~CBaseObject();`, `virtual void Validate() const;`, slot-0
stand-ins (`virtual void b000();`, `v00`, ...: the dtor slot; dropped, with derived mirrors at brace depth 1), and exactly
one 4-byte member (the pointer; renamed to IntelligentPointer where the row uses it). The block must be polymorphic (a
virtual or `: public CBaseClass`). The `#define CBaseObject CBaseObject_HeaderLayout` alias include is removed.
Idempotent. usage: mig_bo.py [--dry] [--force-nonconst] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
STAND = r"(?:v0+|b0+)"
REPL = ('#define FABLE_RETAIL_BASE_OBJECT\n'
        '#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)\n')
ALIAS = re.compile(r'[ \t]*#define CBaseObject CBaseObject_\w+[^\n]*\n[ \t]*#include "engine/CBaseObject.h"[^\n]*\n[ \t]*#undef CBaseObject[^\n]*\n')
MEMBER = re.compile(r"^(?:CBaseObjectPointer ?\* ?|void ?\* ?|long |unsigned char )(\w+)(?: ?\[(?:4|0x4)\])?;$")
ITEMS = [r"(?:public|protected|private):", r"(?:virtual ?)?~CBaseObject\(\);", r"CBaseObject\(\);",
         r"CBaseObject ?& ?operator ?= ?\(const CBaseObject ?& ?\w*\);", r"CBaseObject\(const CBaseObject ?& ?\w*\);",
         r"void ClearIntelligentPointers\(\);", r"virtual void Validate\(\) ?const;", r"virtual void " + STAND + r"\(\);"]
def items(body):
    out = []; i = 0; body = body.strip()
    while body:
        m = re.match(r"(?:public|protected|private):\s*", body)
        if not m: m = re.match(r"[^;{}]*;\s*", body)
        if not m: return None
        out.append(m.group(0).strip()); body = body[m.end():]
    return out
def fix(t):
    if "FABLE_RETAIL_BASE_OBJECT" in t: return None, "already"
    if re.search(r"m_cObjects|WCHAR|wchar_t|unsigned short\* pName|CBaseObject\(const char|CUnknown|INonDelegatingUnknown", t): return None, "dshow"
    b = find_block(t, "CBaseObject")
    if not b: return None, "noblock"
    if find_block(t, "CBaseObject", b[1]): return None, "two blocks"
    if find_block(t, "CBaseClass"): return None, "local CBaseClass"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CBaseObject ?(:\s*public\s+CBaseClass\s*)?\{(.*)\};$", n)
    if not m: return None, "head"
    its = items(m.group(2))
    if its is None: return None, "parse"
    poly = bool(m.group(1)); mem = None; stand = set(); nonconst = False
    for it in its:
        if any(re.fullmatch(x, it) for x in ITEMS):
            if "virtual" in it: poly = True
            s = re.fullmatch(r"virtual void (" + STAND + r")\(\);", it)
            if s: stand.add(s.group(1))
            continue
        if it == "virtual void Validate();" and FORCE_NC: poly = True; nonconst = True; continue
        mm = MEMBER.match(it)
        if mm and mem is None: mem = mm.group(1); continue
        return None, "item " + it[:60]
    if not poly and not ALLOW_LOOSE: return None, "not polymorphic"
    if mem is None and not ALLOW_LOOSE: return None, "no member"
    if mem is None: mem = "IntelligentPointer"
    rest = t[:b[0]] + t[b[1]:]
    code = re.sub(r"//[^\n]*", "", rest)
    for s in stand:
        if re.search(r"(?:->|\.|::)\s*" + s + r"\s*\(", code): return None, "calls stand-in " + s
    out = t[:b[0]] + REPL + t[b[1]:]
    out, na = ALIAS.subn("", out)
    if re.search(r'#include "engine/CBaseObject.h"', out.replace(REPL, "")): return None, "other CBaseObject.h include"
    ren = 0
    if mem != "IntelligentPointer" and re.search(r"\b" + mem + r"\b", re.sub(r"//[^\n]*", "", out)):
        if mem not in ("m_pIntelligentPointer",): return None, "uses member " + mem
        out, ren = re.subn(r"\b" + mem + r"\b", "IntelligentPointer", out)
    mirrors = 0
    if stand:
        edges = re.findall(r"(?:class|struct)\s+(\w+)\s*:\s*((?:(?:public|protected|private)?\s*[\w:<>]+\s*,?\s*)+)\{", out)
        derived = {"CBaseObject"}; ch = True
        while ch:
            ch = False
            for d, bases in edges:
                bl = [re.sub(r"^(?:public|protected|private)\s+", "", x.strip()).split("<")[0] for x in bases.split(",")]
                if d not in derived and any(x in derived for x in bl):
                    derived.add(d); ch = True
        derived.discard("CBaseObject")
        def depth_at(s, i):
            d = 0
            for c in s[:i]:
                if c == "{": d += 1
                elif c == "}": d -= 1
            return d
        for d in sorted(derived):
            db = find_block(out, d)
            if not db: continue
            seg = out[db[0]:db[1]]
            for s in stand:
                for rx in (r"[ \t]*virtual\s+void\s+" + s + r"\s*\(\s*\)\s*;[ \t]*(//[^\n]*)?\n", r"virtual\s+void\s+" + s + r"\s*\(\s*\)\s*;\s*"):
                    hits = [h for h in re.finditer(rx, seg) if depth_at(seg, h.start()) == 1]
                    for h in reversed(hits): seg = seg[:h.start()] + seg[h.end():]
                    mirrors += len(hits)
                    if hits: break
            out = out[:db[0]] + seg + out[db[1]:]
    return out, f"ok mem={mem} ren={ren} alias={na} stand={','.join(sorted(stand)) or '-'} mirrors={mirrors}{' nonconst' if nonconst else ''}"
FORCE_NC = "--force-nonconst" in sys.argv
ALLOW_LOOSE = "--loose" in sys.argv   # wave 5: an opaque base (no member, or not polymorphic): mcheck decides
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
