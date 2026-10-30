#!/usr/bin/env python3
"""Wave 7 item 4 (family ST): a row-local CScriptThing becomes engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT: : CBaseClass,
vtable 0x01238C8C, 79 slots, 0xc; ctor 0x006E7B40, copy 0x004ABE90, ~ 0x004AA840 all out of line).
Accepted local items: an optional `: public CBaseClass` base; the out-of-line ctor / copy ctor / dtor (virtual or not) /
operator= / operator== / operator!= declarations; the PImp model (`CCountedPointer<CScriptThing> PImp;`, or two void*
words / an 8-byte pad when the row never names them); virtual slot declarations: slot k (in local declaration order, the
dtor taking its own position) must be a filler the row never calls, or carry the header's slot-k name (a different name
at slot k is renamed at its call sites only for the invented GetThing -> PDB GetPThing, slot 11); declarations of
non-virtual members the header has. Anything with an inline body, or a slot that disagrees with the header, skips.
Contract: CCountedPointer<T> must be a template defined before the block (else, when the row has none, a minimal
`template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };` is added: the Ego_r members).
The HeaderLayout alias trio (#define CScriptThing CScriptThing_HeaderLayout / include / #undef) goes when unused.
usage: mig_st.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
from mig_sb import items
R = os.environ["FABLE_ROOT"]
H = open(R + "/rebuild/include/engine/CScriptThing.h").read()
HR = H[H.index("class CScriptThing : public CBaseClass"):H.index("#else // flattened")]
SLOT = {0: "~CScriptThing"}; SIG = {}
def sig(decl, name):
    """return type + parameter types + const of a declaration, names dropped, whitespace normalised"""
    d = re.sub(r"\s+", " ", decl.replace("virtual ", "").strip().rstrip(";").strip())
    mm = re.match(r"(.*?)\b" + re.escape(name) + r"\s*\((.*)\)\s*(const)?\s*$", d)
    if not mm: return None
    ps = []
    for p in [x.strip() for x in mm.group(2).split(",") if x.strip() and x.strip() != "void"]:
        p = re.sub(r"\s*=.*$", "", p)
        p = re.sub(r"(?<=[\w*&>])\s+\w+$", "", p) if re.search(r"[\w*&>]\s+\w+$", p) and not re.fullmatch(r"(unsigned|signed|const)\s+\w+", p) else p
        ps.append(re.sub(r"\s*([*&<>,])\s*", r"\1", p))
    ret = re.sub(r"\s*([*&<>,])\s*", r"\1", mm.group(1).strip())
    return (ret, tuple(ps), bool(mm.group(3)))
for m in re.finditer(r"(virtual [^;]*?(\w+)\([^;]*;)\s*(?:public:)?\s*//\s*(\d+) 0x", HR):
    SLOT[int(m.group(3))] = m.group(2); SIG[int(m.group(3))] = sig(m.group(1).replace("protected: ", ""), m.group(2))
HMETH = set(re.findall(r"\b(\w+|operator\s*[=!]=?)\s*\(", HR))
RENAME = {"GetThing": "GetPThing"}
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def called(t, name): return re.search(r"(?:\.|->)\s*" + name + r"\s*\(", t) is not None
def fix(t):
    if re.search(r'^\s*#include "engine/CScriptThing.h"', t, re.M) and "CScriptThing_HeaderLayout" not in t:
        return None, "already"
    b = find_block(t, "CScriptThing")
    if not b: return None, "noblock"
    if find_block(t, "CScriptThing", b[1]): return None, "two blocks"
    if find_block(t, "CBaseClass"): return None, "local CBaseClass"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CScriptThing ?(?::\s*public\s+CBaseClass\s*)?\{(.*)\};$", n)
    if not m: return None, "head"
    its = items(m.group(1))
    if its is None: return None, "parse"
    outside = code_of(t[:b[0]] + t[b[1]:])
    slot = 0; pimp = None; renames = {}; bodies = []
    for it in its:
        if "{" in it and INLINE:
            mm = re.fullmatch(r"(virtual )?~CScriptThing\(\) ?\{ ?\}", it)
            if mm:
                if mm.group(1):
                    if slot != 0: return None, "dtor not slot 0"
                    slot += 1
                bodies.append("CScriptThing::~CScriptThing() { }"); continue
            mm = re.fullmatch(r"CScriptThing\(const CScriptThing ?& ?(\w+)\) ?: ?(?:CBaseClass\(\1\) ?, ?)?PImp\(\1\.PImp\) ?\{ ?\}", it)
            if mm:
                bodies.append("CScriptThing::CScriptThing(const CScriptThing& %s) : CBaseClass(%s), PImp(%s.PImp) { }" % ((mm.group(1),) * 3)); continue
            mm = re.fullmatch(r"CScriptThing ?& ?operator ?= ?\(const CScriptThing ?& ?(\w+)\) ?\{ ?PImp ?= ?\1\.PImp; ?return \*this; ?\}", it)
            if mm:
                bodies.append("CScriptThing& CScriptThing::operator=(const CScriptThing& %s) { PImp = %s.PImp; return *this; }" % ((mm.group(1),) * 2)); continue
        if "{" in it: return None, "inline body: " + it[:50]
        if re.fullmatch(r"(public|protected|private)\s*:", it): continue
        if re.fullmatch(r"CScriptThing\(\s*(void)?\s*\);", it): continue
        if re.fullmatch(r"CScriptThing\(const CScriptThing ?& ?\w*\);", it): continue
        if re.fullmatch(r"(?:const )?CScriptThing ?& ?operator ?= ?\(const CScriptThing ?& ?\w*\);", it): continue
        if re.fullmatch(r"bool operator ?[=!]= ?\(const CScriptThing ?& ?\w*\) ?const;", it): continue
        if re.fullmatch(r"~CScriptThing\(\);", it):
            continue
        if re.fullmatch(r"virtual ~CScriptThing\(\);", it):
            if slot != 0: return None, "dtor not slot 0"
            slot += 1; continue
        mm = re.fullmatch(r"virtual [^;()]*?\b(\w+) ?\([^;]*\) ?(?:const)? ?;", it)
        if mm:
            name = mm.group(1)
            if slot not in SLOT: return None, "slot %d beyond 78" % slot
            if re.fullmatch(r"(?:v|s|t|Slot|UnusedSlot|UnusedThingSlot|Unused|Unknown|UnknownSlot)_?\d+|v\d+[a-z]?", name):
                if called(outside, name): return None, "filler called " + name
            elif name == SLOT[slot]: pass
            elif slot in SIG and sig(it, name) == SIG[slot]:
                if re.search(r"\b" + SLOT[slot] + r"\b", outside): return None, "rename target used " + SLOT[slot]
                renames[name] = SLOT[slot]
            elif CASTRET and slot in SIG and sig(it, name) and sig(it, name)[1:] == SIG[slot][1:] and SIG[slot][0].endswith("*") and sig(it, name)[0].endswith("*"):
                # same slot, same parameters: only the row's stand-in pointee type differs (CThing_Retail* for CThing*)
                if re.search(r"\b" + SLOT[slot] + r"\b", outside): return None, "rename target used " + SLOT[slot]
                renames[name] = (SLOT[slot], sig(it, name)[0])
            else: return None, "slot %d %s != %s" % (slot, name, SLOT[slot])
            slot += 1; continue
        if re.fullmatch(r"CCountedPointer ?< ?CScriptThing ?> PImp;", it): pimp = "cp"; continue
        mm = re.fullmatch(r"void ?\* ?(\w+);", it)
        if mm and re.fullmatch(r"PImp_?Data|PImp_?Info|m_pImp\w*|PImp\w*", mm.group(1)):
            if re.search(r"\b" + mm.group(1) + r"\b", outside): return None, "pimp word used " + mm.group(1)
            pimp = (pimp or "") + "w"; continue
        mm = re.fullmatch(r"unsigned char (\w+)\[([^\]]+)\];", it)
        if mm:
            if re.search(r"\b" + mm.group(1) + r"\b", outside): return None, "pad used"
            pimp = (pimp or "") + "pad:" + mm.group(2).replace(" ", ""); continue
        mm = re.fullmatch(r"[\w:<>*& ]+?[ *&]\s*(\w+)\s*\([^{};]*\)\s*(const)?\s*;", it)
        if mm and mm.group(1) in HMETH and mm.group(1) not in SLOT.values(): continue
        return None, "item: " + it[:60]
    if IMPLICIT and pimp == "cp":
        if not any(re.fullmatch(r"(virtual )?~CScriptThing\(\) ?(;|\{ ?\})", i) for i in its) and not any(b.startswith("CScriptThing::~") for b in bodies):
            bodies.append("CScriptThing::~CScriptThing() { }")
        if not any(re.match(r"CScriptThing\(const CScriptThing", i) for i in its):
            bodies.append("CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }")
        if not any(re.search(r"operator ?=", i) for i in its):
            bodies.append("CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }")
    if pimp in ("pad:12", "pad:0xc", "pad:0xC") and slot == 0: pimp = "pod12"
    if pimp not in ("cp", "ww", "pad:8", "pad:12-4", "pad:0x8", "pod12", None):
        return None, "pimp " + str(pimp)
    vptr = slot > 0 or bool(re.search(r"CScriptThing ?: ?public CBaseClass", n)) or "virtual ~CScriptThing" in n
    size = (4 if vptr else 0) + {"cp": 8, "ww": 8, "pad:8": 8, "pad:12-4": 8, "pad:0x8": 8, "pod12": 12, None: 0}[pimp]
    if pimp is not None and size != 12: return None, "size %d" % size
    out = t
    note = []
    # renames at call sites
    for a, z in renames.items():
        if isinstance(z, tuple):
            # `x.GetThing()` -> `((R)x.GetPThing())`: the call expression is the receiver chain before the member access
            z, rt = z; k = 0
            rx = re.compile(r"([A-Za-z_][\w]*(?:\s*(?:\.|->)\s*[A-Za-z_]\w*(?:\(\))?)*?)\s*(\.|->)\s*" + a + r"\s*\(\s*\)")
            out, k = rx.subn(lambda mo: "((%s)%s%s%s())" % (rt, mo.group(1), mo.group(2), z), out)
            if re.search(r"(?:\.|->)\s*" + a + r"\s*\(", code_of(out)): return None, "cast rename left a call"
            note.append("%s->(%s)%s x%d" % (a, rt, z, k)); continue
        out, k = re.subn(r"((?:\.|->)\s*)" + a + r"(\s*\()", r"\1" + z + r"\2", out)
        note.append("%s->%s x%d" % (a, z, k))
    b = find_block(out, "CScriptThing")
    pre = out[:b[0]]
    # CCountedPointer contract
    cpb = find_block(out, "CCountedPointer")
    if cpb and cpb[0] < b[0]:
        if not re.match(r"\s*template", out[cpb[0]:cpb[1]]): return None, "non-template CCountedPointer"
    elif cpb: return None, "CCountedPointer after the block"
    else:
        add = ""
        if not re.search(r"\bclass\s+CCPPointerInfo\b", code_of(pre)): add += "class CCPPointerInfo;\n"
        add += "template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)\n"
        out = out[:b[0]] + add + out[b[0]:]; b = (b[0] + len(add), b[1] + len(add)); note.append("cpmin")
    inc = "// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).\n"
    if "#define FABLE_RETAIL_NSCRIPT" not in out[:b[0]]: inc += "#define FABLE_RETAIL_NSCRIPT\n"
    inc += '#include "engine/CScriptThing.h"\n'
    if bodies:
        inc += "// Retail expands these at this row's sites (the local model had them inline or implicit): defined %s after the header.\n" % INLINE
        inc += "".join("%s %s\n" % (INLINE, x) for x in bodies)
        note.append("inl%d" % len(bodies))
    if b[0] > 0 and out[b[0] - 1] != "\n": inc = "\n" + inc
    out = out[:b[0]] + inc + out[b[1]:]
    # HeaderLayout alias trio
    trio = re.compile(r'#define CScriptThing CScriptThing_HeaderLayout\s*\n#include "engine/CScriptThing.h"[^\n]*\n#undef CScriptThing[^\n]*\n')
    if trio.search(out):
        if out.count("CScriptThing_HeaderLayout") > 1: return None, "HeaderLayout used"
        out = trio.sub("", out); note.append("trio")
    return out, "ok " + (pimp or "-") + " slots%d" % slot + ("" if not note else " " + " ".join(note))
INLINE = None; IMPLICIT = False; CASTRET = False
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    if "--inline" in sys.argv: INLINE = sys.argv[sys.argv.index("--inline") + 1]; sys.argv.remove(INLINE)
    CASTRET = "--castret" in sys.argv   # a slot whose stand-in return pointer type differs: rename + cast at the call
    IMPLICIT = "--implicit" in sys.argv   # the local model's implicit copy / dtor / operator= become explicit inline definitions
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
