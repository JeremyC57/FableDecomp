// NScript::CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CTH_Trigger_NorthernWastes@CV_TravellingHeroesScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE1B90, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00edf229 "TH_Trigger_NorthernWastes" in 00eded10; Alloc 00ee1af0; vtable 012f4438 slot 3; PDB ?GetParentScript@CTH_Trigger_NorthernWastes@CV_TravellingHeroesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ee1b90.
//   ?GetParentScript@CTH_Trigger_NorthernWastes@CV_TravellingHeroesScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_TravellingHeroesScript.h (shared header: nested class CTH_Trigger_NorthernWastes, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_TravellingHeroesScript.h"
CScriptBase* NScript::CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::GetParentScript() const
{
    return &ParentClass;
}
