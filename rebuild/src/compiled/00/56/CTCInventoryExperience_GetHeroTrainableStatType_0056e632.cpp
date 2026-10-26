// CTCInventoryExperience::GetHeroTrainableStatType(long button) -- map the selected category + button to a trainable stat
// Retail 0x0056E632, 66 bytes, ret 4. Only caller: UpdateExperienceSpendStat (0x0057229B), so this is
// the STAT lookup (the manifest label GetHeroTrainableAbilityType is propagated from the 20-entry twin
// 0x0056E674 that UpdateExperienceSpendAbility calls).
// Shape: the selected category's identifier (0 when nothing is selected) is the first key; a linear
// scan of the static 10-entry table at 0x0137C8E8 ({result, category, button}, 12 bytes) stops at the
// first {category, button} match and returns that entry's result. The loop is strength-reduced to a
// pointer at the `button` word (`cmp [eax-4],esi ; ... ; cmp eax,end ; jl`); an unmatched scan reads
// entry [10] -- retail has no not-found case.
// Callees: CTCInventoryBase::GetSelectedCategory 0x005BCACE, CCategory::GetCategoryIdentifier 0x005BD1B3.
// Size-optimised module: `#pragma optimize("s", on)`.
// Leaf: GetHeroTrainableStatType
#pragma optimize("s", on)

struct CCategory {
    long GetCategoryIdentifier();       // 0x005BD1B3
};

struct STrainableEntry {
    long Result;        // +0x0 EHeroTrainableStatType
    long Category;      // +0x4 EInventoryCategoryIdentifier
    long Button;        // +0x8
};

extern STrainableEntry g_TrainableStatTable[10];   // 0x0137C8E8

class CTCInventoryExperience { public:
    CCategory* GetSelectedCategory();   // 0x005BCACE (CTCInventoryBase)
    long GetHeroTrainableStatType(long button);
};

long CTCInventoryExperience::GetHeroTrainableStatType(long button)
{
    long category = 0;
    CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    long i;
    for (i = 0; i < 10; ++i) {
        if (g_TrainableStatTable[i].Category == category && g_TrainableStatTable[i].Button == button)
            break;
    }
    return g_TrainableStatTable[i].Result;
}