// NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E26B90, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e26909 "MazeAtTavern" in 00e268c0; Alloc 00e26af0; vtable 012e1f74 slot 3; PDB ?GetParentScript@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e26b90.
//   ?GetParentScript@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h (shared header: nested class CMazeAtTavern, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h"
CScriptBase* NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::GetParentScript() const
{
    return &ParentClass;
}
