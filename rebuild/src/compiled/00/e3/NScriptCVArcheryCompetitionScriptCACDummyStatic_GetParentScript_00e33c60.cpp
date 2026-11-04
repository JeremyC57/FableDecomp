// NScript::CV_ArcheryCompetitionScript::CAC_DummyStatic::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CAC_DummyStatic@CV_ArcheryCompetitionScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E33C60, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e32606 "AC_DummyStatic" in 00e32540; Alloc 00e33bc0; vtable 012e315c slot 3; PDB ?GetParentScript@CAC_DummyStatic@CV_ArcheryCompetitionScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e33c60.
//   ?GetParentScript@CAC_DummyStatic@CV_ArcheryCompetitionScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_ArcheryCompetitionScript.h (shared header: nested class CAC_DummyStatic, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CV_ArcheryCompetitionScript.h"
CScriptBase* NScript::CV_ArcheryCompetitionScript::CAC_DummyStatic::GetParentScript() const
{
    return &ParentClass;
}
