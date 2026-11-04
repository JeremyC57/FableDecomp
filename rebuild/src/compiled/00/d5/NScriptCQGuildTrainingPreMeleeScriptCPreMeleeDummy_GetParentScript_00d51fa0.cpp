// NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeDummy::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CPreMeleeDummy@CQ_GuildTrainingPreMeleeScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D51FA0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d515d2 "PreMeleeDummy" in 00d51520; Alloc 00d51f00; vtable 012cf858 slot 3; PDB ?GetParentScript@CPreMeleeDummy@CQ_GuildTrainingPreMeleeScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d51fa0.
//   ?GetParentScript@CPreMeleeDummy@CQ_GuildTrainingPreMeleeScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_GuildTrainingPreMeleeScript.h (shared header: nested class CPreMeleeDummy, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQ_GuildTrainingPreMeleeScript.h"
CScriptBase* NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeDummy::GetParentScript() const
{
    return &ParentClass;
}
