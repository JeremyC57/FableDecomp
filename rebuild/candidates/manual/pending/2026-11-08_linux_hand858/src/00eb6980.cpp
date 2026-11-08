// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::OnPersist(CPersistContext& context) -- saves/loads HasBeenThreatened and HasGivenIntroToHero by name (default false)
// Leaf: ?OnPersist@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6980, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0
//   (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound) twice, each with a `false` default temporary.
// Identity: entity vtable slot 4 (.rdata ref 0x012F00CC; slot 0 scalar dtor 0x00EB6310, 1 Main 0x00EB3F40, 2 Init 0x00EB3E40,
//   3 GetParentScript 0x00EB3D00, 5 OnPredicateFail 0x00EB3D10, all CMurderTwistAssassin); Ego_r.pdb / FableWin.pdb
//   ?OnPersist@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z (private virtual). The keys
//   "HasBeenThreatened" (0x012F0504) and "HasGivenIntroToHero" (0x012F04F0) are the PDB member names at +0x30 / +0x38.
// Class: engine/CV_MurderTwistFinalScript.h under FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN (declares OnPersist); includes as
//   the landed includer 0x00EB4760. Sibling: CMurderTwistAssassinGuard::OnPersist 0x00EB69D0 (landed, one Transfer).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::OnPersist(CPersistContext& context)
{
    context.Transfer("HasBeenThreatened", HasBeenThreatened, false);
    context.Transfer("HasGivenIntroToHero", HasGivenIntroToHero, false);
}
