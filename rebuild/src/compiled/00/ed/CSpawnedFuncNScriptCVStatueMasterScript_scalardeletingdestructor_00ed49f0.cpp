// CSpawnedFunc<NScript::CV_StatueMasterScript>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc (no members with destructors), i.e.
//   ~CSpawnedFuncBase, then free the thread object when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc@VCV_StatueMasterScript@NScript@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED49F0, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Naming wave 33 (family sf): slot 0 of vtable 0x012F2E80 (no RTTI), the CSpawnedFunc<T> vtable the landed spawning rows of T store
//   (0x00ED3A90 ?RegisterMain@CV_StatueMasterScript@NScript@@UAEXXZ); FableWin.pdb ??_E?$CSpawnedFunc@VCV_StatueMasterScript@NScript@@@@UAEPAXI@Z; FableWin.pdb ??1?$CSpawnedFunc@VCV_StatueMasterScript@NScript@@@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012f2e80::`scalar deleting destructor').
// Shape: the compiler's deleting destructor is emitted with the vtable, in a TU that constructs the class: MakeOne is that construction (the
//   wave 29 CSpawnedFunc1 precedent 0x00CE6C10). The implicit ~CSpawnedFunc is expanded into it, the implicit ~CSpawnedFuncBase called out of line.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc.h"
namespace NScript {
class CV_StatueMasterScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CV_StatueMasterScript* pclass, void (NScript::CV_StatueMasterScript::*func)())
{
    return new CSpawnedFunc<NScript::CV_StatueMasterScript>(name, id, pclass, func);
}
