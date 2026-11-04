// NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::OnPersist(CPersistContext& context) -- saves/loads GuardianSpokeToHero by name (default false)
// Leaf: ?OnPersist@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E26E00, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012e1f08 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e268c0 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQS_GuardianSisterInfo2_SisterInBanditCampScript; PDB ?OnPersist@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@; refs=1; was _unattributed::PersistTransfer_GuardianSpokeToHero_00e26e00.
//   ?OnPersist@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e26e00::PersistTransfer_GuardianSpokeToHero_00e26e00(CPersistContext* context) -- persists the "GuardianSpokeToHero" flag at +0x48 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E26E00, oracle length 33 ... (catalog stand-in _unattributed).
// Class: engine/CQS_GuardianSisterInfo2_SisterInBanditCampScript.h (shared header: CQS_GuardianSisterInfo2_SisterInBanditCampScript declares bool GuardianSpokeToHero and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
#include "engine/CPersistContext.h"

void NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::OnPersist(CPersistContext& context)
{
    context.Transfer("GuardianSpokeToHero", GuardianSpokeToHero, false);
}
