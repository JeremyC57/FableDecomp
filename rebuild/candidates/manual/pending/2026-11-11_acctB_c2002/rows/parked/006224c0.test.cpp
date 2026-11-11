// Behaviour test for 0x006224C0 CTCInventoryQuests::OpenInventoryPC (parked row): a host copy of the row's logic over plain models --
// the hero-info screen switch, the selected-category clamp, the live quest cards of the building's hero centre and the two
// components kept -- checked as a standalone screen and as a hero-info sub-screen.
#include <cstdio>
#include <cstring>

struct Card { bool Present; bool Dead; int Id; };
struct Screen {
    bool InventoryScreen;                     // IsInventoryScreen()
    int CurrentScreenType, ThisScreenType;    // the hero-info manager's current screen / ours
    int QuestCardCount, SelectedCategory;
    bool GuildValidity;
    bool BuildingAlive; bool HasBuilding;
    Card Cards[6]; int CardCount;
    // effects
    int PreviousScreenType; bool ActiveTestQuestMode; bool IsGuildValid; float AccumFadeTime;
    bool SoundsFaded, MenusOpened, InventoryOpened, ManagerSwitched; int SelectedScreenType;
    int Quests[6]; int QuestCount; const char* Components[2]; int ComponentCount; bool ListBuilt;
};

// The row's logic (same order of tests and effects as 0x006224C0).
static void OpenInventoryPC(Screen& s)
{
    s.AccumFadeTime = 0.0f;
    s.PreviousScreenType = -1;
    if (!s.InventoryScreen) {
        s.SoundsFaded = true;
        s.MenusOpened = true;
        s.PreviousScreenType = s.CurrentScreenType;
        s.ActiveTestQuestMode = false;
    } else {
        s.ManagerSwitched = true; s.SelectedScreenType = s.ThisScreenType;
        s.ActiveTestQuestMode = true;
    }
    s.IsGuildValid = s.GuildValidity;
    if (s.QuestCardCount > 0) {
        int last = s.QuestCardCount - 1;
        if (s.SelectedCategory > last) s.SelectedCategory = last;
    }
    s.InventoryOpened = true;
    if (s.InventoryScreen) return;
    s.QuestCount = 0;
    if (s.HasBuilding && s.BuildingAlive)
        for (int i = 0; i < s.CardCount; ++i)
            if (s.Cards[i].Present && !s.Cards[i].Dead) s.Quests[s.QuestCount++] = s.Cards[i].Id;
    s.ListBuilt = true;
    s.Components[s.ComponentCount++] = "PC_QUESTS_SELECTION_MENU";
    s.Components[s.ComponentCount++] = "UI_MOUSE_POINTER";
}

static Screen Make()
{
    Screen s; std::memset(&s, 0, sizeof(s));
    s.CurrentScreenType = 8; s.ThisScreenType = 1; s.QuestCardCount = 3; s.SelectedCategory = 7; s.GuildValidity = true;
    s.HasBuilding = s.BuildingAlive = true;
    Card c0 = { true, false, 10 }, c1 = { true, true, 11 }, c2 = { false, false, 12 }, c3 = { true, false, 13 };
    s.Cards[0] = c0; s.Cards[1] = c1; s.Cards[2] = c2; s.Cards[3] = c3; s.CardCount = 4;
    return s;
}

int main()
{
    int fails = 0;
    Screen s = Make();
    OpenInventoryPC(s);
    if (s.PreviousScreenType != 8 || s.ActiveTestQuestMode || !s.SoundsFaded || !s.MenusOpened || s.ManagerSwitched) ++fails;
    if (s.SelectedCategory != 2 || !s.IsGuildValid || !s.InventoryOpened) ++fails;
    if (s.QuestCount != 2 || s.Quests[0] != 10 || s.Quests[1] != 13 || !s.ListBuilt) ++fails;
    if (s.ComponentCount != 2 || std::strcmp(s.Components[0], "PC_QUESTS_SELECTION_MENU") || std::strcmp(s.Components[1], "UI_MOUSE_POINTER")) ++fails;
    // a sub-screen of the hero-information screens: switch the manager, open the base inventory, build nothing
    s = Make(); s.InventoryScreen = true; s.SelectedCategory = 1;
    OpenInventoryPC(s);
    if (s.PreviousScreenType != -1 || !s.ActiveTestQuestMode || !s.ManagerSwitched || s.SelectedScreenType != 1 || s.SoundsFaded) ++fails;
    if (s.SelectedCategory != 1 || !s.InventoryOpened || s.ComponentCount != 0 || s.ListBuilt) ++fails;
    // a dead building gives an empty quest list
    s = Make(); s.BuildingAlive = false;
    OpenInventoryPC(s);
    if (s.QuestCount != 0 || !s.ListBuilt) ++fails;
    if (fails) { std::printf("006224c0_TEST FAIL\n"); return 1; }
    std::printf("006224c0_TEST PASS\n");
    return 0;
}
