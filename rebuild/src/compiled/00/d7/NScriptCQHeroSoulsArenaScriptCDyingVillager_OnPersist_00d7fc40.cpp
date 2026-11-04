// NScript::CQ_HeroSoulsArenaScript::CDyingVillager::OnPersist(CPersistContext& context) -- saves/loads PlayedOnce by name (default false)
// Leaf: ?OnPersist@CDyingVillager@CQ_HeroSoulsArenaScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7FC40, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00d7cc57 "DyingVillager" in 00d7c890; Alloc 00d7f380; vtable 012d3f44 slot 4; PDB ?OnPersist@CDyingVillager@CQ_HeroSoulsArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_PlayedOnce_00d7fc40.
//   ?OnPersist@CDyingVillager@CQ_HeroSoulsArenaScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d7fc40::PersistTransfer_PlayedOnce_00d7fc40(CPersistContext* context) -- persists the "PlayedOnce" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7FC40, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: engine/CQ_HeroSoulsArenaScript.h (shared header: CDyingVillager declares bool PlayedOnce and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_HeroSoulsArenaScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_HeroSoulsArenaScript::CDyingVillager::OnPersist(CPersistContext& context)
{
    context.Transfer("PlayedOnce", PlayedOnce, false);
}
