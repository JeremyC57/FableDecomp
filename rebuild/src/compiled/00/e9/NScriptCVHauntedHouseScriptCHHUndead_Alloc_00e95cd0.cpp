// NScript::CV_HauntedHouseScript::CHHUndead::Alloc(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "HHUndead" binding: new CHHUndead(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CHHUndead@CV_HauntedHouseScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95CD0, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CHHUndead@CV_HauntedHouseScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CV_HauntedHouseScript::Main 0x00E93EB0 (slot 2 of the script vtable 0x012ECB94, which the "V_HauntedHouse" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CV_HauntedHouseScript> record named "HHUndead" (site 0x00E94AD5).
//   This Main builds its bindings through an out-of-line CEntityScriptBinding constructor (the factory is pushed).
//   Was landed as _global::Create_CVtable_012ecd3c (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x20 = sizeof(CHHUndead), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CHHUndead 0x012ECD3C.
// Layout: Ego_r.pdb CV_HauntedHouseScript::CHHUndead (32 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, RaiseUndeadBarAmount +0x1C.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
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
// The Alloc expands the entity constructor (CEntityScriptBase's members, then the vtable and ParentClass / PMasterData): its body
//   is visible here, inline. The destructor is the implicit one (Ego_r.pdb: compiler-generated).
inline NScript::CV_HauntedHouseScript::CHHUndead::CHHUndead(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL NScript::CV_HauntedHouseScript::CHHUndead::Alloc(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CHHUndead(parent, data, me));
}
