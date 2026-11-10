// CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) -- the experience cost of the ability's level levelsAhead past the current one, less the cost times the ability's morality cost factor times the hero's morality ratio when asked; -1 when there is no such level
// Leaf: ?GetExperienceCostToUpgradeAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@J_N@Z
// Retail-asm: 0x005EE86C `fistp dword ptr [ebp+0Ch]` -- `cost * factor * ratio` stored to the dead applyMorality slot, reloaded
//   with `fld dword ptr [ebp+10h]` (0x005EE869) and stored with a bare dword fistp: the engine's inline-assembly float-to-long
//   GFFloatToLongNear (shared engine/GFFloatToLongNear.h; VC7.1 C++ gives __ftol2 or `fistp qword`). No asm in this row.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE77C, 267 bytes, thiscall `ret 0Ch`; frame-pointer size TU (optimize "s").
//   Ego_r.pdb ?GetExperienceCostToUpgradeAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@J_N@Z (public const).
// Callees: std::bitset<274>::test 0x00410DE0 + const CVectorMap::LowerBound 0x004365B0 (inline CThing::PeekTC<const CTCHeroStats>,
//   interface 4), CTCHeroStats::GetMoralityMax 0x0057A67E (Ego_r ?GetMoralityMax@CTCHeroStats@@QBEJXZ).
// Layout: Abilities +0x160 (8-byte pair<long,CDefPointer<const CHeroAbilityDef>>); def ExperienceCostsToUpgrade +0x80,
//   MoralityCostFactor +0x90; hero stats Morality +0x28.
// Shape: the morality read through the inline CTCHeroStats::GetMorality() (the direct member read fild's [ecx+28h] in place;
//   retail copies it through the dead ability slot), and the reduced cost kept in its own `result` copied from cost before
//   the applyMorality test (retail loads eax from cost ahead of the `je`, the fistp result reusing cost's slot [ebp+0Ch]);
//   `return cost - ...` or `cost -= ...` load cost after the fistp, 265 v 267.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#include "engine/GFFloatToLongNear.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CTCInventoryAbilities.h"

long CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) const
{
    if ((unsigned long)ability < Abilities.size() && Abilities[ability].second.Get() != 0 &&
        Abilities[ability].first + levelsAhead <= (long)Abilities[ability].second->ExperienceCostsToUpgrade.size() - 1) {
        long cost = Abilities[ability].second->ExperienceCostsToUpgrade[Abilities[ability].first + levelsAhead];
        const CTCHeroStats* stats;
        if (ParentThing->PeekTC(&stats)) {
            float ratio = (float)stats->GetMorality() / (float)stats->GetMoralityMax();
            long result = cost;
            if (applyMorality)
                result -= GFFloatToLongNear(cost * Abilities[ability].second->MoralityCostFactor * ratio);
            return result;
        }
        return cost;
    }
    return -1;
}