#!/usr/bin/env python3
"""Wave 8 family ST2 (extends wave 7 mig_st.py): a row-local CScriptThing becomes engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT)
for the shapes mig_st skipped:
 - PImp stand-ins: `CCountedPointer<P> PImp;` with a stand-in pointee P (CScriptThingImpl / CScriptThingImp / void ...)
   and a CCountedPointer template whose Data word is typed as a stand-in pointee Q (`CScriptEntity* m_pData;`). Ego_r.pdb
   types PImp as CCountedPointer<CScriptThing> (the impl is a CScriptThing subclass, CGameScriptThing, same vtable
   slots), so P / Q go: every slot the row calls through them must sit at the header's index (by name, or renamed when
   the signature at that index is the header's); Q's word becomes `T*`; P / Q blocks go when nothing else names them.
 - inline bodies: the slot / wrapper / ctor / dtor / operator bodies the local model defined in class are defined out of
   class after the include (`inline <ret> CScriptThing::<slot>(..) const { .. }`), where retail expands them; virtual
   ones are index-checked like declarations; filler slots with empty bodies count as fillers. The invented non-virtual
   `CThing* GetThing() const { return PImp.Data ? PImp.Data->GetThing() : 0; }` is retail's slot 11 GetPThing (0x004AAA60:
   `mov ecx,[ecx+4]; test; xor eax,eax; ret` / `jmp [eax+2Ch]`): its calls are renamed GetPThing.
usage: mig_st2.py [--dry] [--implicit] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
from mig_sb import items
import mig_st as B
SLOT, SIG, sig, HMETH = B.SLOT, B.SIG, B.sig, B.HMETH
NAMES = {v: k for k, v in SLOT.items()}
FILLER = re.compile(r"(?:v|s|t|i|e|f|_?slot|Slot|UnusedSlot|UnusedThingSlot|Unused|Unknown|UnknownSlot|UnknownThingSlot|Pad|pad|vf|vfunc|VSlot|_v)_?\d+[a-z]?")
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def called(t, name): return re.search(r"(?:\.|->)\s*" + name + r"\s*\(", t) is not None
def vslots(block):
    """virtual declarations of a stand-in class in order (dtor at its own position): [(idx, name, decl)] or None"""
    n = norm(code_of(block)); m = re.match(r"^(?:class|struct) \w+ ?(?::[^{]*)?\{(.*)\};$", n)
    if not m: return None
    its = items(m.group(1))
    if its is None: return None
    out = []; k = 0
    for it in its:
        if not it.startswith("virtual"): continue
        it2 = re.sub(r"\{.*\}$", ";", it)
        mm = re.fullmatch(r"virtual\s+~\w+\s*\(\s*(?:void)?\s*\)\s*;", it2)
        if mm: out.append((k, "~", it2)); k += 1; continue
        mm = re.fullmatch(r"virtual [^;()]*?\b(\w+) ?\([^;]*\) ?(?:const)? ?(?:= ?0)? ?;", it2)
        if not mm: return None
        out.append((k, mm.group(1), it2)); k += 1
    return out
def map_pointee(t, P, calls_text):
    """check stand-in pointee class P against the header slots for the calls made through it; returns (renames, err)"""
    pb = find_block(t, P)
    called_names = set(re.findall(r"->\s*(\w+)\s*\(", calls_text))
    if not called_names: return {}, None
    if not pb: return None, "pointee %s undefined" % P
    vs = vslots(t[pb[0]:pb[1]])
    if vs is None: return None, "pointee %s parse" % P
    ren = {}
    for idx, name, decl in vs:
        if name not in called_names: continue
        if idx not in SLOT: return None, "pointee slot %d beyond 78" % idx
        if SLOT[idx] == name: continue
        s = sig(decl, name)
        if idx in SIG and s and (s[:2] == SIG[idx][:2] or (s[1:] == SIG[idx][1:] and s[0].endswith("*") and SIG[idx][0].endswith("*"))):
            ren[name] = SLOT[idx]; continue
        return None, "pointee %s slot %d %s != %s" % (P, idx, name, SLOT[idx])
    miss = called_names - set(n for _, n, _ in vs)
    if miss: return None, "pointee %s lacks %s" % (P, ",".join(sorted(miss)))
    return ren, None
def drop_block(out, name):
    b = find_block(out, name)
    if not b: return out
    out = out[:b[0]] + out[b[1]:]
    out = re.sub(r"^[ \t]*(?:class|struct) " + name + r"\s*;[^\n]*\n", "", out, flags=re.M)
    return out
INL = re.compile(r"(?:(virtual)\s+)?(?:(?:inline|__forceinline)\s+)?([\w:<>*& ]+?)\s*\b(\w+)\s*\(([^()]*)\)\s*(const)?\s*(\{.*\})$", re.S)
def fix(t):
    if re.search(r'^\s*#include "engine/CScriptThing.h"', t, re.M) and "CScriptThing_HeaderLayout" not in t:
        return None, "already"
    b = find_block(t, "CScriptThing")
    if not b: return None, "noblock"
    if find_block(t, "CScriptThing", b[1]): return None, "two blocks"
    if find_block(t, "CBaseClass"): return None, "local CBaseClass"
    if re.search(r"\bInlineCopy\b", t): return None, "InlineCopy tag (mixed inlining residue)"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CScriptThing ?(?::\s*public\s+CBaseClass\s*)?\{(.*)\};$", n)
    if not m: return None, "head"
    its = items(m.group(1))
    if its is None: return None, "parse"
    outside = code_of(t[:b[0]] + t[b[1]:])
    # slot counting: with `: public CBaseClass` the dtor is inherited at slot 0 (CBaseClass.h: virtual ~CBaseClass), so the
    # first own virtual is slot 1 and a dtor declared anywhere overrides slot 0; without a base the dtor takes its own place
    cb = bool(re.search(r"CScriptThing ?: ?public CBaseClass", n))
    f0 = False
    slot = 1 if cb else 0; pimp = None; P = None; renames = {}; bodies = []; notes = []
    for it in its:
        if re.fullmatch(r"(public|protected|private)\s*:", it): continue
        fi = bool(re.match(r"(?:virtual\s+)?__forceinline\b", it))
        it = re.sub(r"^(?:inline|__forceinline)\s+", "", it); it = re.sub(r"^virtual\s+(?:inline|__forceinline)\s+", "virtual ", it)
        if "{" in it:
            nb0 = len(bodies)
            mm = re.fullmatch(r"(virtual )?~CScriptThing\(\s*\) ?(\{.*\})", it)
            if mm:
                if mm.group(1) and not cb:
                    if slot != 0:
                        # a late dtor after a slot-0 filler: the row's local vtable order put the filler at slot 0 (the
                        # dtor's retail place) and the dtor last; the row never calls either through the vtable
                        if not f0: return None, "dtor not slot 0"
                        notes.append("latedtor")
                    else: slot += 1
                bodies.append(("@FI@" if fi else "") + "CScriptThing::~CScriptThing() " + mm.group(2)); continue
            mm = re.fullmatch(r"CScriptThing\(\s*(?:void)?\s*\) ?((?::[^{]*)?\{.*\})", it)
            if mm: bodies.append(("@FI@" if fi else "") + "CScriptThing::CScriptThing() " + mm.group(1)); continue
            mm = re.fullmatch(r"CScriptThing\(const CScriptThing ?& ?(\w+)\) ?((?::[^{]*)?\{.*\})", it)
            if mm: bodies.append(("@FI@" if fi else "") + "CScriptThing::CScriptThing(const CScriptThing& %s) %s" % (mm.group(1), mm.group(2))); continue
            mm = re.fullmatch(r"((?:const )?CScriptThing ?&) ?operator ?= ?\((const CScriptThing ?& ?\w+)\) ?(\{.*\})", it)
            if mm: bodies.append(("@FI@" if fi else "") + "CScriptThing& CScriptThing::operator=(%s) %s" % (mm.group(2), mm.group(3))); continue
            mm = re.fullmatch(r"bool operator ?([=!])= ?\((const CScriptThing ?& ?\w+)\) ?const ?(\{.*\})", it)
            if mm: bodies.append(("@FI@" if fi else "") + "bool CScriptThing::operator%s=(%s) const %s" % (mm.group(1), mm.group(2), mm.group(3))); continue
            mm = INL.fullmatch(it)
            if not mm: return None, "inline body: " + it[:50]
            virt, ret, name, params, const, body = mm.groups()
            decl = "%s %s(%s)%s;" % (ret, name, params, " const" if const else "")
            if virt:
                if slot not in SLOT: return None, "slot %d beyond 78" % slot
                if FILLER.fullmatch(name) and not (name in NAMES):
                    if called(outside, name) or re.search(r"\b" + name + r"\s*\(", code_of(body)): return None, "filler called " + name
                    if slot == 0: f0 = True
                    slot += 1; continue
                hn = SLOT[slot]
                if name != hn:
                    s = sig("virtual " + decl, name)
                    if slot in SIG and s == SIG[slot]:
                        if re.search(r"\b" + hn + r"\b", outside): return None, "rename target used " + hn
                        renames[name] = hn
                    else: return None, "slot %d %s != %s" % (slot, name, hn)
                slot += 1
            else:
                if name == "GetThing" and sig(decl, name) == SIG[11]: hn = "GetPThing"; renames[name] = hn
                elif name in NAMES or name in HMETH: hn = name
                else: return None, "inline nonheader " + name
            bodies.append(("@FI@" if fi else "") + "%s CScriptThing::%s(%s)%s %s" % (ret.strip(), hn, params, " const" if const else "", body)); continue
        if re.fullmatch(r"CScriptThing\(\s*(void)?\s*\);", it): continue
        if re.fullmatch(r"CScriptThing\(const CScriptThing ?& ?\w*\);", it): continue
        if re.fullmatch(r"(?:const )?CScriptThing ?& ?operator ?= ?\(const CScriptThing ?& ?\w*\);", it): continue
        if re.fullmatch(r"bool operator ?[=!]= ?\(const CScriptThing ?& ?\w*\) ?const;", it): continue
        if re.fullmatch(r"~CScriptThing\(\);", it): continue
        if re.fullmatch(r"virtual ~CScriptThing\(\);", it):
            if cb: continue
            if slot != 0:
                        # a late dtor after a slot-0 filler: the row's local vtable order put the filler at slot 0 (the
                        # dtor's retail place) and the dtor last; the row never calls either through the vtable
                        if not f0: return None, "dtor not slot 0"
                        notes.append("latedtor"); continue
            slot += 1; continue
        mm = re.fullmatch(r"virtual [^;()]*?\b(\w+) ?\([^;]*\) ?(?:const)? ?;", it)
        if mm:
            name = mm.group(1)
            if slot not in SLOT: return None, "slot %d beyond 78" % slot
            if FILLER.fullmatch(name) and name not in NAMES:
                if called(outside, name): return None, "filler called " + name
                if slot == 0: f0 = True
            elif name == SLOT[slot]:
                s = sig(it, name)
                if slot in SIG and s and s[0] != SIG[slot][0]:
                    if s[1:] == SIG[slot][1:] and s[0].endswith("*") and SIG[slot][0].endswith("*"): renames[name] = (name, s[0])
                    elif s[:2] != SIG[slot][:2]: return None, "slot %d %s signature %s" % (slot, name, s)
            elif slot in SIG and sig(it, name) and sig(it, name)[1:] == SIG[slot][1:] and SIG[slot][0].endswith("*") and sig(it, name)[0].endswith("*") and sig(it, name)[0] != SIG[slot][0]:
                if re.search(r"\b" + SLOT[slot] + r"\b", outside): return None, "rename target used " + SLOT[slot]
                renames[name] = (SLOT[slot], sig(it, name)[0])
            elif slot in SIG and sig(it, name) == SIG[slot]:
                if re.search(r"\b" + SLOT[slot] + r"\b", outside): return None, "rename target used " + SLOT[slot]
                renames[name] = SLOT[slot]
            else: return None, "slot %d %s != %s" % (slot, name, SLOT[slot])
            slot += 1; continue
        mm = re.fullmatch(r"CCountedPointer ?< ?(\w+) ?> PImp;", it)
        if mm: pimp = "cp"; P = mm.group(1); continue
        mm = re.fullmatch(r"(?:void|CScriptThing|CThing_Retail|CThing|CScriptThingImpl?|CCPPointerInfo|SScriptThingInfo|SCountBlock|SCount|CScriptThingImpInfo) ?\* ?(\w+);", it)
        if mm and re.fullmatch(r"PImp_?Data|PImp_?Info|m_pImp\w*|PImp\w*|m_pData|m_pInfo|m_pCount|m_pThing|Data|Info", mm.group(1)):
            if re.search(r"\b" + mm.group(1) + r"\b", outside): return None, "pimp word used " + mm.group(1)
            pimp = (pimp or "") + "w"; continue
        mm = re.fullmatch(r"unsigned char (\w+)\[([^\]]+)\];", it)
        if mm:
            if re.search(r"\b" + mm.group(1) + r"\b", outside): return None, "pad used"
            pimp = (pimp or "") + "pad:" + mm.group(2).replace(" ", ""); continue
        mm = re.fullmatch(r"[\w:<>*& ]+?[ *&]\s*(\w+)\s*\([^{};]*\)\s*(const)?\s*;", it)
        if mm and (mm.group(1) in HMETH or mm.group(1) in NAMES): continue
        return None, "item: " + it[:60]
    if B.IMPLICIT and pimp == "cp":
        if not any(x.replace("@FI@", "").startswith("CScriptThing::~") for x in bodies) and not any(re.fullmatch(r"(virtual )?~CScriptThing\(\);", i) for i in its):
            bodies.append("CScriptThing::~CScriptThing() { }")
        if not any(re.match(r"CScriptThing\(const CScriptThing", i) for i in its):
            bodies.append("CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }")
        if not any(re.search(r"operator ?=", i) for i in its):
            bodies.append("CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }")
    if pimp in ("pad:12", "pad:0xc", "pad:0xC") and slot == 0: pimp = "pod12"
    if pimp not in ("cp", "ww", "pad:8", "pad:12-4", "pad:0x8", "pod12", None): return None, "pimp " + str(pimp)
    vptr = slot > 0 or cb or bool(re.search(r"CScriptThing ?: ?public CBaseClass", n)) or "virtual ~CScriptThing" in n
    size = (4 if vptr else 0) + {"cp": 8, "ww": 8, "pad:8": 8, "pad:12-4": 8, "pad:0x8": 8, "pod12": 12, None: 0}[pimp]
    if pimp is not None and size != 12: return None, "size %d" % size
    out = t
    for a, z in renames.items():
        if isinstance(z, tuple): continue
        bodies = [re.sub(r"((?:\.|->)\s*)" + a + r"(\s*\()", r"\1" + z + r"\2", x) for x in bodies]
    words = set(re.findall(r"\b(\w+)\s*;", " ".join(i for i in its if "{" not in i and "(" not in i))) - {"PImp"}
    for x in bodies:
        w = [v for v in words if re.search(r"\b" + v + r"\b", x)]
        if w: return None, "moved body uses local word " + w[0]
    body_text = "\n".join(bodies)
    # the CCountedPointer template: contract + a stand-in Data word type Q
    cpb = find_block(out, "CCountedPointer")
    Q = None
    if cpb and cpb[0] < b[0]:
        cpt = out[cpb[0]:cpb[1]]
        if not re.match(r"\s*template", cpt): return None, "non-template CCountedPointer"
        mm = re.search(r"^\s*(?:template\s*<\s*class\s+(\w+)\s*>)", cpt)
        Tn = mm.group(1) if mm else "T"
        dm = re.search(r"^([ \t]*)(?:(?:class|struct)\s+)?(\w+)\s*\*(\s*)(\w+)\s*;", code_of(cpt[cpt.index("{") + 1:]), re.M)
        if dm and dm.group(2) not in (Tn, "CScriptThing", "CCPPointerInfo", "SCountBlock", "SCount", "void", "long", "int", "unsigned") and find_block(out, dm.group(2)):
            Q = dm.group(2)
    elif cpb: return None, "CCountedPointer after the block"
    # calls through the pointee stand-ins (in the bodies that move, and anywhere else through PImp)
    pnames = [x for x in (P, Q) if x and x not in ("CScriptThing",)]; keep = set()
    pren = {}
    for X in pnames:
        others = code_of(out[:b[0]] + out[b[1]:])
        xb = find_block(out, X)
        if xb: others = code_of(out[:xb[0]] + out[xb[1]:b[0]] + out[b[1]:])
        if X == Q:
            cpb2 = find_block(out, "CCountedPointer"); others = code_of(out[:cpb2[0]] + out[cpb2[1]:b[0]] + out[b[1]:])
            if xb: others = code_of(out[:xb[0]] + out[xb[1]:cpb2[0]] + out[cpb2[1]:b[0]] + out[b[1]:])
        others = re.sub(r"(?:class|struct)\s+" + X + r"\s*;", "", others)
        if re.search(r"\b" + X + r"\b", others):
            if X == Q: return None, "pointee %s named elsewhere" % X
            keep.add(X)
        if X == "void": continue
        r, err = map_pointee(out, X, body_text)
        if err: return None, err
        pren.update(r)
    for a, z in pren.items():
        bodies = [re.sub(r"(->\s*)" + a + r"(\s*\()", r"\1" + z + r"\2", x) for x in bodies]
        notes.append("pointee %s->%s" % (a, z))
    if P and P not in ("CScriptThing", "void") and P != Q:
        if P in keep: notes.append("keepP " + P)
        else: out = drop_block(out, P); notes.append("dropP " + P)
    elif P == "void": notes.append("Pvoid")
    if Q:
        cpb = find_block(out, "CCountedPointer"); cpt = out[cpb[0]:cpb[1]]
        Tn = (re.search(r"template\s*<\s*class\s+(\w+)\s*>", cpt) or [None, "T"])[1]
        cpt2 = re.sub(r"\b(?:(?:class|struct)\s+)?" + Q + r"\s*\*", Tn + "* ", cpt, count=1)
        out = out[:cpb[0]] + cpt2 + out[cpb[1]:]
        out = drop_block(out, Q); notes.append("dropQ " + Q)
    # renames at call sites outside the block
    for a, z in renames.items():
        if isinstance(z, tuple):
            # `x.GetThing()` -> `((R)x.GetPThing())`: the stand-in returned its own pointee type (wave 7 castret)
            z, rt = z
            rx = re.compile(r"([A-Za-z_][\w]*(?:\s*(?:\.|->)\s*[A-Za-z_]\w*(?:\(\))?)*?)\s*(\.|->)\s*" + a + r"\s*\(\s*\)")
            b0 = find_block(out, "CScriptThing")
            seg, k = rx.subn(lambda mo: "((%s)%s%s%s())" % (rt, mo.group(1), mo.group(2), z), out[b0[1]:])
            out = out[:b0[1]] + seg
            if re.search(r"(?:\.|->)\s*" + a + r"\s*\((?!\)\)\))", code_of(out[b0[1]:]).replace("((%s)" % rt, "")) and a != z: return None, "cast rename left a call"
            notes.append("%s->(%s)%s x%d" % (a, rt, z, k)); continue
        out, k = re.subn(r"((?:\.|->)\s*)" + a + r"(\s*\()", r"\1" + z + r"\2", out)
        notes.append("%s->%s x%d" % (a, z, k))
    b = find_block(out, "CScriptThing")
    cpb = find_block(out, "CCountedPointer")
    if not cpb:
        pre = out[:b[0]]; add = ""
        if not re.search(r"\bclass\s+CCPPointerInfo\b|\bstruct\s+CCPPointerInfo\b", code_of(pre)): add += "class CCPPointerInfo;\n"
        add += "template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)\n"
        out = out[:b[0]] + add + out[b[0]:]; b = (b[0] + len(add), b[1] + len(add)); notes.append("cpmin")
    inc = "// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).\n"
    if "#define FABLE_RETAIL_NSCRIPT" not in out[:b[0]]: inc += "#define FABLE_RETAIL_NSCRIPT\n"
    inc += '#include "engine/CScriptThing.h"\n'
    if bodies:
        inc += "// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.\n"
        inc += "".join(("__forceinline %s\n" % x[4:]) if x.startswith("@FI@") else ("inline %s\n" % x) for x in bodies)
        notes.append("inl%d" % len(bodies))
    if b[0] > 0 and out[b[0] - 1] != "\n": inc = "\n" + inc
    out = out[:b[0]] + inc + out[b[1]:]
    trio = re.compile(r'#define CScriptThing CScriptThing_HeaderLayout\s*\n#include "engine/CScriptThing.h"[^\n]*\n#undef CScriptThing[^\n]*\n')
    if trio.search(out):
        if out.count("CScriptThing_HeaderLayout") > 1: return None, "HeaderLayout used"
        out = trio.sub("", out); notes.append("trio")
    return out, "ok " + (pimp or "-") + (":" + P if P else "") + " slots%d" % slot + ("" if not notes else " " + " ".join(notes))
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    B.IMPLICIT = "--implicit" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
