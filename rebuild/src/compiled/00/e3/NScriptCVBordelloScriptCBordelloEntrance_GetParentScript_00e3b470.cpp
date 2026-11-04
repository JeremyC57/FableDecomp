// NScript::CV_BordelloScript::CBordelloEntrance::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CBordelloEntrance@CV_BordelloScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3B470, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e39de9 "BordelloEntrance" in 00e39b40; Alloc 00e3b3d0; vtable 012e3b80 slot 3; PDB ?GetParentScript@CBordelloEntrance@CV_BordelloScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e3b470.
//   ?GetParentScript@CBordelloEntrance@CV_BordelloScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_BordelloScript.h (shared header: nested class CBordelloEntrance, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/e3/NScriptCVBordelloScript_Main_00e39b40.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_BordelloScript.h"
CScriptBase* NScript::CV_BordelloScript::CBordelloEntrance::GetParentScript() const
{
    return &ParentClass;
}
