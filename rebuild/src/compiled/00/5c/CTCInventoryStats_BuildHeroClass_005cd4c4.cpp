// Flags: (none)
// Leaf: ?BuildHeroClass@CTCInventoryStats@@QAE?AVCWideString@@XZ
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_STLP_BITSET_TEST_OWNER_PASS27
// Inline-budget: 005cd4de passes UsingComponents to bitset::test. Ordinary inline calls HasTC on this.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_SPENTEXPERIENCE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCHeroStats.h"
#include "engine/EHeroTrainableStatType.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "fable_assert.h"
#pragma optimize("y",off)
#pragma optimize("s",on)
CWideString CTCInventoryStats::BuildHeroClass()
{
    CWideString res;
    CTCHeroStats* hero_stats;
    GetParentThing().GetTC(&hero_stats);
    CCharString Tag("TEXT_GUI_STATS_HEROCLASS_");
    long stexp = hero_stats->GetHeroStatExperience(HERO_STAT_STRENGTH);
    long skexp = hero_stats->GetHeroStatExperience(HERO_STAT_SKILL);
    long mexp = hero_stats->GetHeroStatExperience(HERO_STAT_WILL);
    CCharString middle;
    if (stexp >= skexp && stexp >= mexp)
        middle = "PHYSICAL_";
    else if (skexp >= stexp && skexp >= mexp)
        middle = "SKILL_";
    else if (mexp >= stexp && mexp >= skexp)
        middle = "MAGIC_";
    FABLE_RETAIL_ASSERT(middle.GetLength() != 0);
    Tag += middle;
    std::vector<long>& expon = hero_stats->GetExperienceSpentOn();
    bool first = true;
    long maxexp = 0;
    EHeroTrainableStatType maxstat = HERO_STAT_STRENGTH_PHYSIQUE;
    for (long i = 0; i != expon.size(); ++i) {
        if (first || expon[i] >= maxexp) {
            first = false;
            maxstat = (EHeroTrainableStatType)i;
            maxexp = expon[i];
        }
    }
    CCharString last;
    if (maxexp == 0)
        last = "NONE";
    else {
        if (maxstat == HERO_STAT_STRENGTH_PHYSIQUE)
            last = "STRENGTH";
        if (maxstat == HERO_STAT_STRENGTH_TOUGHNESS)
            last = "TOUGHNESS";
        if (maxstat == HERO_STAT_STRENGTH_HEALTH)
            last = "HEALTH";
        if (maxstat == HERO_STAT_SKILL_SPEED)
            last = "SPEED";
        if (maxstat == HERO_STAT_SKILL_ACCURACY)
            last = "ACCURACY";
        if (maxstat == HERO_STAT_SKILL_STEALTH)
            last = "GUILE";
        if (maxstat == HERO_STAT_WILL_MAGIC_POWER)
            last = "MAGICPOWER";
        if (maxstat == NUMBER_OF_TRAINABLE_HERO_STATS)
            last = "SPELLS";
    }
    FABLE_RETAIL_ASSERT(last.GetLength() != 0);
    Tag += last;
    const NGameText::CDataBank& TextBank = CMainGameComponent::Get()->GetTextBank();
    return TextBank.GetTextBySymbol(Tag);
}