// CSpawnedFunc1<NScript::CQR_EscortTraderScript, CScriptThing>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCQR_EscortTraderScript@NScript@@VCScriptThing@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012E15D0 (no RTTI), the
//   CSpawnedFunc1<NScript::CQR_EscortTraderScript, CScriptThing> vtable: the storers spawn Func =
//   CQR_EscortTraderScript::WatchForExpressionFollow 0x00E1FCF0 / WatchForExpressionWait 0x00E1FEF0 (and the landed CAttackingBandit::Init
//   0x00E1F3E0).
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc1<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1F780, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x3C]; call ~CScriptThing 0x004AA840; mov ecx,esi; call
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
class CQR_EscortTraderScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQR_EscortTraderScript* pclass, void (NScript::CQR_EscortTraderScript::*func)(CScriptThing), const CScriptThing& par1)
{
    return new CSpawnedFunc1<NScript::CQR_EscortTraderScript, CScriptThing>(name, id, pclass, func, par1);
}
