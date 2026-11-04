// NScript::CQ_RansomVictimScript::CRansomRevenge::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CRansomRevenge@CQ_RansomVictimScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE3800, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00de0bc4 "RansomRevenge" in 00de0850; Alloc 00de3760; vtable 012dd3d8 slot 3; PDB ?GetParentScript@CRansomRevenge@CQ_RansomVictimScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00de3800.
//   ?GetParentScript@CRansomRevenge@CQ_RansomVictimScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_RansomVictimScript.h (shared header: nested class CRansomRevenge, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_RansomVictimScript.h"
CScriptBase* NScript::CQ_RansomVictimScript::CRansomRevenge::GetParentScript() const
{
    return &ParentClass;
}
