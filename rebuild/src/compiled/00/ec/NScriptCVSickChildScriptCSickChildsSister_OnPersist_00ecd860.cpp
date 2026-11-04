// NScript::CV_SickChildScript::CSickChildsSister::OnPersist(CPersistContext& context) -- saves/loads LeadHeroToSickChild by name (default false)
// Leaf: ?OnPersist@CSickChildsSister@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECD860, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00ec55f1 "SickChildsSister" in 00ec54e0; Alloc 00ec67b0; vtable 012f1964 slot 4; PDB ?OnPersist@CSickChildsSister@CV_SickChildScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_LeadHeroToSickChild_00ecd860.
//   ?OnPersist@CSickChildsSister@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ecd860::PersistTransfer_LeadHeroToSickChild_00ecd860(CPersistContext* context) -- persists the "LeadHeroToSickChild" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD860, oracle length 33 ... (catalog stand-in _unattributed).
// Class: engine/CV_SickChildScript.h (shared header: CSickChildsSister declares bool LeadHeroToSickChild and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
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

void NScript::CV_SickChildScript::CSickChildsSister::OnPersist(CPersistContext& context)
{
    context.Transfer("LeadHeroToSickChild", LeadHeroToSickChild, false);
}
