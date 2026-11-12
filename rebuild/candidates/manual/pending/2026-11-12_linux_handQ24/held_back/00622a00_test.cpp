// Behaviour test for CTCInventoryQuests::OpenInventory (0x00622A00): a copy of the row's logic over a small model of the
// UI manager, hero-info screens, the inventory base and the hero centre's quest cards.
#include <cstdio>
#include <vector>

struct Thing { bool dead; int id; };

struct Log {
    int fades, menuOpens, questInventorySet, renderingOff, baseOpened, initialised, stateSet, cleared;
    Thing* playerThing;
    int selectedScreen;
};

struct Inventory {
    Log log;
    bool inventoryScreen;
    bool guildValid;
    bool isGuildValid;
    bool activeTestQuestMode;
    long questCardCount;
    long selectedIndex;
    long previousScreenType;
    int currentScreenType;
    int screenType;
    Thing* building;
    Thing* parent;
    Thing* hero;
    std::vector<Thing*> cards;           // the hero centre's quest cards (null = an expired pointer)
    std::vector<Thing*> built;           // what ConstructQuestList received
    std::vector<int> components;

    void OpenInventory()
    {
        log.playerThing = hero;
        previousScreenType = -1;
        if (!inventoryScreen) {
            log.fades += 3;
            log.playerThing = parent;
            log.renderingOff = 1;
            ++log.menuOpens;
            previousScreenType = currentScreenType;
            activeTestQuestMode = false;
        } else {
            log.selectedScreen = screenType;
            activeTestQuestMode = true;
        }
        isGuildValid = guildValid;
        ++log.initialised;
        if (questCardCount > 0) {
            long last = questCardCount - 1;
            if (selectedIndex > last)
                selectedIndex = last;
        }
        ++log.baseOpened;
        if (inventoryScreen)
            return;
        log.questInventorySet = 1;
        std::vector<Thing*> quests;
        if (building && !building->dead) {
            for (std::vector<Thing*>::iterator it = cards.begin(); it != cards.end(); ++it)
                if (*it && !(*it)->dead)
                    quests.push_back(*it);
        }
        ++log.cleared;
        built = quests;
        log.stateSet = 5;
        components.push_back(1);
    }
};

static int fail() { std::printf("00622a00_TEST FAIL\n"); return 1; }

int main()
{
    Thing hero = { false, 1 }, parent = { false, 2 }, building = { false, 3 };
    Thing q1 = { false, 10 }, q2 = { true, 11 }, q3 = { false, 12 };
    Inventory inv;
    Log zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
    inv.log = zero;
    inv.inventoryScreen = false;
    inv.guildValid = true;
    inv.isGuildValid = false;
    inv.activeTestQuestMode = true;
    inv.questCardCount = 3;
    inv.selectedIndex = 7;
    inv.previousScreenType = 0;
    inv.currentScreenType = 8;
    inv.screenType = 1;
    inv.building = &building;
    inv.parent = &parent;
    inv.hero = &hero;
    inv.cards.push_back(&q1);
    inv.cards.push_back(0);
    inv.cards.push_back(&q2);
    inv.cards.push_back(&q3);
    inv.OpenInventory();
    if (inv.previousScreenType != 8 || inv.activeTestQuestMode || !inv.isGuildValid || inv.selectedIndex != 2) return fail();
    if (inv.log.fades != 3 || inv.log.playerThing != &parent || inv.log.menuOpens != 1 || !inv.log.questInventorySet) return fail();
    if (inv.built.size() != 2 || inv.built[0] != &q1 || inv.built[1] != &q3 || inv.components.size() != 1) return fail();

    // already the inventory screen: select this screen, stop after the base open
    inv.log = zero;
    inv.inventoryScreen = true;
    inv.components.clear();
    inv.OpenInventory();
    if (inv.previousScreenType != -1 || !inv.activeTestQuestMode || inv.log.selectedScreen != 1 || inv.log.baseOpened != 1) return fail();
    if (inv.log.playerThing != &hero || inv.log.questInventorySet || !inv.components.empty()) return fail();

    // dead building: an empty quest list
    inv.log = zero;
    inv.inventoryScreen = false;
    building.dead = true;
    inv.OpenInventory();
    if (!inv.built.empty() || inv.components.size() != 1) return fail();

    std::printf("00622a00_TEST PASS\n");
    return 0;
}
