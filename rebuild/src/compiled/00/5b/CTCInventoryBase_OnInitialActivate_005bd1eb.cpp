// CTCInventoryBase::OnInitialActivate() -- unless the quest manager is between a quest's start and finish screens, strips every category of its non-quest-persistent items
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD1EB, length 74 bytes; size-optimised TU (`push 2Ch ; pop ecx` divisor).
// Callees: CQuestManager::IsBetweenQuestStartAndFinishScreens @ 0x004AF290 on the global quest manager @ 0x013B89FC;
// CCategory::RemoveAllNonQuestPersistentItems @ 0x005BC836 (landed).
// Retail CTCInventoryBase: no engine header; Categories (std::vector<NInventory::CCategory>, 0x2C-byte elements,
// {first, last, end}) sits at +0x20 (PDB +0x24 minus the 4-byte-narrower Components vector). The loop re-reads
// size() every iteration (`sub ; cdq ; idiv`, unsigned `jb` compare) and re-indexes `first` through the stride register.
#pragma optimize("s", on)
struct CQuestManager {
    bool IsBetweenQuestStartAndFinishScreens();  // 0x004AF290
};
extern CQuestManager* g_pQuestManager;           // 0x013B89FC

struct CCategory_Retail {
    unsigned char _body[0x2C];
    void RemoveAllNonQuestPersistentItems();     // 0x005BC836
};
struct CCategoryVector {                         // retail std::vector<CCategory>
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                  // +0x20
    void OnInitialActivate();
};

void CTCInventoryBase_Retail::OnInitialActivate()
{
    if (!g_pQuestManager->IsBetweenQuestStartAndFinishScreens()) {
        for (unsigned long i = 0; i < Categories.size(); ++i)
            Categories[i].RemoveAllNonQuestPersistentItems();
    }
}