// CSpawnedFunc1<NScript::CQ_BountyHuntScript, CScriptThing>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCQ_BountyHuntScript@NScript@@VCScriptThing@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012CA5E0 (no RTTI), the CSpawnedFunc1<NScript::CQ_BountyHuntScript,
//   CScriptThing> vtable: the storers are the CQ_BountyHuntScript entity Inits (landed
//   ?Init@CBH_GangDeputy1@CQ_BountyHuntScript@NScript@@EAEXXZ 0x00D16460 and four more) storing Func =
//   CQ_BountyHuntScript::WatchForDeputy1Killed 0x00D16570 / WatchForHostage1Killed / WatchForLeaderKilled ...
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc1<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D172E0, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x3C]; call ~CScriptThing 0x004AA840; mov ecx,esi; call
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
class CQ_BountyHuntScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQ_BountyHuntScript* pclass, void (NScript::CQ_BountyHuntScript::*func)(CScriptThing), const CScriptThing& par1)
{
    return new CSpawnedFunc1<NScript::CQ_BountyHuntScript, CScriptThing>(name, id, pclass, func, par1);
}
