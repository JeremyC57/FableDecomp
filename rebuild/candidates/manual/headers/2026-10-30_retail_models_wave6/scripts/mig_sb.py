#!/usr/bin/env python3
"""Wave 6 family SB (NScript bulk, wave 1 NEXT (a)-(d)): a row-local CScriptBase (and its local CLevelScriptBase<T>)
becomes engine/CScriptBase.h / engine/CLevelScriptBase.h (FABLE_RETAIL_NSCRIPT; vtable 0x012C1648, 5 slots, 0x40;
CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44). Accepted CScriptBase items: access labels, `CScriptBase();`,
`virtual ~CScriptBase();`, the four PDB slots by name (RegisterMain / Main / Init / OnPersist, declared, not defined),
filler virtuals (`v00` .. ) the row never calls, pad arrays that bring the class to 0x40, and non-virtual methods the
header declares (same name: the compile and mcheck decide the signature). The invented inline helper
`SpawnFunction(CSpawnedFuncBase* f, const char* r = "") { AddSpawnedFunction(f, CCharString(r)); }` goes: its calls
become `AddSpawnedFunction(f, CCharString(r))` (the expansion it always had). Accepted CLevelScriptBase<T> items:
`CGameScriptInterfaceBase* Game;`, `T* MasterScriptData;`, pads to 0x48, the PDB ctor / dtor declarations.
Rows defining CBaseClass / CBaseClassNonCopyable locally are skipped. usage: mig_sb.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
R = os.environ["FABLE_ROOT"]
H = open(R + "/rebuild/include/engine/CScriptBase.h").read()
HMETH = set(re.findall(r"^\s+(?:virtual\s+)?[\w:<>*& ]+?[ *&](\w+)\(", H[H.index("class CScriptBase"):], re.M))
SLOTS = {"RegisterMain", "Main", "Init", "OnPersist"}
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
def ev(e):
    if not re.fullmatch(r"[0-9a-fA-FxX+\-*() ]+", e): raise ValueError(e)
    return int(eval(re.sub(r"0[xX]([0-9a-fA-F]+)", lambda m: str(int(m.group(1), 16)), e)))
SPAWN = re.compile(r"(?:__forceinline\s+|inline\s+)?void SpawnFunction\(CSpawnedFuncBase\s*\*\s*(\w+), const char\s*\*\s*(\w+)(?:\s*=\s*\"\")?\)\s*\{\s*AddSpawnedFunction\(\1, CCharString\(\2\)\);\s*\}")
def called(t, name): return re.search(r"(?:\.|->)\s*" + name + r"\s*\(", t) is not None
def fix(t):
    if re.search(r'#include "engine/CScriptBase.h"', t): return None, "already"
    b = find_block(t, "CScriptBase")
    if not b: return None, "noblock"
    if find_block(t, "CScriptBase", b[1]): return None, "two blocks"
    for c in ("CBaseClass", "CBaseClassNonCopyable"):
        if find_block(t, c): return None, "local " + c
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CScriptBase ?(?::\s*public\s+CBaseClassNonCopyable\s*)?\{(.*)\};$", n)
    if not m: return None, "head"
    its = items(m.group(1))
    if its is None: return None, "parse"
    outside = t[:b[0]] + t[b[1]:]
    size = 0; virt = False; spawn = None; pads = 0
    for it in its:
        if re.fullmatch(r"(public|protected|private)\s*:", it): continue
        if re.fullmatch(r"CScriptBase\(\);", it): continue
        if re.fullmatch(r"virtual ~CScriptBase\(\);", it): virt = True; continue
        mm = re.fullmatch(r"virtual void (\w+)\(([^()]*)\);", it)
        if mm and mm.group(1) in SLOTS:
            if mm.group(1) == "OnPersist" and not re.fullmatch(r"CPersistContext\s*&\s*\w*", mm.group(2)): return None, "OnPersist sig"
            if mm.group(1) != "OnPersist" and mm.group(2).strip() not in ("", "void"): return None, "slot sig " + mm.group(1)
            virt = True; continue
        mm = re.fullmatch(r"virtual void ([vs]\d+)\(\);", it)
        if mm:
            if called(outside, mm.group(1)): return None, "filler called " + mm.group(1)
            virt = True; continue
        mm = re.fullmatch(r"unsigned char (\w+)\[([^\]]+)\];", it)
        if mm and re.fullmatch(r"_?(body|base|pad\w*|members|data)", mm.group(1)):
            try: size += ev(mm.group(2)); pads += 1
            except Exception: return None, "pad expr"
            continue
        sm = SPAWN.fullmatch(it)
        if sm: spawn = sm; continue
        mm = re.fullmatch(r"[\w:<>*& ]+?[ *&]\s*(\w+)\s*\([^{};]*\)\s*(const)?\s*;", it)
        if mm and mm.group(1) in HMETH and mm.group(1) not in SLOTS: continue
        return None, "item: " + it[:70]
    if pads != 1: return None, "pads %d" % pads
    total = size + (4 if virt else 0)
    if total != 0x40: return None, "size %#x" % total
    out = t
    # local CLevelScriptBase
    lb = find_block(out, "CLevelScriptBase")
    hdr = "engine/CScriptBase.h"
    if lb:
        if lb[0] < b[0]: return None, "CLevelScriptBase first"
        ln = norm(out[lb[0]:lb[1]])
        lm = re.match(r"^template ?< ?(?:class|typename) (\w+) ?> ?(?:class|struct) CLevelScriptBase ?: ?public CScriptBase ?\{(.*)\};$", ln)
        if not lm: return None, "lsb head"
        T = lm.group(1); lits = items(lm.group(2))
        if lits is None: return None, "lsb parse"
        got = []
        for it in lits:
            if re.fullmatch(r"(public|protected|private)\s*:", it): continue
            if re.fullmatch(r"CGameScriptInterfaceBase ?\* ?Game;", it): got.append("Game"); continue
            if re.fullmatch(T + r" ?\* ?MasterScriptData;", it): got.append("MSD"); continue
            if re.fullmatch(r"CLevelScriptBase\(CScriptDataBase ?\* ?\w*, CGameScriptInterfaceBase ?& ?\w*\);", it): continue
            if re.fullmatch(r"(?:virtual )?~CLevelScriptBase\(\);", it): continue
            if re.fullmatch(T + r" ?\* ?GetMasterData\(\);", it): continue
            return None, "lsb item: " + it[:60]
        if got != ["Game", "MSD"]: return None, "lsb members " + ",".join(got)
        hdr = "engine/CLevelScriptBase.h"
        out = out[:lb[0]] + out[lb[1]:]
    elif re.search(r"\bCLevelScriptBase\b", re.sub(r"//[^\n]*", "", out)): return None, "CLevelScriptBase used, no block"
    inc = ("// CScriptBase%s: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :\n"
           "//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40%s).\n"
           % (" / CLevelScriptBase<T>" if lb else "", "; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44" if lb else ""))
    if "#define FABLE_RETAIL_NSCRIPT" not in out[:b[0]]: inc += "#define FABLE_RETAIL_NSCRIPT\n"
    inc += '#include "%s"\n' % hdr
    out = out[:b[0]] + inc + out[b[1]:]
    note = ""
    if spawn:
        f, r = spawn.group(1), spawn.group(2)
        k = 0
        while True:
            mo = re.search(r"(?<![\w.>])SpawnFunction\s*\(", out)
            if not mo: break
            i = mo.end(); d = 1; q = None; args = []; cur = ""
            while i < len(out) and d:
                c = out[i]
                if q:
                    cur += c
                    if c == "\\": cur += out[i+1]; i += 1
                    elif c == q: q = None
                elif c in "\"'": q = c; cur += c
                elif c in "(<[": d += c == "("; cur += c
                elif c in ")>]":
                    if c == ")": d -= 1
                    if d: cur += c
                elif c == "," and d == 1: args.append(cur.strip()); cur = ""
                else: cur += c
                i += 1
            args.append(cur.strip())
            if len(args) not in (1, 2): return None, "SpawnFunction args"
            region = args[1] if len(args) == 2 else '""'
            out = out[:mo.start()] + "AddSpawnedFunction(%s, CCharString(%s))" % (args[0], region) + out[i:]
            k += 1
        if re.search(r"\bSpawnFunction\s*\(", re.sub(r"//[^\n]*", "", out)): return None, "SpawnFunction left"
        note = " spawn%d" % k
    return out, ("ok lsb" if lb else "ok sb") + note
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
