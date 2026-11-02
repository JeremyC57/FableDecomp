// CSpawnedFunc1<NScript::CQ_AmbushTradersScript, CScriptThing>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCQ_AmbushTradersScript@NScript@@VCScriptThing@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012F7EBC (no RTTI), the
//   CSpawnedFunc1<NScript::CQ_AmbushTradersScript, CScriptThing> vtable: the storers (landed CQ_AmbushTradersScript entity Inits 0x00F05F30
//   .. and BanditHireSetup 0x00F081E0) spawn Func = CQ_AmbushTradersScript::WatchForHeroAttackOnBandit 0x00F061E0 / WatchForEnemyDeath ...
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc1<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F09C70, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x3C]; call ~CScriptThing 0x004AA840; mov ecx,esi; call
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
class CQ_AmbushTradersScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQ_AmbushTradersScript* pclass, void (NScript::CQ_AmbushTradersScript::*func)(CScriptThing), const CScriptThing& par1)
{
    return new CSpawnedFunc1<NScript::CQ_AmbushTradersScript, CScriptThing>(name, id, pclass, func, par1);
}
