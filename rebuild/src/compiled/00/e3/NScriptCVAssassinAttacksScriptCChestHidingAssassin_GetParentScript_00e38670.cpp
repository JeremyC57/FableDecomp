// NScript::CV_AssassinAttacksScript::CChestHidingAssassin::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CChestHidingAssassin@CV_AssassinAttacksScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E38670, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e378e3 "ChestHidingAssassin" in 00e377d0; Alloc 00e385d0; vtable 012e3704 slot 3; PDB ?GetParentScript@CChestHidingAssassin@CV_AssassinAttacksScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e38670.
//   ?GetParentScript@CChestHidingAssassin@CV_AssassinAttacksScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_AssassinAttacksScript.h (shared header: nested class CChestHidingAssassin, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/e3/NScriptCVAssassinAttacksScript_Main_00e377d0.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_AssassinAttacksScript.h"
CScriptBase* NScript::CV_AssassinAttacksScript::CChestHidingAssassin::GetParentScript() const
{
    return &ParentClass;
}
