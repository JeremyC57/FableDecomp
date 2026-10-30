#!/usr/bin/env python3
"""Wave 8 (family ESB2, extends wave 7 mig_esb.py): blocks inside namespaces (the include goes above the outermost
namespace; a flattened engine/CScriptThing.h include becomes the retail one), derived classes that model the base's
vptr / Game / Me words themselves (dropped with the base), the inline `virtual ~CEntityScriptBase() { }` (slot 0; the
header's is the out-of-line 0x00F35B40) and a flattened `void* __vftable;` base word.
Wave 7 item 4 (family ESB): a row-local CEntityScriptBase becomes engine/CEntityScriptBase.h (FABLE_RETAIL_NSCRIPT:
vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08; inline ctor). Needs the row's CScriptThing to be the shared
one already (family ST). Accepted local items: the inline ctor `: Game(&g), Me(m) {}` (the header's) or its declaration,
`virtual ~CEntityScriptBase();` (slot 0), virtual slots by local declaration order with the header's name at that
index (or an uncalled filler), `CGameScriptInterfaceBase* Game;` / `CGameScriptInterface* Game;` (the PDB type is the
base: the script calls go through the interface base, same slots), `CScriptThing Me;`, declarations of
IsActiveThreadTerminating() const / SetPredicate(...). Inline bodies other than the ctor skip.
usage: mig_esb.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
from mig_sb import items
from nsutil import top_insert_pos, mask
SLOT = ["~CEntityScriptBase", "Main", "Init", "GetParentScript", "OnPersist", "OnPredicateFail", "OnInterrupted"]
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def called(t, name): return re.search(r"(?:\.|->)\s*" + name + r"\s*\(", t) is not None
def fix(t):
    if re.search(r'^\s*#include "engine/CEntityScriptBase.h"', t, re.M): return None, "already"
    mi = re.search(r'^\s*#include "engine/CScriptThing.h"', t, re.M)
    if not mi or find_block(t, "CScriptThing"):
        return None, "CScriptThing not shared"
    flat = "#define FABLE_RETAIL_NSCRIPT" not in t[:mi.start()]
    b = find_block(t, "CEntityScriptBase")
    if not b: return None, "noblock"
    if find_block(t, "CEntityScriptBase", b[1]): return None, "two blocks"
    # a header cannot be included inside a class / function: only namespaces may enclose the block (wave 8: the include
    # goes above the outermost namespace; the header's class is the global one, as both PDBs spell it)
    ipos = top_insert_pos(t, b[0])
    if ipos is None: return None, "block not at top level"
    if re.search(r"\w+\s*::\s*CEntityScriptBase\b", code_of(t)): return None, "qualified CEntityScriptBase"
    if flat:
        if mi.start() > ipos: return None, "CScriptThing include inside the namespace"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CEntityScriptBase ?\{(.*)\};$", n)
    if not m: return None, "head"
    its = items(m.group(1))
    if its is None: return None, "parse"
    outside = code_of(t[:b[0]] + t[b[1]:])
    slot = 0; mem = []; gsi = False; bad = []; ren = {}; inl_dtor = False
    for it in its:
        if re.fullmatch(r"(public|protected|private)\s*:", it): continue
        if re.fullmatch(r"CEntityScriptBase\s*\(\s*CGameScriptInterfaceBase\s*&\s*(\w+)\s*,\s*const CScriptThing\s*&\s*(\w+)\s*\)\s*:\s*Game\s*\(\s*&\s*\1\s*\)\s*,\s*Me\s*\(\s*\2\s*\)\s*\{\s*\}", it): continue
        if re.fullmatch(r"CEntityScriptBase\s*\(\s*CGameScriptInterfaceBase\s*&\s*\w*\s*,\s*const CScriptThing\s*&\s*\w*\s*\)\s*;", it): continue
        if re.fullmatch(r"virtual ~CEntityScriptBase\(\) ?\{ ?\}", it): it = "virtual ~CEntityScriptBase();"; inl_dtor = True
        if it.startswith("virtual ") and not it.startswith("virtual ~") and re.search(r"\)\s*(?:const\s*)?\{\s*\}$", it):
            # an empty in-class body of a slot (filler or header slot): only the slot's place matters to the row (wave 8)
            it = re.sub(r"\)\s*(const\s*)?\{\s*\}$", lambda mo: ")" + (" const" if mo.group(1) else "") + ";", it); inl_dtor = True
        if "{" in it: return None, "inline body: " + it[:50]
        if re.fullmatch(r"void ?\* ?__vftable;", it):
            if slot != 0 or mem: return None, "__vftable not first"
            if re.search(r"\b__vftable\b", outside): return None, "__vftable used"
            slot = -1; continue
        if re.fullmatch(r"virtual ~CEntityScriptBase\(\);", it):
            if slot != 0: return None, "dtor not slot 0"
            slot += 1; continue
        mm = re.fullmatch(r"virtual [^;()]*?\b(\w+) ?\([^;]*\) ?(?:const)? ?(?:= ?0)? ?;", it)
        if mm:
            name = mm.group(1)
            if slot < 0: return None, "virtual after __vftable"
            if slot >= len(SLOT): return None, "slot %d beyond 6" % slot
            if re.fullmatch(r"(?:v|e|s|Slot|UnknownSlot|UnknownEntitySlot|Unknown)_?\d+", name):
                if called(outside, name): return None, "filler called " + name
            elif name != SLOT[slot]:
                # a misplaced / invented slot name is harmless when nothing calls it through the base (the row only
                # overrides it): calls are `.x(` / `->x(` or a bare statement call `x();`
                if called(outside, name) or re.search(r"(?:^|[;{}])\s*" + name + r"\s*\([^;{}]*\)\s*;", outside, re.M):
                    # an invented name the row calls: rename the calls to the header's slot name when both are
                    # `void()` slots and no other class of the row declares the invented name
                    if re.fullmatch(r"virtual void \w+ ?\(\) ?;", it) and SLOT[slot] in ("Main", "Init", "OnPredicateFail", "OnInterrupted") \
                       and not re.search(r"\b(?:void|bool|int|long|unsigned|float)\s+(?:\w+::)*" + name + r"\s*\(", outside) and not re.search(r"(?:\.|->)\s*" + SLOT[slot] + r"\s*\(", outside):
                        ren[name] = SLOT[slot]; slot += 1; continue
                    return None, "slot %d %s != %s (called)" % (slot, name, SLOT[slot])
                if name == "Main" or (name in SLOT and name not in ("Main",)): pass
                bad.append(name)
            slot += 1; continue
        if re.fullmatch(r"CGameScriptInterfaceBase ?\* ?Game;", it): mem.append("Game"); continue
        if re.fullmatch(r"CGameScriptInterface ?\* ?Game;", it): mem.append("Game"); gsi = True; continue
        if re.fullmatch(r"CScriptThing Me;", it): mem.append("Me"); continue
        if re.fullmatch(r"bool IsActiveThreadTerminating\(\) ?const ?;", it): continue
        if re.fullmatch(r"void SetPredicate\(const (?:NScript::)?CPredicateBase ?& ?\w*\) ?;", it): continue
        return None, "item: " + it[:60]
    vft = slot == -1
    if vft:
        slot = 0
        if mem != ["Game", "Me"]: return None, "__vftable without Game/Me"
    if mem not in ([], ["Game", "Me"]): return None, "members " + ",".join(mem)
    # derived entity classes: none may model the base's own words (vptr / Game / Me) itself
    ders = []; dropw = []
    for dm in re.finditer(r"(?:class|struct)\s+(\w+)\s*:\s*public\s+CEntityScriptBase\b", outside):
        db = find_block(t, dm.group(1))
        if not db: continue
        dn = norm(code_of(t[db[0]:db[1]]))
        if re.search(r"__vftable|\bvoid ?\* ?vtbl|\bGame ?;|\bMe ?;|_base\w*\[|_pad0+\[", dn):
            # wave 8: the derived class models the base's words itself; drop them when they are exactly the base's
            # (vptr word only when the local base declares no virtual, then Game, then Me, first in the class)
            if mem or vft: return None, "derived models base words (base has members) " + dm.group(1)
            dropw.append((dm.group(1), slot == 0))
        ders.append(dm.group(1))
    out = t
    for d, wantv in dropw:
        db = find_block(out, d); body = out[db[0]:db[1]]
        hb = body.index("{") + 1
        rx = (r"(\s*(?:public\s*:\s*)?)" + (r"[ \t]*void\s*\*\s*__vftable\s*;[^\n]*\n" if wantv else "") +
              r"[ \t]*CGameScriptInterface(?:Base)?\s*\*\s*Game\s*;[^\n]*\n[ \t]*CScriptThing\s+Me\s*;[^\n]*\n")
        mm = re.match(rx, body[hb:])
        if not mm: return None, "derived models base words (shape) " + d
        body = body[:hb] + mm.group(1) + body[hb + mm.end():]
        if re.search(r"\b__vftable\b", code_of(out[:db[0]] + body + out[db[1]:])): return None, "__vftable used " + d
        out = out[:db[0]] + body + out[db[1]:]
        bad.append("dropwords:" + d)
    # an entity class the row instantiates must not stay abstract: retail declares Main (private, EAE) and
    # GetParentScript (UBE) in every entity class (both PDBs); add the declarations the local model left out
    added = []
    for d in ders:
        if not re.search(r"\bnew\s+(?:\w+::)*" + d + r"\s*\(", code_of(out)): continue
        db = find_block(out, d); body = out[db[0]:db[1]]
        add = ""
        if not re.search(r"\bvoid\s+Main\s*\(", code_of(body)): add += "private:\n    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)\n"
        if not re.search(r"\bGetParentScript\s*\(", code_of(body)): add += "public:\n    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)\n"
        if add:
            j = body.rindex("}")
            body = body[:j] + add + body[j:]
            out = out[:db[0]] + body + out[db[1]:]; added.append(d)
    b = find_block(out, "CEntityScriptBase")
    for a, z in ren.items():
        out, k = re.subn(r"((?:\.|->)\s*)" + a + r"(\s*\()", r"\1" + z + r"\2", out); bad.append("%s->%s x%d" % (a, z, k))
    if flat:
        # the row included the flattened generated CScriptThing; the entity base needs the retail one (wave 7 contract):
        # the global CCountedPointer<T> it names (Ego_r members Data, Info) comes first; the row's namespace-local
        # CCountedPointer still hides it inside the namespace
        mi = re.search(r'^[ \t]*#include "engine/CScriptThing.h"[^\n]*\n', out, re.M)
        rep = ""
        if not re.search(r"^(?:class|struct) CCPPointerInfo\b", mask(out[:mi.start()]), re.M): rep += "class CCPPointerInfo;\n"
        if not re.search(r"^template\s*<[^>]*>\s*(?:class|struct)\s+CCountedPointer\b", mask(out[:mi.start()]), re.M):
            rep += "template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)\n"
        rep += "// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).\n#define FABLE_RETAIL_NSCRIPT\n" + mi.group(0)
        out = out[:mi.start()] + rep + out[mi.end():]
        bad.append("flatST")
    b = find_block(out, "CEntityScriptBase")
    ip = top_insert_pos(out, b[0])
    inc = "// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).\n"
    if "#define FABLE_RETAIL_NSCRIPT" not in out[:ip]: inc += "#define FABLE_RETAIL_NSCRIPT\n"
    inc += '#include "engine/CEntityScriptBase.h"\n'
    if ip == b[0]:
        if b[0] > 0 and out[b[0] - 1] != "\n": inc = "\n" + inc
        out = out[:b[0]] + inc + out[b[1]:]
    else:
        out = out[:b[0]] + out[b[1]:]
        out = out[:ip] + inc + out[ip:]
        bad.append("ns")
    return out, "ok slots%d %s%s%s%s%s" % (slot, "mem" if mem else "nomem", " gsi" if gsi else "", " inldtor" if inl_dtor else "", " misplaced:" + ",".join(bad) if bad else "", " concrete%d" % len(added) if added else "")
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
