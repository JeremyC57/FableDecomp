// NScript::CV_HiddenBootyScript::CM_HiddenBooty::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93010, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e92a99 "M_HiddenBooty" in 00e92a50; Alloc 00e92f70; vtable 012ecae8 slot 3; PDB ?GetParentScript@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e93010.
//   ?GetParentScript@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_HiddenBootyScript.h (shared header: nested class CM_HiddenBooty, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/e9/NScriptCVHiddenBootyScript_Main_00e92a50.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_HiddenBootyScript.h"
CScriptBase* NScript::CV_HiddenBootyScript::CM_HiddenBooty::GetParentScript() const
{
    return &ParentClass;
}
