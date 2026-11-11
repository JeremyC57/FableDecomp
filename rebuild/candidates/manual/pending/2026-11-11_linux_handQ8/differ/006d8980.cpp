// CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat) -- raise a trainable stat one level (capped by its ability track), refresh that stat's abilities and the stat-level state, and age the hero by the global per-level-up increase
// Grade: DIFFER(180v183) -- now on shared engine/CTCHeroExperience.h (BLOCK_THRESHOLDS inline GetTrainableStatLevel/MaxLevel) + overlay FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT / FABLE_RETAIL_CTCHEROSTATS_AGE; residue: retail loads Abilities._M_start into ebx before the lea and keeps TrainableStatLevels in edi with level+1 through ebx (0x006D8997..0x006D89CD); every max/level local order tried leaves 3 regs. Next: c2prio on the start candidate.
// Leaf: ?UpgradeTrainableStat@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D8980, 183 bytes, __thiscall, `ret 4`. Ego_r.pdb / FableWin.pdb
//   ?UpgradeTrainableStat@CTCHeroExperience@@QAEXW4EHeroTrainableStatType@@@Z.
// Callees: CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) 0x006D8170, OnStatLevelChange(bool)
//   0x006D83B0, CTCBase::GetDefinitionManager() const 0x006869D0, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCHeroStats>, type 4),
//   CTCHeroStats::SetAge(float) 0x0057B05F.
// Shape: the inline GetTrainableStatLevel / GetTrainableStatMaxLevel (the signed 72-byte `imul 38E38E39h ; sar 4` divide);
//   CGameDefinitionManager::PeekGlobalDef() (+0xD0) and CGlobalDef::AgeIncreasePerLevelUp (+0xD0) read before the stats lookup.
// Header overlay (hdr/): FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT and FABLE_RETAIL_CTCHEROSTATS_AGE (declarations).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT
#define FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS
#define FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_AGE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CAbilityTrackDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CTCHeroStats.h"

void CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat)
{
    if (GetTrainableStatMaxLevel(stat) > GetTrainableStatLevel(stat)) {
        TrainableStatLevels[stat] = GetTrainableStatLevel(stat) + 1;
        UpdateAbilitiesForStat(stat, false);
        OnStatLevelChange(true);
        const CGlobalDef& global_def = GetDefinitionManager().PeekGlobalDef();
        CTCHeroStats* hero_stats;
        if (GetParentThing().GetTC(&hero_stats))
            hero_stats->SetAge(hero_stats->GetAge() + global_def.AgeIncreasePerLevelUp);
    }
}
