// NScript::CV_SickChildScript::CMansLover::OnPersist(CPersistContext& context) -- saves/loads LeavingRegion by name (default false)
// Leaf: ?OnPersist@CMansLover@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECD930, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00ec571d "MansLover" in 00ec54e0; Alloc 00ec9c00; vtable 012f1d74 slot 4; PDB ?OnPersist@CMansLover@CV_SickChildScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_LeavingRegion_00ecd930.
//   ?OnPersist@CMansLover@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ecd930::PersistTransfer_LeavingRegion_00ecd930(CPersistContext* context) -- persists the "LeavingRegion" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD930, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: engine/CV_SickChildScript.h (shared header: CMansLover declares bool LeavingRegion and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_SickChildScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_SickChildScript::CMansLover::OnPersist(CPersistContext& context)
{
    context.Transfer("LeavingRegion", LeavingRegion, false);
}
