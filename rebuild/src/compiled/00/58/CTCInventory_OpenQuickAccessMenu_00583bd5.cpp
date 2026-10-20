// CTCInventory::OpenQuickAccessMenu() -- opens the owner's quick-access inventory menu unless the in-game menu or the quest start screen is up, the GUI is busy or a hero information screen is showing, and only when the menu is neither active nor closing
// Retail 0x00583BD5, 255 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (two const inline GetInterface
// lookups: the in-game menu component 0xC6 and the quest start component 0xFC) and the non-const LowerBound 0x0040F020
// (the inventory menu component 0xF6), CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (global 0x013B8790),
// CTCInventoryMenu::IsActive() const 0x00625930 / IsClosing() const 0x00625940 (manifest "GetCanItemBeQuickAssigned"
// labels are propagated) and the open call 0x00628000 (manifest "CTCQuickAccessMenu::Activate").
// Shape: each blocker is `HasTC && component && component->flag (+0xC)` through the const lookup; the type constant
// lives in edi and the lookup key in the one frame word; every blocker returns early.
// Layout: CTCInGameMenu::InGameMenuOpen +0x0C (engine header); the quest start component's flag +0x0C (descriptive);
// CPlayerGui busy byte +0xF6, the hero information screen manager's showing byte +0x34 (descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCInGameMenu {                               // interface type 0xC6
    unsigned char _pad_0x00[0x0c];
    bool InGameMenuOpen;                             // +0x0c
};
struct CTCQuestStartTarget {                         // interface type 0xFC
    unsigned char _pad_0x00[0x0c];
    bool Active;                                     // +0x0c (descriptive)
};
struct CTCInventoryMenu {                            // interface type 0xF6
    bool IsActive() const;                           // 0x00625930
    bool IsClosing() const;                          // 0x00625940
    void Open();                                     // 0x00628000
};
namespace NHeroInformationScreens {
struct CManager {
    unsigned char _pad_0x00[0x34];
    bool ScreenShowing;                              // +0x34 (descriptive)
};
}
struct CPlayerGui {
    unsigned char _pad_0x000[0xf6];
    bool GuiBusy;                                    // +0xf6 (descriptive)
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CTCInventory {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    __forceinline const CThing* GetConstParent() const { return ParentThing; }
    void OpenQuickAccessMenu();
};

void CTCInventory::OpenQuickAccessMenu()
{
    CTCInGameMenu* inGameMenu;
    if (GetConstParent()->GetInterface(&inGameMenu, 0xc6) && inGameMenu && inGameMenu->InGameMenuOpen)
        return;
    CTCQuestStartTarget* questStart;
    if (GetConstParent()->GetInterface(&questStart, 0xfc) && questStart && questStart->Active)
        return;
    if (g_pPlayerGui->GuiBusy)
        return;
    if (g_pPlayerGui->GetPHeroInfoScreensManager()->ScreenShowing)
        return;
    CTCInventoryMenu* menu;
    if (ParentThing->GetInterface(&menu, 0xf6)) {
        if (!menu->IsActive() && !menu->IsClosing())
            menu->Open();
    }
}