// CEntityScriptBinding<NScript::CV_HauntedHouseScript>::CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid) -- start an entity script for a newly bound entity: with a class allocator, allocate the entity script object (parent, parent's master data, thing), Init() it and wrap it in a new CActiveEntityScript; otherwise wrap the parent's member function
// Leaf: ?CreateActiveEntityScript@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY -- implicit ~CActiveEntityScriptBase (~CScriptThing +0x48, ~CCountedPointer +0x34, call ~CSpawnedFuncBase) expanded in this TU's ??_G as retail does; sweep on 0x00D032C0: /d2inlS17-19 fail, 20-40 match (with or without /Og; without DECL_ONLY 30v48), 85/120 CActiveEntityScript<T> pairs match at /d2inlS20
// NF424 r4 (2026-11-25, R-NF424-5: 2026-11-08 owner rule, inline-budget residue): DECL_ONLY is the existing engine/CSpawnedFuncBase.h
//   opt-in (~CSpawnedFuncBase 0x00CDD4C0 declared only); no header changes. The flag emulates an unidentified build setting of the retail script TUs.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95F80, 272 bytes, `ret 10h` (hidden result + thing + 64-bit uid). Slot 1 of the
//   CEntityScriptBinding<CV_HauntedHouseScript> vtable @ 0x012ECC98 (slot 0 GetEntityScriptName).
// Identity: that binding vtable is stored by 0x00E95390 (the script's Main building `new CEntityScriptBinding<T>(name, this,
//   alloc, flags)`), which sits in the vtable of the script whose Alloc CScriptInfoManager::RegisterScripts (0x00CD52D0)
//   registers as "V_HauntedHouse" -> NScript::CV_HauntedHouseScript; the PDB name sets hold ?CreateActiveEntityScript@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z.
//   Family F002 (85 byte-identical bodies): the manifest label (CScriptGameResourceObjectMonsterBase_CreateFromEntity) is propagated.
// Callees: CCountedPointer<CEntityScriptBase>::~CCountedPointer @ 0x00CE1000 (release + zero both words), scalar operator new
//   @ 0x00BFEA1A (0x60 bytes), CActiveEntityScriptBase(const CScriptThing&, unsigned __int64, unsigned long) @ 0x00CE1110,
//   CCountedPointer<CEntityScriptBase>::ShareData @ 0x00CE1040, CCountedPointer<CActiveEntityScriptBase>(CActiveEntityScriptBase*)
//   @ 0x00CE10B0; the class allocator (+0x10, `__fastcall`, hidden result in ecx, parent in edx) and the entity script's Init
//   (CEntityScriptBase slot 2, +0x08). Data: CActiveEntityScript<CV_HauntedHouseScript> vtable @ 0x012ECD58.
// Layout (retail = PDB with 4-byte STLport lists / 0x40 CScriptBase): CEntityScriptBinding<T> {vptr, EntityScriptName +4,
//   PParentScript +8, Func +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18}; CActiveEntityScriptBase 0x58
//   (CSpawnedFuncBase 0x34, PEntityScript +0x34, Flags +0x3c, EntityUID +0x40, Entity +0x48); CActiveEntityScript<T> adds
//   Func +0x58 and PParentScript +0x5c (0x60). The parent's +0x44 is CLevelScriptBase<TData>::MasterScriptData (viewed here
//   through its CScriptDataBase base; the data class does not enter any symbol of this function).
// Shape: the counted-pointer destructor must have its body in the TU (`noinline`): with only a declaration VC7.1 swaps the two
//   frame slots of the allocator's result temporary and the named copy and does not keep the copy's Data/Info in ebp/ebx for
//   the later ShareData pushes. Both CActiveEntityScript constructors are `__forceinline` (a plain inline one stays a call).
// CCountedPointer: the shared engine/CCountedPointer.h with the per-TU forms the row-local template spelled (the (T*) constructor
//   0x00CE10B0 called, the destructor 0x00CE1000 visible but called, ShareData 0x00CE1040 declared), as the landed shared-header
//   CreateActiveEntityScript rows do (e.g. 0x00D7C2E0).
#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CActiveEntityScript.h"
#include "engine/CEntityScriptBinding.h"
template <class T>
CCountedPointer<CActiveEntityScriptBase> CEntityScriptBinding<T>::CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid)
{
    if (ClassAllocValid) {
        CCountedPointer<CEntityScriptBase> script(ClassAllocFunc(*PParentScript, &PParentScript->MasterScriptData, thing));
        script->Init();
        return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, script, Flags));
    }
    return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, Func, Flags));
}
template class CEntityScriptBinding<NScript::CV_HauntedHouseScript>;
