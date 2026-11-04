// NScript::CQ_ArenaScript::CCagedBalverine::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CCagedBalverine@CQ_ArenaScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F1CAD0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00f100c7 "CagedBalverine" in 00f0fb70; Alloc 00f1ca30; vtable 012f9ea4 slot 3; PDB ?GetParentScript@CCagedBalverine@CQ_ArenaScript@NScript@@; refs=1; was sub_00f1cad0.
//   ?GetParentScript@CCagedBalverine@CQ_ArenaScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_ArenaScript.h (shared header: nested class CCagedBalverine, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_ArenaScript.h"
CScriptBase* NScript::CQ_ArenaScript::CCagedBalverine::GetParentScript() const
{
    return &ParentClass;
}
