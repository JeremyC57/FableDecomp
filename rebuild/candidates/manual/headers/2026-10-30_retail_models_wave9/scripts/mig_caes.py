#!/usr/bin/env python3
"""Wave 8 family CAES: CEntityScriptBinding<T>::CreateActiveEntityScript rows that define, at the top level, local
CActiveEntityScriptBase / CActiveEntityScript<T> / CEntityScriptBindingBase / CEntityScriptBinding<T> of the shared shape
(the members, offsets, ctors and slots of engine/CActiveEntityScript.h, engine/CEntityScriptBinding.h): the blocks go and
the headers are included in their place. Each local block must match the expected normalized member list.
usage: mig_caes.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
from nsutil import top_insert_pos
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def sq(s): return re.sub(r"\s+", "", s)
EXP = {
 "CActiveEntityScriptBase": "classCActiveEntityScriptBase:publicCSpawnedFuncBase{public:CCountedPointer<CEntityScriptBase>PEntityScript;unsignedlongFlags;unsigned__int64EntityUID;CScriptThingEntity;CActiveEntityScriptBase(constCScriptThing&entity,unsigned__int64uid,unsignedlongflags);};",
 "CActiveEntityScript": "template<classT>classCActiveEntityScript:publicCActiveEntityScriptBase{public:void(T::*Func)(CScriptThing);T*PParentScript;__forceinlineCActiveEntityScript(constCScriptThing&entity,unsigned__int64uid,T*parent,void(T::*func)(CScriptThing),unsignedlongflags):CActiveEntityScriptBase(entity,uid,flags),Func(func),PParentScript(parent){}__forceinlineCActiveEntityScript(constCScriptThing&entity,unsigned__int64uid,T*parent,CCountedPointer<CEntityScriptBase>&script,unsignedlongflags):CActiveEntityScriptBase(entity,uid,flags),PParentScript(parent){PEntityScript=script;}virtualvoidSuspendableProcess();};",
 "CEntityScriptBindingBase": "class__declspec(novtable)CEntityScriptBindingBase{public:virtualconstCCharString&GetEntityScriptName()const=0;protected:virtualCCountedPointer<CActiveEntityScriptBase>CreateActiveEntityScript(constCScriptThing&thing,unsigned__int64uid)=0;};",
 "CEntityScriptBinding": "template<classT>classCEntityScriptBinding:publicCEntityScriptBindingBase{public:typedefCCountedPointer<CEntityScriptBase>(__fastcall*TAllocFunc)(T&parent,CScriptDataBase*data,constCScriptThing&thing);CCharStringEntityScriptName;T*PParentScript;void(T::*Func)(CScriptThing);TAllocFuncClassAllocFunc;boolClassAllocValid;unsignedlongFlags;virtualconstCCharString&GetEntityScriptName()const{returnEntityScriptName;}protected:virtualCCountedPointer<CActiveEntityScriptBase>CreateActiveEntityScript(constCScriptThing&thing,unsigned__int64uid);};",
}
def blk(t, name):
    # find_block misses `class __declspec(novtable) X`; search the head directly
    m = re.search(r"^(?:template\s*<[^>]*>\s*)?class\s+(?:__declspec\(novtable\)\s+)?" + name + r"\s*[:{]", t, re.M)
    if not m: return None
    i = t.index("{", m.start()); d = 0; j = i
    while True:
        if t[j] == "{": d += 1
        elif t[j] == "}":
            d -= 1
            if d == 0: break
        j += 1
    k = t.index(";", j) + 1
    if k < len(t) and t[k] == "\n": k += 1
    return m.start(), k
def fix(t):
    if '#include "engine/CActiveEntityScript.h"' in t: return None, "already"
    bs = {}
    for n in EXP:
        b = blk(t, n)
        if not b: return None, "no " + n
        if sq(code_of(t[b[0]:b[1]])) != EXP[n]: return None, "shape " + n
        if top_insert_pos(t, b[0]) != b[0]: return None, "nested " + n
        bs[n] = b
    order = sorted(bs, key=lambda n: bs[n][0])
    if order != ["CActiveEntityScriptBase", "CActiveEntityScript", "CEntityScriptBindingBase", "CEntityScriptBinding"]: return None, "order"
    out = t
    inc2 = ("// CEntityScriptBindingBase / CEntityScriptBinding<T>: the retail models from the shared headers (engine/CEntityScriptBinding.h:\n"
            "//   0x1C; EntityScriptName +4, PParentScript +8, Func +0xC, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18).\n"
            '#include "engine/CEntityScriptBinding.h"\n')
    inc1 = ("// CActiveEntityScriptBase / CActiveEntityScript<T>: the retail models from the shared headers (engine/CActiveEntityScript.h:\n"
            "//   : CSpawnedFuncBase, vtable 0x012C3594, 0x58; PEntityScript +0x34, Flags +0x3C, EntityUID +0x40, Entity +0x48; Func +0x58,\n"
            "//   PParentScript +0x5C; both ctors inline, as retail expands them here).\n"
            '#include "engine/CActiveEntityScript.h"\n')
    b = bs["CEntityScriptBinding"]; out = out[:b[0]] + out[b[1]:]
    b = bs["CEntityScriptBindingBase"]; out = out[:b[0]] + inc2 + out[b[1]:]
    b = bs["CActiveEntityScript"]; out = out[:b[0]] + out[b[1]:]
    b = bs["CActiveEntityScriptBase"]; out = out[:b[0]] + inc1 + out[b[1]:]
    if "#define FABLE_RETAIL_SUSPENDABLE_PROCESS" not in out[:out.index(inc1)]: return None, "no suspendable opt-in before"
    return out, "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
