// Standalone C++03 behavior fixture containing the selected game-event body.
#include <stdio.h>
#include <stdlib.h>
#include <limits>

#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "line %d: %s\n", __LINE__, #expression); exit(1); } } while (0)

enum EPlayerMode { PLAYER_MODE_NULL=0, PLAYER_MODE_NAVIGATE_INVENTORY=4,
    PLAYER_MODE_NAVIGATE_IN_GAME_MENU=23, PLAYER_MODE_VIEW_HERO=47 };
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE=0, TUTORIAL_CATEGORY_INVENTORY=22, TUTORIAL_CATEGORY_WORLD_MAP=38 };

struct CGameEventDataPackage {
    CGameEventDataPackage() : Mode(PLAYER_MODE_NULL), Filled(false) {}
    unsigned long GetULONG() { CHECK(Filled); return Mode; }
    unsigned long Mode;
    bool Filled;
};
struct CGameEvent {
    CGameEvent() : Mode(PLAYER_MODE_NAVIGATE_INVENTORY), DataRequests(0) {}
    signed char GetPlayerNumber() const { return -1; }
    void GetDataPackage(CGameEventDataPackage* package) const { ++DataRequests; package->Mode = Mode; package->Filled = true; }
    EPlayerMode Mode;
    mutable unsigned int DataRequests;
};
struct CTCQuickAccessMenu {
    CTCQuickAccessMenu() : Active(false) {}
    bool IsActive() const { return Active; }
    bool Active;
};
struct CTCBoastUI : CTCQuickAccessMenu {};
struct CTCQuestCompletionUI : CTCQuickAccessMenu {};
struct CTCInGameMenu {
    CTCInGameMenu() : Open(false) {}
    bool IsOpen() const { return Open; }
    bool Open;
};
struct CTCInventory {
    CTCInventory() : Shortcut(0) {}
    long GetScreenToShortcut() const { return Shortcut; }
    long Shortcut;
};
struct CTCHero {
    CTCHero() : GiveTutorial(false), Calls(0), Category(TUTORIAL_CATEGORY_NONE) {}
    bool GiveTutorialForCategory(ETutorialCategory category, bool force) {
        CHECK(force);
        ++Calls;
        Category = category;
        return GiveTutorial;
    }
    bool GiveTutorial;
    unsigned int Calls;
    ETutorialCategory Category;
};

struct CThingPlayerCreature {
    CThingPlayerCreature() : Health(100), Dead(false), HasMenu(true), HasBoast(true), HasQuest(true),
        HasInGameMenu(true), HasInventory(true), InventoryPointer(&Inventory), HasHero(true), HeroPointer(&Hero) {}
    float GetHealth() const { return Health; }
    bool IsDead() const { return Dead; }
    bool PeekTC(const CTCQuickAccessMenu** result) const { if (HasMenu) *result = &Menu; return HasMenu; }
    bool PeekTC(const CTCBoastUI** result) const { if (HasBoast) *result = &Boast; return HasBoast; }
    bool PeekTC(const CTCQuestCompletionUI** result) const { if (HasQuest) *result = &Quest; return HasQuest; }
    bool PeekTC(const CTCInGameMenu** result) const { if (HasInGameMenu) *result = &InGameMenu; return HasInGameMenu; }
    bool PeekTC(const CTCInventory** result) const { if (HasInventory) *result = InventoryPointer; return HasInventory; }
    bool GetTC(CTCHero** result) { if (HasHero) *result = HeroPointer; return HasHero; }
    long GetJoystickDeviceNumber() const { return 2; }
    float Health;
    bool Dead, HasMenu, HasBoast, HasQuest, HasInGameMenu;
    CTCQuickAccessMenu Menu;
    CTCBoastUI Boast;
    CTCQuestCompletionUI Quest;
    CTCInGameMenu InGameMenu;
    CTCInventory Inventory;
    bool HasInventory;
    CTCInventory* InventoryPointer;
    CTCHero Hero;
    bool HasHero;
    CTCHero* HeroPointer;
};
template<class T> bool IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->IsDead(); }

