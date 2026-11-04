// NScript::CQ_BreakSiegeScript::CBanditAttacker::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CBanditAttacker@CQ_BreakSiegeScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1F5B0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d1d2fe "BanditAttacker" in 00d1d100; Alloc 00d1f510; vtable 012cb024 slot 3; PDB ?GetParentScript@CBanditAttacker@CQ_BreakSiegeScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d1f5b0.
//   ?GetParentScript@CBanditAttacker@CQ_BreakSiegeScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_BreakSiegeScript.h (shared header: nested class CBanditAttacker, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CQ_BreakSiegeScript.h"
CScriptBase* NScript::CQ_BreakSiegeScript::CBanditAttacker::GetParentScript() const
{
    return &ParentClass;
}
