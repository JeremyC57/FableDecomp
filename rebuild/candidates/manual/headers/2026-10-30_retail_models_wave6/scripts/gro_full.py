#!/usr/bin/env python3
"""Wave 6 GRO rule (LESSONS item 1): VC7.1 picks inline-or-call per site for the implicit ScriptedThingBase dtor from the
inlinee's size, so the row's CCountedPointer<T> must carry the retail release dtor (0x007E74D0's body: dec RefCount,
DeleteFunc(Info->Data) __fastcall, operator delete(Info), zero Data/Info). A row whose PImp model has no dtor (a two-word
CCountedPointerWords struct, a dtor-less template, or nothing) gets FULL below in its place, then mig_gro.fix runs.
usage: gro_full.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
import mig_gro
INFO = ("class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)\n"
        "    long RefCount;                                     // +0\n"
        "    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee\n"
        "    void* Data;                                        // +8\n"
        "};\n")
DEL = "void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)\n"
FULL = ("template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release\n"
        "    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:\n"
        "    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows\n"
        "    CCountedPointer() : Data(0), Info(0) {}            // from this size)\n"
        "    ~CCountedPointer()\n"
        "    {\n"
        "        if (Info) {\n"
        "            --Info->RefCount;\n"
        "            if (Info->RefCount == 0) {\n"
        "                Info->DeleteFunc(Info->Data);\n"
        "                delete Info;\n"
        "            }\n"
        "        }\n"
        "        Data = 0;\n"
        "        Info = 0;\n"
        "    }\n"
        "};\n")
GRO = re.compile(r"^[ \t]*(?:class|struct)\s+CScriptGameResourceObject(?:ScriptedThing|Movie)?Base\b[^;]*\{", re.M)
def prep(t):
    """returns (text, note) or (None, why)"""
    code = re.sub(r"//[^\n]*", "", t)
    b = find_block(t, "CCountedPointerWords")
    tb = re.search(r"template\s*<\s*(?:class|typename) \w+\s*>\s*(?:class|struct)\s+CCountedPointer\b[^;]*\{", t)
    if tb:
        bb = find_block(t, "CCountedPointer")
        if not bb: return None, "cp block"
        if re.search(r"~CCountedPointer\s*\(", t[bb[0]:bb[1]]): return t, "has dtor"
        n = norm(t[bb[0]:bb[1]])
        if re.search(r"operator|Release|AddRef|Get\w*\(", n): return None, "cp template with methods"
        start, end, kind = bb[0], bb[1], "tmpl"
    elif b:
        rest = t[:b[0]] + t[b[1]:]
        uses = re.findall(r"\bCCountedPointerWords\b", re.sub(r"//[^\n]*", "", rest))
        if len(uses) != len(re.findall(r"\bCCountedPointerWords\s+PImp\s*;", rest)): return None, "Words used elsewhere"
        n = norm(t[b[0]:b[1]])
        if re.search(r"~|operator|Release|AddRef", n): return None, "Words with methods"
        start, end, kind = b[0], b[1], "words"
    elif re.search(r"\bCCountedPointer\b", code): return None, "names CCountedPointer"
    else:
        m = GRO.search(t)
        if not m: return None, "no GRO"
        start = end = m.start(); kind = "none"
    pre = ""
    ib = find_block(t, "CCPPointerInfo")
    if ib:
        ni = norm(t[ib[0]:ib[1]])
        if not re.search(r"\bRefCount\b", ni) or not re.search(r"\bDeleteFunc\b", ni) or not re.search(r"\bData\b", ni):
            return None, "other CCPPointerInfo"
        if ib[0] > start: return None, "CCPPointerInfo after"
    else: pre += INFO
    if not re.search(r"operator\s+delete\s*\(", code): pre += DEL
    out = t[:start] + pre + FULL + t[end:]
    if kind == "words":
        out = re.sub(r"\bCCountedPointerWords(\s+PImp\s*;)", r"CCountedPointer<CScriptGameResourceObjectScriptedThingBase>\1", out)
    return out, kind
_ok = mig_gro.ok_item
CUR = {"t": ""}
def ok_item(cls, it):
    if _ok(cls, it): return True
    m = re.fullmatch(r"virtual void ([vshm]\d+)\(\)\s*(?:const)?;", it)   # filler slot the row never calls
    if m and not re.search(r"(?:\.|->)\s*" + m.group(1) + r"\s*\(", CUR["t"]): return True
    return False
mig_gro.ok_item = ok_item
NOMOVIE = "--nomovie" in sys.argv
def fix(t):
    if NOMOVIE and find_block(t, "CScriptGameResourceObjectMovieBase"): return None, "movie (wave 6: residue)"
    p, note = prep(t)
    if p is None: return None, "prep: " + note
    CUR["t"] = p
    if "FABLE_RETAIL_BASE_OBJECT" not in p and not find_block(p, "CBaseObject") and "engine/CBaseObject.h" not in p:
        i = p.find("class CCPPointerInfo { public:                         // CCountedPointer's shared block")
        if i < 0: i = p.find("template <class T> class CCountedPointer { public:     // retail")
        if i < 0: i = GRO.search(p).start()
        p = p[:i] + "#define FABLE_RETAIL_BASE_OBJECT\n" + p[i:]; note += "+bo"
    r, why = mig_gro.fix(p)
    if r is None: return None, why
    return r, why + " cp=" + note
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
