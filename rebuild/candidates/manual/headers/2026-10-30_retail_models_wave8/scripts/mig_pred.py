#!/usr/bin/env python3
"""Wave 8 family PRED (wave 7 NEXT item 4, generalises mig_itc.py): row-local NScript predicate models -> the shared
engine/CPredicateBase.h / CIsThingAlive.h / CIsThingConscious.h (CPredicateBase slots 0 operator() / 1 Clone / 2 dtor
0x00CDDB40; CIsThingAlive 0x012C32F0 slots 0x00CDF120 / 0x00CDF130 / 0x00CDF160, CIsThingConscious 0x00CDDAB0 / 0x00CDDAE0 /
0x00CDDB10; Thing +0x04, 0x10). Needs the row's CScriptThing shared. Accepted local items:
 - CPredicateBase: filler / header-named slot declarations (pure or not, empty inline bodies), its inline dtor / ctor;
   a filler the row calls skips.
 - leaves: `CScriptThing Thing;` / `m_thing;` (respelled Thing), the ctor from const CScriptThing& (declared: dropped;
   inline: defined after the include, `inline NScript::C::C(const CScriptThing& t) : Thing(t) { }`, __forceinline kept),
   the dtor (declared: dropped; inline: defined after the include), operator() / Clone declarations, a declared or inline
   member-wise copy ctor (retail has no out-of-line copy: the header's implicit one), filler slots the row never calls.
The include goes at the top level before the first removed block; leaves that were global get `using NScript::C;`."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
from mig_sb import items
from nsutil import top_insert_pos, scope_stack
def code_of(t): return re.sub(r"//[^\n]*", "", t)
FILL = re.compile(r"(?:v|s|slot|Slot|Unknown|UnknownSlot|pad|Pad)_?\d+[a-z]?")
LEAVES = ("CIsThingAlive", "CIsThingConscious")
def in_ns(t, pos):
    st = scope_stack(t, pos)
    if not st: return ""
    m = re.search(r"namespace\s+(\w+)", t[st[-1][1]:st[-1][1] + 200])
    return m.group(1) if m else "?"
def fix(t):
    if not re.search(r'^\s*#include "engine/CScriptThing.h"', t, re.M) or find_block(t, "CScriptThing"): return None, "ST not shared"
    if "CScriptThing_HeaderLayout" in t: return None, "HeaderLayout"
    for h in ("CPredicateBase", "CIsThingAlive", "CIsThingConscious"):
        if re.search(r'#include "engine/%s.h"' % h, t): return None, "already " + h
    present = [c for c in ("CPredicateBase",) + LEAVES if find_block(t, c)]
    if not present: return None, "none"
    for c in present:
        if find_block(t, c, find_block(t, c)[1]): return None, "two " + c
    code = code_of(t); bodies = []; notes = []; usings = []
    blocks = {c: find_block(t, c) for c in present}
    others = code
    for c in present:
        b = blocks[c]; others = others.replace(code_of(t[b[0]:b[1]]), "")
    for c in present:
        b = blocks[c]; n = norm(code_of(t[b[0]:b[1]]))
        ns = in_ns(t, b[0])
        if ns not in ("", "NScript"): return None, c + " in namespace " + ns
        mm = re.match(r"^(?:class|struct) %s ?(?::\s*public\s+(?:NScript::)?CPredicateBase\s*)?\{(.*)\};$" % c, n)
        if not mm: return None, c + " head"
        its = items(mm.group(1))
        if its is None: return None, c + " parse"
        for it in its:
            if re.fullmatch(r"(public|protected|private)\s*:", it): continue
            fi = "__forceinline" if re.match(r"(?:virtual\s+)?__forceinline\b", it) else "inline"
            it = re.sub(r"^(?:inline|__forceinline)\s+", "", it)
            if c == "CPredicateBase":
                if re.fullmatch(r"CPredicateBase\(\) ?(;|\{ ?\})", it): continue
                if re.fullmatch(r"(virtual )?~CPredicateBase\(\) ?(;|\{ ?\})", it): continue
                m2 = re.fullmatch(r"virtual [^;(]*?\b(\w+|operator ?\( ?\)) ?\([^;)]*\) ?(?:const)? ?(?:= ?0)? ?(;|\{ ?\})", it)
                if m2:
                    nm = m2.group(1)
                    if nm in ("Clone",) or nm.replace(" ", "") == "operator()": continue
                    if FILL.fullmatch(nm) or nm == "IsTrue":
                        if re.search(r"(?:\.|->)\s*" + nm + r"\s*\(", others): return None, "base filler called " + nm
                        continue
                    return None, "base slot " + nm
                mm2 = re.fullmatch(r"static void (?:FABLE_FASTCALL |__fastcall )?InitialiseGameInterface\([^)]*\);", it)
                if mm2: continue
                return None, "base item: " + it[:50]
            # leaves
            if re.fullmatch(r"(?:CScriptThing) (?:Thing|m_thing);", it): continue
            m2 = re.fullmatch(r"%s\(const CScriptThing ?& ?(\w*)\) ?;" % c, it)
            if m2: continue
            m2 = re.fullmatch(r"%s\(const CScriptThing ?& ?(\w+)\) ?: ?(?:CPredicateBase\(\) ?, ?)?(?:Thing|m_thing) ?\( ?\1 ?\) ?\{ ?\}" % c, it)
            if m2: bodies.append("%s NScript::%s::%s(const CScriptThing& %s) : Thing(%s) { }" % (fi, c, c, m2.group(1), m2.group(1))); continue
            if re.fullmatch(r"(virtual )?~%s\(\) ?;" % c, it): continue
            if re.fullmatch(r"(virtual )?~%s\(\) ?\{ ?\}" % c, it): bodies.append("%s NScript::%s::~%s() { }" % (fi, c, c)); continue
            if re.fullmatch(r"virtual bool operator ?\( ?\) ?\( ?\) ?const ?;", it): continue
            if re.fullmatch(r"virtual (?:NScript::)?CPredicateBase ?\* ?Clone ?\( ?\) ?const ?;", it): continue
            if re.fullmatch(r"%s\(const %s ?& ?\w*\) ?;" % (c, c), it): notes.append("dropcopy"); continue
            if re.fullmatch(r"%s\(const %s ?& ?(\w+)\) ?: ?(?:CPredicateBase\(\) ?, ?)?(?:Thing|m_thing) ?\( ?\1\.(?:Thing|m_thing) ?\) ?\{ ?\}" % (c, c), it): notes.append("dropcopy"); continue
            m2 = re.fullmatch(r"virtual void (\w+) ?\(\) ?(?:const)? ?(;|\{ ?\})", it)
            if m2 and FILL.fullmatch(m2.group(1)):
                if re.search(r"(?:\.|->)\s*" + m2.group(1) + r"\s*\(", others): return None, "leaf filler called"
                continue
            return None, c + " item: " + it[:50]
        if c in LEAVES and ns == "": usings.append(c)
    if re.search(r"\bm_thing\b", others): return None, "m_thing used outside"
    first = min(blocks[c][0] for c in present)
    ip = top_insert_pos(t, first)
    if ip is None: return None, "not top level"
    out = t
    for c in sorted(present, key=lambda c: -blocks[c][0]):
        b = find_block(out, c); out = out[:b[0]] + out[b[1]:]
    # forward declarations of the leaves / base the row may carry
    leaves = [c for c in LEAVES if c in present or re.search(r"\b%s\b" % c, code_of(out))]
    inc = ("// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; "
           + ", ".join(leaves or ["CPredicateBase"]) + ": CScriptThing Thing +0x04, 0x10).\n")
    if "#define FABLE_RETAIL_NSCRIPT" not in out[:ip]: inc += "#define FABLE_RETAIL_NSCRIPT\n"
    inc += "".join('#include "engine/%s.h"\n' % c for c in (leaves or ["CPredicateBase"]))
    if bodies:
        inc += "// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.\n"
        inc += "".join(x + "\n" for x in bodies)
    inc += "".join("using NScript::%s;\n" % c for c in usings)
    if "CPredicateBase" in present and in_ns(t, blocks["CPredicateBase"][0]) == "" and re.search(r"\bCPredicateBase\b", code_of(out)) and not re.search(r"NScript::CPredicateBase|namespace NScript", code_of(out)):
        inc += "using NScript::CPredicateBase;\n"
    if ip > 0 and out[ip - 1] != "\n": inc = "\n" + inc
    out = out[:ip] + inc + out[ip:]
    out = re.sub(r"^[ \t]*(?:class|struct) (?:CIsThingAlive|CIsThingConscious|CPredicateBase)\s*;[^\n]*\n", "", out, flags=re.M) if not re.search(r"namespace NScript\s*\{\s*(?:class|struct) (?:CIsThingAlive|CIsThingConscious|CPredicateBase)\s*;", out) else out
    return out, "ok " + ",".join(present) + (" inl%d" % len(bodies) if bodies else "") + (" " + " ".join(sorted(set(notes))) if notes else "") + (" using" if usings else "")
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
