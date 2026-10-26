// CTCInventoryExperience::GetHeroTrainableAbilityType(long button) -- map the selected category + button to a trainable ability
// Retail 0x0056E674, 66 bytes, ret 4. Only caller: UpdateExperienceSpendAbility (0x0057238F); the
// twin 0x0056E632 is the 10-entry stat lookup.
// Shape: the selected category's identifier (0 when nothing is selected) is the first key; a linear
// scan of the static 20-entry table at 0x0137C960 ({result, category, button}, 12 bytes) stops at the
// first {category, button} match and returns that entry's result. The loop is strength-reduced to a
// pointer at the `button` word (`cmp [eax-4],esi ; ... ; cmp eax,end ; jl`); an unmatched scan reads
// entry [20] -- retail has no not-found case.
// Callees: CTCInventoryBase::GetSelectedCategory 0x005BCACE, CCategory::GetCategoryIdentifier 0x005BD1B3.
// Size-optimised module: `#pragma optimize("s", on)`.
#pragma optimize("s", on)

struct CCategory {
    long GetCategoryIdentifier();       // 0x005BD1B3
};

struct STrainableEntry {
    long Result;        // +0x0 EHeroAbility
    long Category;      // +0x4 EInventoryCategoryIdentifier
    long Button;        // +0x8
};

extern STrainableEntry g_TrainableAbilityTable[20];   // 0x0137C960

class CTCInventoryExperience { public:
    CCategory* GetSelectedCategory();   // 0x005BCACE (CTCInventoryBase)
    long GetHeroTrainableAbilityType(long button);
};

long CTCInventoryExperience::GetHeroTrainableAbilityType(long button)
{
    long category = 0;
    CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    long i;
    for (i = 0; i < 20; ++i) {
        if (g_TrainableAbilityTable[i].Category == category && g_TrainableAbilityTable[i].Button == button)
            break;
    }
    return g_TrainableAbilityTable[i].Result;
}