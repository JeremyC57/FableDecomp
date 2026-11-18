// CTCInventoryExperience::GetHeroTrainableStatType(long button) -- map the selected category + button to a trainable stat
// Leaf: ?GetHeroTrainableStatType@CTCInventoryExperience@@QAE?AW4EHeroTrainableStatType@@J@Z
// Retail 0x0056E632, 66 bytes, ret 4. Only caller: UpdateExperienceSpendStat (0x0057229B, which binds this spelling).
// Shape: the selected category's identifier (0 when nothing is selected) is the first key; a linear scan of the static 10-entry table at
//   0x0137C8E8 ({result, category, button}, 12 bytes) stops at the first {category, button} match and returns that entry's result. The loop is
//   strength-reduced to a pointer at the `button` word; an unmatched scan reads entry [10] -- retail has no not-found case.
// Callees: NInventory::CTCInventoryBase::GetSelectedCategory 0x005BCACE, NInventory::CCategory::GetCategoryIdentifier 0x005BD1B3.
// Name fix (agentNF89, q11170): Ego_r.pdb + FableWin.pdb ?GetHeroTrainableStatType@CTCInventoryExperience@@QAE?AW4EHeroTrainableStatType@@J@Z (the
//   return is the global enum; the old leaf returned long). Moved off the row-local CTCInventoryExperience / CCategory / empty-enum models onto
//   engine/CTCInventoryExperience.h (overlay opt-in CTCINVENTORYEXPERIENCE_TRAINABLESTATTYPE), NInventory_CTCInventoryBase.h (SELECTEDOWNED) and
//   CCategory.h (PASS22, CCATEGORY_CATEGORYIDENTIFIER). The table keeps its descriptive name: no PDB, label or sibling repo names the file
//   static at 0x0137C8E8 (descriptive, see NOTES).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_TRAINABLESTATTYPE
#include "engine/ETCInterfaceType.h"
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"
struct STrainableEntry {
    long Result;        // +0x0 EHeroTrainableStatType
    long Category;      // +0x4 NInventory::EInventoryCategoryIdentifier
    long Button;        // +0x8
};
extern STrainableEntry g_TrainableStatTable[10];   // 0x0137C8E8 (file static of the experience TU; descriptive name)
EHeroTrainableStatType CTCInventoryExperience::GetHeroTrainableStatType(long button)
{
    long category = 0;
    NInventory::CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    long i;
    for (i = 0; i < 10; ++i) {
        if (g_TrainableStatTable[i].Category == category && g_TrainableStatTable[i].Button == button)
            break;
    }
    return (EHeroTrainableStatType)g_TrainableStatTable[i].Result;
}
