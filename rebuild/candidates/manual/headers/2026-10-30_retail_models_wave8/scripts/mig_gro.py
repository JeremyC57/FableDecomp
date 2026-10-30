#!/usr/bin/env python3
"""Wave 5 family GRO: row-local CScriptGameResourceObjectBase / ...ScriptedThingBase / ...MovieBase (and the row's local
CBaseObject, via mig_bo) become the retail headers under FABLE_RETAIL_BASE_OBJECT (engine/CScriptGameResourceObjectBase.h,
...ScriptedThingBase.h retail branch, ...MovieBase.h). Accepted block items: access labels, trivial ctor / dtor forms, the
PImp member (any by-value pointer-pair model; the header's is CCountedPointer<T> from the row's own template), method
declarations the header has (any spelling: the compile decides), `bool IsScripting();` (retail IsValid() const 0x00CD23B9:
call sites respelled), operator= declarations. Rejects stand-in virtuals and helper members. The STB HeaderLayout alias
include is removed. Idempotent. usage: mig_gro.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
import mig_bo
B, STB, MB = "CScriptGameResourceObjectBase", "CScriptGameResourceObjectScriptedThingBase", "CScriptGameResourceObjectMovieBase"
HDR = {B: "engine/CScriptGameResourceObjectBase.h", STB: "engine/CScriptGameResourceObjectScriptedThingBase.h",
       MB: "engine/CScriptGameResourceObjectMovieBase.h"}
STB_METHODS = set("""SummonerLightningOrbAttackTarget FireProjectileWeaponAtTarget MoveToPosition MoveToThing FollowPreCalculatedRoute
FollowThing StopFollowingThing IsFollowActionRunning ClearCommands WaitWhilePerformingTasks GetScriptThing Speak AskHeroQuestion
Converse PerformExpression PlayAnimation PlayCombatAnimation PlayLoopingAnimation ClearAllActions ClearAllActionsIncludingLoopingAnimations
MoveToAndPickUpGenericBox DropGenericBox UnsheatheWeapons IsPerformingScriptTask IsFollowingThing WaitForEntityToFinishPerformingTasks
Wait Release IsValid""".split())
ALIAS = re.compile(r'[ \t]*#define (\w+) \1_HeaderLayout[^\n]*\n[ \t]*#include "engine/\1.h"[^\n]*\n[ \t]*#undef \1[^\n]*\n')
def items(body):
    out = []; d = 0; cur = ""
    for ch in body:
        cur += ch
        if ch == "{": d += 1
        elif ch == "}":
            d -= 1
            if d == 0: out.append(cur.strip()); cur = ""
        elif ch == ";" and d == 0: out.append(cur.strip()); cur = ""
        elif ch == ":" and d == 0 and re.search(r"\b(public|protected|private)\s*:$", cur): out.append(cur.strip()); cur = ""
    if cur.strip(): return None
    return [i for i in out if i and i != ";"]
def ok_item(cls, it):
    s = cls
    if re.fullmatch(r"(public|protected|private)\s*:", it): return True
    if re.fullmatch(r"(?:__forceinline )?" + s + r"\(\) ?(?:: ?CBaseObject\(\) ?, ?PImp\(\) ?)?\{ ?(?:PImp\.Data ?= ?0; ?PImp\.Info ?= ?0; ?)?\}", it): return True
    if re.fullmatch(s + r"\(\);", it): return True
    if re.fullmatch(r"(?:(?:virtual|__forceinline) ?)*~" + s + r"\(\) ?(?:;|\{ ?\})", it): return True
    if re.fullmatch(r"(?:__forceinline )?" + s + r" ?& ?operator ?= ?\(const " + s + r" ?& ?\w*\);", it): return True
    if cls in (STB, MB) and re.fullmatch(r"(?:CCountedPointer<[\w ]+\*?>|CCountedPointerWords|CMovieImpPointer|CMovieImpWords|CResourcePImp) PImp;", it): return True
    if cls == STB:
        m = re.match(r"(?:virtual )?[\w:<> ]+?[ &*]\s*(\w+) ?\(", it)
        if m and it.endswith(";") and "{" not in it and (m.group(1) in STB_METHODS or m.group(1) == "IsScripting"): return True
    if cls == MB and re.fullmatch(r"void Release\(\);", it): return True
    return False
def fix(t):
    if any(re.search(r'#include "' + h + '"', t) and find_block(t, c) is None for c, h in HDR.items() if c != STB): return None, "already"
    found = [c for c in (B, STB, MB) if find_block(t, c)]
    if not found: return None, "noblock"
    if not re.search(r"template\s*<\s*(?:class|typename) \w+\s*>\s*(?:class|struct)\s+CCountedPointer\b[^;]*\{", t): return None, "no CCountedPointer template"
    if find_block(t, "CBaseObject"):
        r, why = mig_bo.fix(t)
        if r is None: return None, "bo: " + why
        t = r
    if "FABLE_RETAIL_BASE_OBJECT" not in t: return None, "no CBaseObject model"
    t, na = ALIAS.subn(lambda m: "" if m.group(1) in (STB, MB, B) else m.group(0), t)
    for c in found:
        b = find_block(t, c)
        if find_block(t, c, b[1]): return None, "two blocks " + c
        n = norm(t[b[0]:b[1]])
        m = re.match(r"^(?:class|struct) " + c + r" ?(:\s*public\s+(\w+)\s*)?\{(.*)\};$", n)
        if not m: return None, "head " + c
        want = {B: "CBaseObject", STB: B, MB: B}[c]
        if m.group(2) not in (want, "CBaseObject" if c != B else want): return None, f"base {c}:{m.group(2)}"
        its = items(m.group(3))
        if its is None: return None, "parse " + c
        for it in its:
            if not ok_item(c, it): return None, f"item {c[26:]}: {it[:70]}"
    # place the includes at the first block, cut the others
    firstpos = min(find_block(t, c)[0] for c in found)
    need = set(found)
    if STB in need or MB in need: need.add(B)
    inc = "".join(f'#include "{HDR[c]}"\n' for c in (B, STB, MB) if c in need and f'#include "{HDR[c]}"' not in t)
    out = t
    for c in sorted(found, key=lambda c: -find_block(out, c)[0]):
        b = find_block(out, c)
        out = out[:b[0]] + (inc if b[0] == firstpos else "") + out[b[1]:]
    if re.search(r'#include "engine/CScriptGameResourceObjectScriptedThingBase.h"', out) and out.find("FABLE_RETAIL_BASE_OBJECT") > out.find('#include "engine/CScriptGameResourceObjectScriptedThingBase.h"'):
        return None, "flattened STB include before the opt-in"
    code = re.sub(r"//[^\n]*", "", out)
    nr = 0
    if re.search(r"\bIsScripting\b", code):
        out, nr = re.subn(r"(\.|->)\s*IsScripting\s*\(", r"\1IsValid(", out)
    return out, f"ok {'+'.join(x[26:] or 'B' for x in found)} alias={na} isvalid={nr}"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
