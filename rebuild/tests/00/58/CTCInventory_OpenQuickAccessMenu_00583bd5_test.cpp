// Behaviour test for CTCInventory::OpenQuickAccessMenu 0x00583BD5 (model copy with stub components)
#include <cstdio>

static int g_opened = 0;
struct CTCInGameMenu { bool InGameMenuOpen; };
struct CTCQuestStartTarget { bool Active; };
struct CTCInventoryMenu {
    bool active, closing;
    bool IsActive() const { return active; }
    bool IsClosing() const { return closing; }
    void Open() { ++g_opened; }
};
struct CThing {
    CTCInGameMenu* inGameMenu; CTCQuestStartTarget* questStart; CTCInventoryMenu* menu;
    bool hasInGameMenu, hasQuestStart, hasMenu;
    template<class T> bool GetInterface(T** out, long wanted) const
    {
        if (wanted == 0xc6 && hasInGameMenu) { *out = (T*)inGameMenu; return true; }
        if (wanted == 0xfc && hasQuestStart) { *out = (T*)questStart; return true; }
        if (wanted == 0xf6 && hasMenu) { *out = (T*)menu; return true; }
        return false;
    }
};
struct CManager { bool ScreenShowing; };
struct CPlayerGui {
    bool GuiBusy; CManager manager;
    CManager* GetPHeroInfoScreensManager() const { return const_cast<CManager*>(&manager); }
};
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;

struct CTCInventory {
    CThing* ParentThing;
    const CThing* GetConstParent() const { return ParentThing; }
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

int main()
{
    CTCInGameMenu igm = { false }; CTCQuestStartTarget qs = { false }; CTCInventoryMenu menu = { false, false };
    CThing hero = { &igm, &qs, &menu, true, true, true };
    CTCInventory inv; inv.ParentThing = &hero;
    inv.OpenQuickAccessMenu();                    // opens
    igm.InGameMenuOpen = true; inv.OpenQuickAccessMenu();   // blocked
    igm.InGameMenuOpen = false; g_gui.GuiBusy = true; inv.OpenQuickAccessMenu();   // blocked
    g_gui.GuiBusy = false; menu.closing = true; inv.OpenQuickAccessMenu();         // closing: no open
    menu.closing = false; hero.hasInGameMenu = false; hero.hasQuestStart = false; inv.OpenQuickAccessMenu();   // opens
    if (g_opened != 2) { std::printf("00583bd5_TEST FAIL\n"); return 1; }
    std::printf("00583bd5_TEST PASS\n");
    return 0;
}