// Behaviour test for 0x0047DC69 PCLoad: the yes/no dialog and the mouse pointer are released from the UI manager, the
// pointer is re-added and told event 0x19, the dialog's YES / NO buttons are released when present, and the hero's
// in-game menu copies SaveGameToLoad into LoadGameIndex.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void logf(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct CObserver {
    const char* name;
    long lastEvent;
    CObserver() : name(""), lastEvent(0) { }
    void ObserveEvent(long e) { lastEvent = e; }
};
struct CComponent : CObserver {
    CComponent* yes;
    CComponent* no;
    CComponent() : yes(0), no(0) { }
    CComponent* FindChild(const char* n)
    {
        if (std::strcmp(n, "PC_YESNO_GAME_YESBUTTON") == 0) return yes;
        if (std::strcmp(n, "PC_YESNO_GAME_NOBUTTON") == 0) return no;
        return 0;
    }
};
struct CManager {
    void AddObserver(CObserver* o) { std::strcat(g_log, "add:"); logf(o->name); }
    void ReleaseObserver(CObserver* o) { std::strcat(g_log, "rel:"); logf(o ? o->name : "null"); }
};
static CManager g_manager;
static CManager* GetInstance() { return &g_manager; }

struct CTCInGameMenu { long LoadGameIndex; long SaveGameToLoad; };
struct CThing {
    bool hasMenu;
    CTCInGameMenu* menu;
    bool HasInterface(long type) const { return type == 0xC6 && hasMenu; }
    CTCInGameMenu* GetInterfaceUnchecked(long) const { return menu; }
};
static CThing* g_hero;

static CComponent* g_pPCYesNoDialog;
static CComponent* g_pPCMousePointer;

// The body as reconstructed (src/0047dc69.cpp), over the test's models.
static void PCLoad()
{
    GetInstance()->ReleaseObserver(g_pPCYesNoDialog);
    GetInstance()->ReleaseObserver(g_pPCMousePointer);
    GetInstance()->AddObserver(g_pPCMousePointer);
    g_pPCMousePointer->ObserveEvent(0x19);
    CComponent* yesButton = g_pPCYesNoDialog->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (yesButton)
        GetInstance()->ReleaseObserver(yesButton);
    CComponent* noButton = g_pPCYesNoDialog->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (noButton)
        GetInstance()->ReleaseObserver(noButton);
    CTCInGameMenu* menu = 0;
    CThing* hero = g_hero;
    if (hero->HasInterface(0xC6))
        menu = hero->GetInterfaceUnchecked(0xC6);
    menu->LoadGameIndex = menu->SaveGameToLoad;
}

int main()
{
    int ok = 1;
    CComponent dialog, pointer, yes, no;
    dialog.name = "dialog"; pointer.name = "pointer"; yes.name = "yes"; no.name = "no";
    CTCInGameMenu menu; menu.LoadGameIndex = -1; menu.SaveGameToLoad = 3;
    CThing hero; hero.hasMenu = true; hero.menu = &menu;
    g_hero = &hero; g_pPCYesNoDialog = &dialog; g_pPCMousePointer = &pointer;

    dialog.yes = &yes; dialog.no = &no;
    g_log[0] = 0;
    PCLoad();
    if (std::strcmp(g_log, "rel:dialog;rel:pointer;add:pointer;rel:yes;rel:no;") != 0) ok = 0;
    if (pointer.lastEvent != 0x19 || menu.LoadGameIndex != 3) ok = 0;

    dialog.yes = 0; dialog.no = &no; menu.SaveGameToLoad = 5;
    g_log[0] = 0;
    PCLoad();
    if (std::strcmp(g_log, "rel:dialog;rel:pointer;add:pointer;rel:no;") != 0) ok = 0;
    if (menu.LoadGameIndex != 5) ok = 0;

    dialog.yes = &yes; dialog.no = 0; menu.SaveGameToLoad = 0;
    g_log[0] = 0;
    PCLoad();
    if (std::strcmp(g_log, "rel:dialog;rel:pointer;add:pointer;rel:yes;") != 0) ok = 0;
    if (menu.LoadGameIndex != 0) ok = 0;

    std::printf("0047dc69_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}