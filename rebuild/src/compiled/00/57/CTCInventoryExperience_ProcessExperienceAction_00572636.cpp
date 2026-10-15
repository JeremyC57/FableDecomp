// CTCInventoryExperience::ProcessExperienceAction() -- route the spend action by the selected category: stats or abilities
// Retail 0x00572636, 60 bytes. Nothing selected or a category identifier <= 0 does nothing;
// identifiers 1..2 always update the stat spending (UpdateExperienceSpendStat 0x0057229B); 3..5 update
// the ability spending (UpdateExperienceSpendAbility 0x0057238F) when the stat indicator (+0x1BC) is
// off its first row, else the stat spending; larger identifiers do nothing. Both updates are tail jumps
// sharing the `mov ecx,esi ; pop esi` epilogue. The `test ; jle / cmp 2 ; jle / cmp 5 ; jg` ladder is
// VC's binary-search lowering of a `switch` with the case groups {1,2} and {3,4,5} (an if/else chain
// lays the {1,2} arm out first).
// Callees: CTCInventoryBase::GetSelectedCategory 0x005BCACE, CCategory::GetCategoryIdentifier 0x005BD1B3.
// statIndicatorPos is +0x1BC in retail (PDB +0x200; 12-byte STLport containers ahead), local model.
#pragma optimize("s", on)

struct CCategory {
    long GetCategoryIdentifier();       // 0x005BD1B3
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x1BC];
    long statIndicatorPos;              // +0x1BC
    CCategory* GetSelectedCategory();   // 0x005BCACE (CTCInventoryBase)
    void UpdateExperienceSpendStat();   // 0x0057229B
    void UpdateExperienceSpendAbility();    // 0x0057238F
    void ProcessExperienceAction();
};

void CTCInventoryExperience::ProcessExperienceAction()
{
    CCategory* selected = GetSelectedCategory();
    if (selected) {
        switch (selected->GetCategoryIdentifier()) {
        case 1:
        case 2:
            UpdateExperienceSpendStat();
            break;
        case 3:
        case 4:
        case 5:
            if (statIndicatorPos)
                UpdateExperienceSpendAbility();
            else
                UpdateExperienceSpendStat();
            break;
        }
    }
}