struct CPlayer {
    CPlayer() : ViewHero(false), Creature(0) {}
    bool HasMode(EPlayerMode mode) const { CHECK(mode == PLAYER_MODE_VIEW_HERO); return ViewHero; }
    CThingPlayerCreature* GetPControlledCreature() const { return Creature; }
    bool ViewHero;
    CThingPlayerCreature* Creature;
};
struct CDisplayEngine {
    CDisplayEngine() : Rendering(true) {}
    bool IsRenderingNormalScene() const { return Rendering; }
    bool Rendering;
};
struct CMainGameComponent {
    static CMainGameComponent* Get() { return Instance; }
    const CDisplayEngine& PeekDisplayEngine() const { return Engine; }
    static CMainGameComponent* Instance;
    CDisplayEngine Engine;
};
CMainGameComponent* CMainGameComponent::Instance = 0;
struct CSubtitleRenderer {
    CSubtitleRenderer() : Displaying(false) {}
    bool IsDisplayingText() const { return Displaying; }
    bool Displaying;
};
namespace NHeroInformationScreens {
struct CManager {
    CManager() : Open(false) {}
    bool IsOpen() const { return Open; }
    bool Open;
};
}
struct CPlayerGui {
    CPlayerGui() : LiveGui(false), Toggles(0) {}
    static CPlayerGui& Get() { return *Instance; }
    CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; }
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() { return &ScreenManager; }
    bool IsLiveGUIMode() const { return LiveGui; }
    void ToggleLiveGui() { ++Toggles; LiveGui = !LiveGui; }
    static CPlayerGui* Instance;
    CSubtitleRenderer SubtitleRenderer;
    NHeroInformationScreens::CManager ScreenManager;
    bool LiveGui;
    unsigned int Toggles;
};
CPlayerGui* CPlayerGui::Instance = 0;
struct CJoystick {
    CJoystick() : Clears(0) {}
    void ClearAllRumbles() { ++Clears; }
    unsigned int Clears;
};
struct CInputManager {
    CJoystick& GetJoystick(long n) { CHECK(n == 2); return Joystick; }
    CJoystick Joystick;
};
CInputManager* PInputManager = 0;

struct CWorld {
    CWorld() : WaitingForInventoryTutorialToFinish(false), HeroInformationScreenModeAfterTutorial(PLAYER_MODE_NULL),
        PlayerLookups(0), Opens(0), OpenedMode(PLAYER_MODE_NULL) {}
    void EAOpenHeroInformationScreen(const CGameEvent* event);
    CPlayer* GetPlayer(unsigned long number) { CHECK(number == static_cast<unsigned long>(-1)); ++PlayerLookups; return &Player; }
    void OpenHeroInformationScreen(EPlayerMode mode) {
        CHECK(!WaitingForInventoryTutorialToFinish);
        if (mode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU)
            CHECK(PInputManager->Joystick.Clears == 1);
        ++Opens;
        OpenedMode = mode;
    }
    bool WaitingForInventoryTutorialToFinish;
    EPlayerMode HeroInformationScreenModeAfterTutorial;
    unsigned int PlayerLookups, Opens;
    EPlayerMode OpenedMode;
    CPlayer Player;
};

void CWorld::EAOpenHeroInformationScreen(const CGameEvent* event)
{
    {
        CPlayer* player = GetPlayer(event->GetPlayerNumber());
        if (player->HasMode(PLAYER_MODE_VIEW_HERO))
            return;
    }
    if (WaitingForInventoryTutorialToFinish)
        return;
    CThingPlayerCreature* pcreature = GetPlayer(event->GetPlayerNumber())->GetPControlledCreature();
    if (!IS_THING_ALIVE(pcreature) || !(pcreature->GetHealth() > 0.0f))
        return;
    if (CMainGameComponent::Get()->PeekDisplayEngine().IsRenderingNormalScene()) {
        const CTCQuickAccessMenu* ptc_menu;
        const CTCBoastUI* ptc_boast_ui;
        const CTCQuestCompletionUI* ptc_qc_ui;
        if (pcreature->PeekTC(&ptc_menu) && !ptc_menu->IsActive() &&
            pcreature->PeekTC(&ptc_boast_ui) && !ptc_boast_ui->IsActive() &&
            pcreature->PeekTC(&ptc_qc_ui) && !ptc_qc_ui->IsActive() &&
            !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            CGameEventDataPackage event_data;
            event->GetDataPackage(&event_data);
            CPlayer* player = GetPlayer(event->GetPlayerNumber());
            EPlayerMode player_mode = static_cast<EPlayerMode>(event_data.GetULONG());
            if (player_mode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU && CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen())
                return;
            {
                const CTCInGameMenu* ptc_in_game_menu;
                if (pcreature->PeekTC(&ptc_in_game_menu) && ptc_in_game_menu->IsOpen())
                    return;
            }
            if (player_mode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU) {
                PInputManager->GetJoystick(pcreature->GetJoystickDeviceNumber()).ClearAllRumbles();
            } else {
                const CTCInventory* ptc_inventory = 0;
                pcreature->PeekTC(&ptc_inventory);
                if (ptc_inventory && ptc_inventory->GetScreenToShortcut() == 4) {
                    CTCHero* ptc_hero = 0;
                    pcreature->GetTC(&ptc_hero);
                    if (ptc_hero && ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_WORLD_MAP, true)) {
                        WaitingForInventoryTutorialToFinish = true;
                        HeroInformationScreenModeAfterTutorial = player_mode;
                        return;
                    }
                } else {
                    CTCHero* ptc_hero = 0;
                    pcreature->GetTC(&ptc_hero);
                    if (ptc_hero && ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_INVENTORY, true)) {
                        WaitingForInventoryTutorialToFinish = true;
                        HeroInformationScreenModeAfterTutorial = player_mode;
                        return;
                    }
                }
            }
            OpenHeroInformationScreen(player_mode);
        }
    } else if (CPlayerGui::Get().IsLiveGUIMode()) {
        CPlayerGui::Get().ToggleLiveGui();
    }
}

