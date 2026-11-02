// CSpawnedFunc2<NScript::CQ_BanditCampBossBattleScript, float, CCharString>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc2@VCQ_BanditCampBossBattleScript@NScript@@MVCCharString@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012C9918 (no RTTI), the
//   CSpawnedFunc2<NScript::CQ_BanditCampBossBattleScript, float, CCharString> vtable: the COMDAT sits in the CQ_BanditCampBossBattleScript
//   TU (beside CEntityScriptBinding<CQ_BanditCampBossBattleScript>::CreateActiveEntityScript 0x00D0EC50 /
//   CActiveEntityScript<..>::SuspendableProcess 0x00D0ED60); the member at +0x40 is a CCharString, +0x3C a scalar: FableWin.pdb's only
//   CSpawnedFunc2 of that script is <CQ_BanditCampBossBattleScript,float,CCharString>.
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc2<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0EDB0, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x40]; call ~CCharString 0x0099EAE0; mov ecx,esi; call
//   ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je; push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Shape: the compiler's deleting destructor is emitted with the vtable, in a TU that constructs the class: MakeOne is that construction (the
//   0x00ED7DB0 precedent). The implicit ~CSpawnedFunc2 is expanded into it, the implicit ~CSpawnedFuncBase (engine/CSpawnedFuncBase.h)
//   called out of line.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFuncBase.h"
template <class T, class P1, class P2> class CSpawnedFunc2 : public CSpawnedFuncBase {   // FableWin.pdb CSpawnedFunc2<T,P1,P2>; no shared header
public:
    CSpawnedFunc2(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1, P2), P1 par1, P2 par2)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1), Par2(par2) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1, Par2); }  // slot 4
    void (T::*Func)(P1, P2);                                             // +0x34
    T* PClass;                                                           // +0x38
    P1 Par1;                                                             // +0x3C
    P2 Par2;                                                             // +0x40
};
namespace NScript {
class CQ_BanditCampBossBattleScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CQ_BanditCampBossBattleScript* pclass, void (NScript::CQ_BanditCampBossBattleScript::*func)(float, CCharString), float par1, const CCharString& par2)
{
    return new CSpawnedFunc2<NScript::CQ_BanditCampBossBattleScript, float, CCharString>(name, id, pclass, func, par1, par2);
}
