// NScript::CV_HauntedHouseScript::CHHUndead::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CHHUndead (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCHHUndead@CV_HauntedHouseScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95E60, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Naming wave 33 (family esb): slot 0 of vtable 0x012ECD3C (no RTTI: NScript TU), the vtable the landed Alloc 0x00E95CD0 stores
//   (?Alloc@CHHUndead@CV_HauntedHouseScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z, Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_GCHHUndead@CV_HauntedHouseScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CHHUndead@CV_HauntedHouseScript@NScript@@UAE@XZ.
//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_012ecd3c).
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc
//   of the landed row 0x00E95CD0 (rebuild/src/compiled/00/e9/NScriptCVHauntedHouseScriptCHHUndead_Alloc_00e95cd0.cpp), its user-declared destructor dropped. The Alloc body is not part of this graded body.
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
// The deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity (as at HEAD: the constructor and
//   the PDB Alloc are defined here); the destructor is the implicit one (??_G calls ~CEntityScriptBase 0x00F35B40).
inline NScript::CV_HauntedHouseScript::CHHUndead::CHHUndead(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
CCountedPointer<CEntityScriptBase> FABLE_FASTCALL NScript::CV_HauntedHouseScript::CHHUndead::Alloc(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CHHUndead(parent, data, me));
}
