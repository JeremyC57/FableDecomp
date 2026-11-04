// NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8BDD0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e8ba09 "GhostGrannyNecklace" in 00e8b9c0; Alloc 00e8bd30; vtable 012ebb14 slot 3; PDB ?GetParentScript@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e8bdd0.
//   ?GetParentScript@CGhostGrannyNecklace@CV_GhostGrannyNecklaceScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_GhostGrannyNecklaceScript.h (shared header: nested class CGhostGrannyNecklace, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_GhostGrannyNecklaceScript.h"
CScriptBase* NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::GetParentScript() const
{
    return &ParentClass;
}
