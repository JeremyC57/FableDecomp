#ifndef FABLE_ENGINE_CACTIVEENTITYSCRIPT_H
#define FABLE_ENGINE_CACTIVEENTITYSCRIPT_H
// Retail model (hand-written 2026-10-30, wave 8 class-model migration): template <class T> class CActiveEntityScript :
// public CActiveEntityScriptBase, 0x60 bytes (Ego_r.pdb CActiveEntityScript<NScript::*> layouts: Func +0x58 (a pointer to
// member), PParentScript +0x5C). No RTTI; one vtable per T, stored by the inlined ctor inside
// CEntityScriptBinding<T>::CreateActiveEntityScript (e.g. 0x012D3FE0 by 0x00D7FAB0 for NScript::CQ_HeroSoulsArenaScript):
// slot 0 the scalar deleting dtor (0x00D7FC10, implicit: no own vtable store), 1-3 the microthread's, 4 SuspendableProcess
// (per T, e.g. 0x00D7FBC0), 5-6 CActiveEntityScriptBase's 0x00CE1090 / 0x00CE10A0. All 120 retail SuspendableProcess bodies
// sit in such a vtable (wave 8 scripts/aes_T.py) and FableWin.pdb names each ?SuspendableProcess@?$CActiveEntityScript@V<T>@
// NScript@@@@UAEXXZ (Ego_r.pdb lists every T too).
// Func: FableWin.pdb ctor ??0?$CActiveEntityScript@VCQ_ArenaScript@NScript@@@@QAE@ABVCScriptThing@@_KPAVCQ_ArenaScript@NScript@@
// P823@AEXV1@@ZK@Z -- `void (T::*)(CScriptThing)`, called `mov ecx,[esi+5Ch]; call [esi+58h]` with the Entity copy by value
// (a 4-byte single-inheritance member pointer: the includer declares T complete or `class __single_inheritance T;`).
// The two ctors (Func, or an already allocated entity script) are in FableWin.pdb (Ego_r.pdb keeps the second out of line,
// e.g. 0x00CEFBE0). Retail expands both inside every CEntityScriptBinding<T>::CreateActiveEntityScript (0x00D7FAB0:
// `call 0x00CE1110` (the base ctor), the CActiveEntityScript<T> vtable store, then Func +0x58 / PParentScript +0x5C, or
// PParentScript and `PEntityScript = script` through CCountedPointer::ShareData 0x00CE1040): they are inline here,
// __forceinline under VC7.1 as the landed binding rows model them (85 include this header since wave 8; the CSpawnedFunc<T>
// ctor rule, engine/CSpawnedFunc.h).
// The dtor is implicit (0x00D7FC10 has no own vtable store).
// Contract: as engine/CActiveEntityScriptBase.h; the second ctor needs CCountedPointer<T>::operator= (the includer's model).
#include "engine/CActiveEntityScriptBase.h"
#if defined(FABLE_RETAIL_NSCRIPT) && defined(FABLE_RETAIL_SUSPENDABLE_PROCESS)

#if defined(_MSC_VER)
#  define FABLE_ACTIVEENTITYSCRIPT_INLINE __forceinline
#else
#  define FABLE_ACTIVEENTITYSCRIPT_INLINE inline
#endif
template <class T> class CActiveEntityScript : public CActiveEntityScriptBase {
public:
    typedef void (T::*TFunc)(CScriptThing);
    FABLE_ACTIVEENTITYSCRIPT_INLINE CActiveEntityScript(const CScriptThing& entity, FableNScriptUInt64 uid, T* parent, TFunc func, unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), Func(func), PParentScript(parent) { }
    FABLE_ACTIVEENTITYSCRIPT_INLINE CActiveEntityScript(const CScriptThing& entity, FableNScriptUInt64 uid, T* parent, CCountedPointer<CEntityScriptBase>& script, unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), PParentScript(parent) { PEntityScript = script; }
    virtual void SuspendableProcess();                                   // slot 4  (per T)
    TFunc Func;                                                          // +0x58
    T*    PParentScript;                                                 // +0x5c
};

#endif
#endif // FABLE_ENGINE_CACTIVEENTITYSCRIPT_H
