// CSpawnedFunc2<NScript::CHeroBoastsScript, long, CCharString>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc2@VCHeroBoastsScript@NScript@@JVCCharString@@@@UAEPAXI@Z
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x012C36B8 (no RTTI), the CSpawnedFunc2<NScript::CHeroBoastsScript,
//   long, CCharString> vtable: the vtable 0x012C36B8 is bound ??_7?$CSpawnedFunc2@VCHeroBoastsScript@NScript@@JVCCharString@@@@6B@ (its
//   storer NScript::CHeroBoastsScript::Main 0x00CE1AF0); slot 4 is
//   CSpawnedFunc2<NScript::CHeroBoastsScript,long,CCharString>::SuspendableProcess 0x00CE1C70 (catalog).
//   FableWin.pdb names the instantiation (??_G / ??_E CSpawnedFunc2<..> with these arguments). Was a free __fastcall
//   CEngineSubPrimitiveText_CompositeScalarDeletingDestructor stand-in (Leaf `scalar_deleting_destructor').
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE6BE0, 40 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x40]; call ~CCharString 0x0099EAE0; mov ecx,esi; call
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
class CHeroBoastsScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CHeroBoastsScript* pclass, void (NScript::CHeroBoastsScript::*func)(long, CCharString), long par1, const CCharString& par2)
{
    return new CSpawnedFunc2<NScript::CHeroBoastsScript, long, CCharString>(name, id, pclass, func, par1, par2);
}
