#ifndef FABLE_ENGINE_CENTITYSCRIPTBINDINGBASE_H
#define FABLE_ENGINE_CENTITYSCRIPTBINDINGBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). An abstract interface with no RTTI, no virtual
// destructor and no own vtable in retail: every CEntityScriptBinding<T> vtable (e.g. 0x012E1E10) has exactly 2 slots and the
// binding ctors store only the derived vtable, so the base is __declspec(novtable) (modelled as in the landed binding rows).
// Slots: 0 GetEntityScriptName (FableWin/Ego_r ?GetEntityScriptName@?$CEntityScriptBinding@..@@UBEABVCCharString@@XZ, public),
// 1 CreateActiveEntityScript (?CreateActiveEntityScript@?$CEntityScriptBinding@..@@MAE?AV?$CCountedPointer@
// VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z, protected in the derived template; the base's own access and its
// pure-ness are not in either PDB (no base symbols but the ctor) -- declared pure and protected here). Ctor
// ??0CEntityScriptBindingBase@@QAE@XZ is trivial (inlined into every binding ctor): left implicit. No data members (4 bytes).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CEntityScriptBindingBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/fable_nscript_forward.h"
#if defined(_MSC_VER)
#  define FABLE_NSCRIPT_NOVTABLE __declspec(novtable)
#else
#  define FABLE_NSCRIPT_NOVTABLE
#endif

class FABLE_NSCRIPT_NOVTABLE CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;                                         // slot 0
protected:
    virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&, FableNScriptUInt64) = 0; // slot 1
};
FABLE_STATIC_ASSERT(sizeof(CEntityScriptBindingBase) == 0x4);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CENTITYSCRIPTBINDINGBASE_H
