// NScript::CQ_EndGameScript::CM_UndeadSpawn::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CM_UndeadSpawn@CQ_EndGameScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D29140, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d2757b "M_UndeadSpawn" in 00d27460; Alloc 00d290a0; vtable 012cbc58 slot 3; PDB ?GetParentScript@CM_UndeadSpawn@CQ_EndGameScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d29140.
//   ?GetParentScript@CM_UndeadSpawn@CQ_EndGameScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_EndGameScript.h (shared header: nested class CM_UndeadSpawn, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/d2/NScriptCQEndGameScript_Main_00d27460.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_EndGameScript.h"
CScriptBase* NScript::CQ_EndGameScript::CM_UndeadSpawn::GetParentScript() const
{
    return &ParentClass;
}
