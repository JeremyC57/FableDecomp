#ifndef FABLE_ENGINE_CENTITYSCRIPTBINDING_H
#define FABLE_ENGINE_CENTITYSCRIPTBINDING_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): template <class T> class CEntityScriptBinding :
// public CEntityScriptBindingBase, 0x1C bytes (Ego_r.pdb CEntityScriptBinding<NScript::*> layouts: EntityScriptName +4,
// PParentScript +8, Func +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18; the binding factories new 0x1c).
// FableWin.pdb: 121 instantiations, each with exactly one ctor (const CCharString&, T*, CCountedPointer<CEntityScriptBase>
// (__fastcall*)(T&, CScriptDataBase*, const CScriptThing&), unsigned long), GetEntityScriptName (slot 0, retail body
// `lea eax,[ecx+4]; ret`, e.g. 0x00E25C60) and CreateActiveEntityScript (slot 1, e.g. 0x00E25D90); no destructor symbol.
// The ctor body is the inlined store sequence of the landed binding rows (Func is not written). Contract: the includer
// defines CCharString (a by-value member) before including this header.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CEntityScriptBinding.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CEntityScriptBindingBase.h"
class CEntityScriptBase;

template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (FABLE_FASTCALL *TAllocFunc)(T&, CScriptDataBase*, const CScriptThing&);
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }                // slot 0
protected:
    virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&, FableNScriptUInt64); // slot 1
public:
    CCharString   EntityScriptName;                                      // +0x04
    T*            PParentScript;                                         // +0x08
    void (T::*Func)();                                                   // +0x0c
    TAllocFunc    ClassAllocFunc;                                        // +0x10
    bool          ClassAllocValid;                                       // +0x14
    unsigned long Flags;                                                 // +0x18
};

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CENTITYSCRIPTBINDING_H
