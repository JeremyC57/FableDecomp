// NScript::CQ_GuildTrainingScript::CWillDummy::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CWillDummy@CQ_GuildTrainingScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D43400, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d3c3bf "WillDummy" in 00d3bc60; Alloc 00d43360; vtable 012ce364 slot 3; PDB ?GetParentScript@CWillDummy@CQ_GuildTrainingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d43400.
//   ?GetParentScript@CWillDummy@CQ_GuildTrainingScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_GuildTrainingScript.h (shared header: nested class CWillDummy, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_GuildTrainingScript.h"
CScriptBase* NScript::CQ_GuildTrainingScript::CWillDummy::GetParentScript() const
{
    return &ParentClass;
}
