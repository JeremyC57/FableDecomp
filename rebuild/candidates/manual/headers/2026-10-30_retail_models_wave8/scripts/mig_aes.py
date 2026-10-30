#!/usr/bin/env python3
"""Wave 8 family AES: the 120 CActiveEntityScript<T>::SuspendableProcess rows (one generated shape: an ad-hoc
CActiveEntityScript_<addr> struct with a local CScriptThing {m_pThing, m_pCount}) become the retail model:
engine/CActiveEntityScript.h (+ CActiveEntityScriptBase.h, CSpawnedFuncBase.h, CScriptThing.h), T recovered from retail by
aes_T.py (the vtable whose slot 4 is the body is stored only by CEntityScriptBinding<T>::CreateActiveEntityScript), and
the body `if (PEntityScript.Data) PEntityScript.Data->Main(); else (PParentScript->*Func)(Entity); StopRunning = true;`
(slot 1 of CEntityScriptBase is Main; +0x05 is CASuspendableProcess::StopRunning, Ego_r.pdb). The by-value Entity copy is
the inline CScriptThing copy the local model had (defined inline after the header, with the PImp addref).
usage: mig_aes.py [--dry] <aes_T.tsv> <file>..."""
import sys, os, re
dry = "--dry" in sys.argv; a = [x for x in sys.argv[1:] if x != "--dry"]
T = {}
for l in open(a[0]):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 4 and q[3] != "?": T[q[0]] = (q[3], q[1])
CODE = r'''#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CCountedPointer<T>: the Ego_r.pdb members (Data, Info; CCPPointerInfo RefCount +0); the copy is the inline addref retail
// expands inside the CScriptThing copy below.
class CCPPointerInfo { public: unsigned long RefCount; };
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing / CEntityScriptBase / CActiveEntityScript<T>: the retail models from the shared headers.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
// Retail expands the CScriptThing copy for the by-value argument (CBaseClass vtable 0x01231710 then 0x01238C8C, PImp addref).
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CEntityScriptBase.h"
#include "engine/CActiveEntityScript.h"
namespace NScript { class __single_inheritance %(S)s; }   // T: Func is a 4-byte member pointer (single inheritance)

template <class T> void CActiveEntityScript<T>::SuspendableProcess()
{
    if (PEntityScript.Data)
        PEntityScript.Data->Main();
    else
        (PParentScript->*Func)(Entity);
    StopRunning = true;
}
template void CActiveEntityScript<NScript::%(S)s>::SuspendableProcess();
'''
for f in a[1:]:
    ad = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
    if ad not in T: print("skip", f, "no T"); continue
    t = open(f, encoding="utf-8").read()
    if "CActiveEntityScript_" + ad not in t or "m_pThing" not in t: print("skip", f, "shape"); continue
    full, vt = T[ad]; s = full.split("::", 1)[1]
    lines = t.split("\n"); i = 0
    while i < len(lines) and lines[i].startswith("//"): i += 1
    head = lines[:i]
    head[0] = "// CActiveEntityScript<NScript::%s>::SuspendableProcess() -- runs the entity script's Main, or calls the bound member function with a CScriptThing copy, then stops the process" % s
    head.insert(1, "// Leaf: ?SuspendableProcess@?$CActiveEntityScript@V%s@NScript@@@@UAEXXZ" % s)
    head.append("// Identity (wave 8): slot 4 of the RTTI-less vtable 0x%s, which only CEntityScriptBinding<NScript::%s>::CreateActiveEntityScript" % (vt.upper(), s))
    head.append("//   stores (scripts/aes_T.py); FableWin.pdb ?SuspendableProcess@?$CActiveEntityScript@V%s@NScript@@@@UAEXXZ. Layout and" % s)
    head.append("//   members: engine/CActiveEntityScript.h (PEntityScript +0x34, Entity +0x48, Func +0x58, PParentScript +0x5c; StopRunning +0x05).")
    out = "\n".join(head) + "\n" + CODE % {"S": s}
    if not dry: open(f, "w", encoding="utf-8", newline="\n").write(out)
    print("edit", f, s)
