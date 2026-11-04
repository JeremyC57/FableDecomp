// NScript::CPersonalScript_EmptyScript::CArcherMan::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CArcherMan@CPersonalScript_EmptyScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF24C0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ef08a0 "ArcherMan" in 00ef0090; Alloc 00ef2420; vtable 012f61ec slot 3; PDB ?GetParentScript@CArcherMan@CPersonalScript_EmptyScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ef24c0.
//   ?GetParentScript@CArcherMan@CPersonalScript_EmptyScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CPersonalScript_EmptyScript.h (shared header: nested class CArcherMan, ParentClass reference); includes as the landed includer rebuild/src/compiled/00/ef/NScriptCPersonalScriptEmptyScriptCArcherMan_Main_00ef71e0.cpp; the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CPersonalScript_EmptyScript.h"
CScriptBase* NScript::CPersonalScript_EmptyScript::CArcherMan::GetParentScript() const
{
    return &ParentClass;
}
