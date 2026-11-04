// NScript::CPersonalScript_GlobalThingsScript::CMK_HS3_SOUL::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CMK_HS3_SOUL@CPersonalScript_GlobalThingsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE0500, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00cdea77 "MK_HS3_SOUL" in 00cde420; Alloc 00ce0460; vtable 012c34f8 slot 3; PDB ?GetParentScript@CMK_HS3_SOUL@CPersonalScript_GlobalThingsScript@NScript@@; refs=1; was sub_00ce0500.
//   ?GetParentScript@CMK_HS3_SOUL@CPersonalScript_GlobalThingsScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CPersonalScript_GlobalThingsScript.h (shared header: nested class CMK_HS3_SOUL, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CPersonalScript_GlobalThingsScript.h"
CScriptBase* NScript::CPersonalScript_GlobalThingsScript::CMK_HS3_SOUL::GetParentScript() const
{
    return &ParentClass;
}
