// CSpawnedFunc1<NScript::CQ_HobbeToothContestScript, CScriptThing>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCQ_HobbeToothContestScript@NScript@@VCScriptThing@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012D6BF0 (no RTTI), the
//   CSpawnedFunc1<NScript::CQ_HobbeToothContestScript, CScriptThing> vtable: the storer is the landed
//   ?Init@CQ_HobbeToothWhisper@CQ_HobbeToothContestScript@NScript@@EAEXXZ 0x00D9D4F0 (its parent script); the COMDAT sits in that TU beside
//   CActiveEntityScript<CQ_HobbeToothContestScript>::SuspendableProcess 0x00D9DA40.
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc1<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9DA90, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x3C]; call ~CScriptThing 0x004AA840; mov ecx,esi; call
//   ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je; push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Shape: the compiler's deleting destructor is emitted with the vtable, in a TU that constructs the class: MakeOne is that construction (the
//   0x00ED7DB0 precedent). The implicit ~CSpawnedFunc1 is expanded into it, the implicit ~CSpawnedFuncBase (engine/CSpawnedFuncBase.h)
//   called out of line.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class CQ_HobbeToothContestScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQ_HobbeToothContestScript* pclass, void (NScript::CQ_HobbeToothContestScript::*func)(CScriptThing), const CScriptThing& par1)
{
    return new CSpawnedFunc1<NScript::CQ_HobbeToothContestScript, CScriptThing>(name, id, pclass, func, par1);
}
