// NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@COakValeBanditRaidBandit@CQ_OakValeBanditRaidScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBFE70, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00dbf633 "OakValeBanditRaidBandit" in 00dbf2d0; Alloc 00dbfdd0; vtable 012d9f64 slot 3; PDB ?GetParentScript@COakValeBanditRaidBandit@CQ_OakValeBanditRaidScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00dbfe70.
//   ?GetParentScript@COakValeBanditRaidBandit@CQ_OakValeBanditRaidScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_OakValeBanditRaidScript.h (shared header: nested class COakValeBanditRaidBandit, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQ_OakValeBanditRaidScript.h"
CScriptBase* NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit::GetParentScript() const
{
    return &ParentClass;
}
