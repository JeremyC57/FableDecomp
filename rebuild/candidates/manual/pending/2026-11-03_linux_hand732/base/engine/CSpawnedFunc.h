#ifndef FABLE_ENGINE_CSPAWNEDFUNC_H
#define FABLE_ENGINE_CSPAWNEDFUNC_H
// Retail model (hand-written 2026-10-30, class-model migration wave 5; opt-in FABLE_RETAIL_SUSPENDABLE_PROCESS):
// template <class T> CSpawnedFunc : CSpawnedFuncBase, the thread object a script spawns for a `void (T::*)()` member.
// No RTTI; one vtable per instantiation (e.g. 0x012F55CC CSpawnedFunc<NScript::CExpression_FollowScript>, 0x012E12F4),
// 7 slots: 0 scalar deleting dtor (per instantiation), 1-3 CSuspendableProcessMicrothread's (0x00A44880, 0x00A44840,
// 0x00A447D0), 4 SuspendableProcess (per instantiation: `(PClass->*Func)()`), 5-6 CSpawnedFuncBase's (0x00CDD410,
// 0x00CDD420). Layout (Ego_r.pdb CSpawnedFunc<NScript::CQ_BountyHuntScript>, 0x3C): Func +0x34 (a single-inheritance
// pointer to member, 4 bytes), PClass +0x38. Spellings: Ego_r.pdb / FableWin.pdb ??0?$CSpawnedFunc@V<T>@@QAE@ABVCCharString@@
// KPAV<T>@P8<T>@AEXXZ@Z (ctor: name, id, class, func), ?SuspendableProcess@?$CSpawnedFunc@..@@UAEXXZ, ??1 (UAE), ??_E; the
// template's own class key is V. The ctor is inline: landed spawning rows expand it (the CSpawnedFuncBase ctor call
// 0x00CDD450, the vtable store, Func, PClass), and retail keeps out-of-line COMDAT copies where it was not expanded
// (0x00E95F10 CSpawnedFunc<NScript::CV_HauntedHouseScript>). It is __forceinline here: over the 241 landed spawning rows
// (wave 5) a plain inline ctor reproduces 224 and RTM 3077 declines to expand it at 16 sites retail expanded; __forceinline
// reproduces 238 (the other 2 are CSpawnedFunc1 rows with a CScriptThing argument, which keep row-local models).
// unproven: retail's own declaration (plain inline under QFE 4035's heuristic, or forced). The dtor is the implicit one.
#ifndef FABLE_RETAIL_SUSPENDABLE_PROCESS
#error engine/CSpawnedFunc.h is the retail model: define FABLE_RETAIL_SUSPENDABLE_PROCESS before including it
#else
#include "engine/CSpawnedFuncBase.h"
#if defined(_MSC_VER)
#define FABLE_SPAWNEDFUNC_INLINE __forceinline
#else
#define FABLE_SPAWNEDFUNC_INLINE inline
#endif
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {
public:
    FABLE_SPAWNEDFUNC_INLINE CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) { }
    virtual void SuspendableProcess() { (PClass->*Func)(); }            // slot 4
    void (T::*Func)();                                                   // +0x34
    T* PClass;                                                           // +0x38
};
#endif // FABLE_RETAIL_SUSPENDABLE_PROCESS
#endif // FABLE_ENGINE_CSPAWNEDFUNC_H
