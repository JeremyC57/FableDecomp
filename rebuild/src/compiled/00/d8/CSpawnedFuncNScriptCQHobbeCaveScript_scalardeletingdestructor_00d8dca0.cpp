// CSpawnedFunc<NScript::CQ_HobbeCaveScript>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc (no members with destructors), i.e.
//   ~CSpawnedFuncBase, then free the thread object when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc@VCQ_HobbeCaveScript@NScript@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8DCA0, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Naming wave 33 (family sf): slot 0 of vtable 0x012D567C (no RTTI), the CSpawnedFunc<T> vtable the landed spawning rows of T store
//   (0x00D8DC00 ?RegisterMain@CQ_HobbeCaveScript@NScript@@UAEXXZ); FableWin.pdb ??_E?$CSpawnedFunc@VCQ_HobbeCaveScript@NScript@@@@UAEPAXI@Z; FableWin.pdb ??1?$CSpawnedFunc@VCQ_HobbeCaveScript@NScript@@@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012d567c::`scalar deleting destructor').
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
class CQ_HobbeCaveScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQ_HobbeCaveScript* pclass, void (NScript::CQ_HobbeCaveScript::*func)())
{
    return new CSpawnedFunc<NScript::CQ_HobbeCaveScript>(name, id, pclass, func);
}
