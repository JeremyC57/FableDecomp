#pragma optimize("s", on)
// CTCInventoryExperience::UpdateExternalExperienceSpendStat(EHeroTrainableStatType, long) -- forwards the experience spent on a trainable stat to the hero's CTCHeroExperience (SetExperienceSpentOnStat)
// Leaf: UpdateExternalExperienceSpendStat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00572672, length 23 bytes (unaligned, no padding: the 0x56xxxx-0x57xxxx inventory
// module is built size-optimised). Manifest label "CTCHero::GiveTutorialForCategory" is propagated; the name is the Ego_r.pdb
// symbol ?UpdateExternalExperienceSpendStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@J@Z. One caller,
// CTCInventoryExperience::SpendExperienceOnStat @ 0x005746F7 (call site 0x00574797).
// Callees: CTCInventoryExperience::GetTCHeroExperience @ 0x0056FF3D, CTCHeroExperience::SetExperienceSpentOnStat @ 0x006D65C0
// (`ExperienceSpentOnStats[stat] = amount`; manifest "OnRegistedZSortingPatchRelocation" is propagated).
// Shape: under optimize("s") both stack arguments are re-pushed straight from the caller's frame (`push [esp+8]` twice, right to
// left) BEFORE the getter call, whose result becomes `this` for the setter; `ret 8`.
// Pragma: optimize("s", on) (the speed form loads the arguments into registers first).
// CTCHeroExperience_Calls: engine/CTCHeroExperience.h holds the PDB data layout of CTCHeroExperience; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
#include "engine/CTCInventoryExperience.h"

enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

struct CTCHeroExperience_Calls {
    void SetExperienceSpentOnStat(EHeroTrainableStatType stat, long amount);   // 0x006D65C0
};

struct CTCInventoryExperience_Methods : CTCInventoryExperience {
    CTCHeroExperience_Calls* GetTCHeroExperience();                                  // 0x0056FF3D
    void UpdateExternalExperienceSpendStat(EHeroTrainableStatType stat, long amount);
};

void CTCInventoryExperience_Methods::UpdateExternalExperienceSpendStat(EHeroTrainableStatType stat, long amount)
{
    GetTCHeroExperience()->SetExperienceSpentOnStat(stat, amount);
}