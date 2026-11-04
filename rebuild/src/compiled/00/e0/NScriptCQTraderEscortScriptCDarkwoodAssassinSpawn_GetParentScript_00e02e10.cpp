// NScript::CQ_TraderEscortScript::CDarkwoodAssassinSpawn::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CDarkwoodAssassinSpawn@CQ_TraderEscortScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E02E10, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e00f91 "DarkwoodAssassinSpawn" in 00e00db0; Alloc 00e02d70; vtable 012df594 slot 3; PDB ?GetParentScript@CDarkwoodAssassinSpawn@CQ_TraderEscortScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e02e10.
//   ?GetParentScript@CDarkwoodAssassinSpawn@CQ_TraderEscortScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_TraderEscortScript.h (shared header: nested class CDarkwoodAssassinSpawn, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_TraderEscortScript.h"
CScriptBase* NScript::CQ_TraderEscortScript::CDarkwoodAssassinSpawn::GetParentScript() const
{
    return &ParentClass;
}
