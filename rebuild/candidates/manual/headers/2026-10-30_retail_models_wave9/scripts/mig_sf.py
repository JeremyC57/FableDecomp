#!/usr/bin/env python3
"""Wave 5 family SF: the row-local `template <class T> CSpawnedFunc` / `template <class T, class P1> CSpawnedFunc1` (exact
landed shapes: class or struct, plain or __forceinline ctor, inline SuspendableProcess) become engine/CSpawnedFunc.h /
engine/CSpawnedFunc1.h (FABLE_RETAIL_SUSPENDABLE_PROCESS, over engine/CSpawnedFuncBase.h). The row must already include
the retail CSpawnedFuncBase.h before the block. Idempotent. usage: mig_sf.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
def canon(n):
    n = n.replace("__forceinline ", "").replace("struct ", "class ").replace("public: ", "")
    n = re.sub(r":\s*public\s+CSpawnedFuncBase", ": CSpawnedFuncBase", n)
    n = re.sub(r"\{ \}", "{}", n); n = re.sub(r"const P1 par1", "P1 par1", n)
    n = re.sub(r"\bTClass\b", "T", n); n = re.sub(r"\bTPar1\b", "P1", n)
    return n
WANT = {
 "CSpawnedFunc": "template <class T> class CSpawnedFunc : CSpawnedFuncBase { void (T::*Func)(); T* PClass; CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)()) : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {} virtual void SuspendableProcess() { (PClass->*Func)(); } };",
 "CSpawnedFunc1": "template <class T, class P1> class CSpawnedFunc1 : CSpawnedFuncBase { void (T::*Func)(P1); T* PClass; P1 Par1; CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1) : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {} virtual void SuspendableProcess() { (PClass->*Func)(Par1); } };",
}
def fix(t):
    done = []
    for c in ("CSpawnedFunc", "CSpawnedFunc1"):
        b = find_block(t, c)
        if not b: continue
        if find_block(t, c, b[1]): return None, "two blocks " + c
        if canon(norm(t[b[0]:b[1]])) != WANT[c]: return None, "shape " + c
        pre = t[:b[0]]
        if "FABLE_RETAIL_SUSPENDABLE_PROCESS" not in pre or '#include "engine/CSpawnedFuncBase.h"' not in pre: return None, "no retail CSpawnedFuncBase before " + c
        if re.search(r"(?m)^\s*namespace\b[^\n]*\{\s*$", pre) and pre.count("{") != pre.count("}"): return None, "block inside a namespace"
        t = t[:b[0]] + f'#include "engine/{c}.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess\n' + t[b[1]:]
        done.append(c)
    if not done: return None, "noblock"
    return t, "ok " + "+".join(done)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
