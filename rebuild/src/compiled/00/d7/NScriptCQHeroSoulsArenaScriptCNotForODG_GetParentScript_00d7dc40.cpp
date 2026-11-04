// NScript::CQ_HeroSoulsArenaScript::CNotForODG::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CNotForODG@CQ_HeroSoulsArenaScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7DC40, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d7c8e3 "NotForODG" in 00d7c890; Alloc 00d7dba0; vtable 012d3c50 slot 3; PDB ?GetParentScript@CNotForODG@CQ_HeroSoulsArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d7dc40.
//   ?GetParentScript@CNotForODG@CQ_HeroSoulsArenaScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_HeroSoulsArenaScript.h (shared header: nested class CNotForODG, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_HeroSoulsArenaScript.h"
CScriptBase* NScript::CQ_HeroSoulsArenaScript::CNotForODG::GetParentScript() const
{
    return &ParentClass;
}
