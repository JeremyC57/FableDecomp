#ifndef FABLE_ENGINE_CSPAWNEDFUNC1_H
#define FABLE_ENGINE_CSPAWNEDFUNC1_H
// Retail model (hand-written 2026-10-30, class-model migration wave 5; opt-in FABLE_RETAIL_SUSPENDABLE_PROCESS):
// template <class T, class P1> CSpawnedFunc1 : CSpawnedFuncBase, the thread object for a `void (T::*)(P1)` member with one
// stored argument. No RTTI; one vtable per instantiation, the same 7-slot shape as CSpawnedFunc<T> (slot 4
// SuspendableProcess `(PClass->*Func)(Par1)`). Layout (Ego_r.pdb CSpawnedFunc1<NScript::CQ_BountyHuntScript,CScriptThing>,
// 0x48): Func +0x34, PClass +0x38, Par1 +0x3C (by value). Spellings: Ego_r.pdb / FableWin.pdb ??0?$CSpawnedFunc1@V<T>@V<P1>@@
// QAE@ABVCCharString@@KPAV<T>@P8<T>@AEXV<P1>@Z<P1>@Z (ctor: name, id, class, func, by-value argument); the template's own
// class key is V. The ctor is __forceinline, as CSpawnedFunc<T>'s (engine/CSpawnedFunc.h: why, and what is unproven).
// The dtor is the implicit one.
#ifndef FABLE_RETAIL_SUSPENDABLE_PROCESS
#error engine/CSpawnedFunc1.h is the retail model: define FABLE_RETAIL_SUSPENDABLE_PROCESS before including it
#else
#include "engine/CSpawnedFuncBase.h"
#if defined(_MSC_VER)
#define FABLE_SPAWNEDFUNC_INLINE __forceinline
#else
#define FABLE_SPAWNEDFUNC_INLINE inline
#endif
template <class T, class P1> class CSpawnedFunc1 : public CSpawnedFuncBase {
public:
    FABLE_SPAWNEDFUNC_INLINE CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }        // slot 4
    void (T::*Func)(P1);                                                 // +0x34
    T* PClass;                                                           // +0x38
    P1 Par1;                                                             // +0x3C
};
#endif // FABLE_RETAIL_SUSPENDABLE_PROCESS
#endif // FABLE_ENGINE_CSPAWNEDFUNC1_H
