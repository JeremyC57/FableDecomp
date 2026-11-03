// CSpawnedFunc<NScript::CV_GuildMasterScript>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc (no members with destructors), i.e.
//   ~CSpawnedFuncBase, then free the thread object when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc@VCV_GuildMasterScript@NScript@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E90B90, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Naming wave 33 (family sf): slot 0 of vtable 0x012EC14C (no RTTI), the CSpawnedFunc<T> vtable the landed spawning rows of T store
//   (0x00E90790 ?RegisterMain@CV_GuildMasterScript@NScript@@UAEXXZ); FableWin.pdb ??_E?$CSpawnedFunc@VCV_GuildMasterScript@NScript@@@@UAEPAXI@Z; FableWin.pdb ??1?$CSpawnedFunc@VCV_GuildMasterScript@NScript@@@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012ec14c::`scalar deleting destructor').
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
class CV_GuildMasterScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CV_GuildMasterScript* pclass, void (NScript::CV_GuildMasterScript::*func)())
{
    return new CSpawnedFunc<NScript::CV_GuildMasterScript>(name, id, pclass, func);
}