struct Fixture {
    Fixture() {
        CMainGameComponent::Instance = &Component;
        CPlayerGui::Instance = &Gui;
        PInputManager = &Input;
        World.Player.Creature = &Creature;
    }
    void Run() { World.EAOpenHeroInformationScreen(&Event); }
    void CheckNoAction(unsigned int lookups, unsigned int payloads) const {
        CHECK(World.PlayerLookups == lookups && Event.DataRequests == payloads);
        CHECK(World.Opens == 0 && Creature.Hero.Calls == 0 && Input.Joystick.Clears == 0 && Gui.Toggles == 0);
    }
    CWorld World;
    CThingPlayerCreature Creature;
    CMainGameComponent Component;
    CPlayerGui Gui;
    CInputManager Input;
    CGameEvent Event;
};

static void CheckPreconditions()
{
    for (unsigned int reason = 0; reason < 13; ++reason) {
        Fixture f;
        switch (reason) {
        case 0: f.World.Player.ViewHero = true; break;
        case 1: f.World.WaitingForInventoryTutorialToFinish = true; break;
        case 2: f.World.Player.Creature = 0; break;
        case 3: f.Creature.Dead = true; break;
        case 4: f.Creature.Health = 0; break;
        case 5: f.Creature.Health = -1; break;
        case 6: f.Creature.Health = std::numeric_limits<float>::quiet_NaN(); break;
        case 7: f.Creature.HasMenu = false; break;
        case 8: f.Creature.Menu.Active = true; break;
        case 9: f.Creature.HasBoast = false; break;
        case 10: f.Creature.Boast.Active = true; break;
        case 11: f.Creature.HasQuest = false; break;
        case 12: f.Creature.Quest.Active = true; break;
        }
        f.Run();
        f.CheckNoAction(reason < 2 ? 1 : 2, 0);
    }
    {
        Fixture f;
        f.Gui.SubtitleRenderer.Displaying = true;
        f.Run();
        f.CheckNoAction(2, 0);
    }
    {
        Fixture f;
        f.Component.Engine.Rendering = false;
        f.Run();
        f.CheckNoAction(2, 0);
        f.Gui.LiveGui = true;
        f.Run();
        CHECK(f.Gui.Toggles == 1 && !f.Gui.LiveGui && !f.World.Opens && !f.Event.DataRequests);
    }
    {
        Fixture f;
        f.Creature.InGameMenu.Open = true;
        f.Run();
        f.CheckNoAction(3, 1);
    }
}

static void CheckInGameMenu()
{
    Fixture f;
    f.Event.Mode = PLAYER_MODE_NAVIGATE_IN_GAME_MENU;
    f.Gui.ScreenManager.Open = true;
    f.Run();
    f.CheckNoAction(3, 1);
    f.Gui.ScreenManager.Open = false;
    f.Creature.HasInGameMenu = false;
    f.Run();
    CHECK(f.World.Opens == 1 && f.World.OpenedMode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU);
    CHECK(f.World.PlayerLookups == 6 && f.Event.DataRequests == 2 && f.Input.Joystick.Clears == 1);
    CHECK(!f.Creature.Hero.Calls);
}

static void CheckTutorials()
{
    for (unsigned int mode = 0; mode < 4; ++mode) {
        for (unsigned int result = 0; result < 4; ++result) {
            Fixture f;
            f.Gui.ScreenManager.Open = true;
            f.Creature.Inventory.Shortcut = mode == 0 ? 4 : 1;
            f.Creature.HasInventory = mode != 2;
            if (mode == 3) f.Creature.InventoryPointer = 0;
            if (result == 2) f.Creature.HasHero = false;
            if (result == 3) f.Creature.HeroPointer = 0;
            f.Creature.Hero.GiveTutorial = result == 1;
            f.Run();
            CHECK(f.Creature.Hero.Calls == (result < 2 ? 1U : 0U));
            if (result < 2)
                CHECK(f.Creature.Hero.Category == (mode == 0 ? TUTORIAL_CATEGORY_WORLD_MAP : TUTORIAL_CATEGORY_INVENTORY));
            CHECK(f.World.WaitingForInventoryTutorialToFinish == (result == 1));
            CHECK(!f.Input.Joystick.Clears && !f.Gui.Toggles && f.World.PlayerLookups == 3 && f.Event.DataRequests == 1);
            if (result == 1) {
                CHECK(!f.World.Opens && f.World.HeroInformationScreenModeAfterTutorial == PLAYER_MODE_NAVIGATE_INVENTORY);
                f.Run();
                CHECK(f.World.PlayerLookups == 4 && f.Event.DataRequests == 1 && f.Creature.Hero.Calls == 1);
            } else {
                CHECK(f.World.Opens == 1 && f.World.OpenedMode == PLAYER_MODE_NAVIGATE_INVENTORY);
            }
        }
    }
}

int main()
{
    CheckPreconditions();
    CheckInGameMenu();
    CheckTutorials();
    puts("0062b070_TEST PASS");
    return 0;
}