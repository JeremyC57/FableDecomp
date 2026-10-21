#include <cstdio>
#include <cstdlib>
// Behaviour model of CQuestManager::CQuestManager (0x004B4590): every list starts empty (self-linked sentinel), every
// vector/map/set empty, the three ctor arguments land in ScriptInfoManager / World / GameInterface, CurrentQuestID 1,
// HeroMapNumber -1, flags false, and a fresh CGameScriptThingManager is owned by the counted pointer.
struct Node { Node* next; Node* prev; };
struct List {
    Node* node;
    List() : node(0) { Node* n = (Node*)std::malloc(sizeof(Node) + 8); n->next = n; n->prev = n; node = n; }
    ~List() { std::free(node); }
    bool empty() const { return node->next == node && node->prev == node; }
};
struct ThingManager { int created; ThingManager() : created(1) {} };
struct Counted {
    ThingManager* Data;
    int resets;
    Counted() : Data(0), resets(0) {}
    void Reset(ThingManager* p) { Data = p; ++resets; }
};
struct QuestManager {
    Counted PGameScriptThingManager;
    List ActiveQuests, PendingQuestsToDeactivate, AvailableQuestCardTypes, ActiveQuestCards, CompletedQuestCards;
    unsigned int FrameBoastsLastUpdated;
    List Boasts, AcceptedBoasts, TempBoasts, Objectives;
    void* ScriptInfoManager; void* World; void* GameInterface;
    int CurrentQuestID; void* PQuestCurrentlyRunning;
    bool DayActive, NightActive, WorldDestructed, QuestCardsNeedUpdating;
    int HeroMapNumber, LastAutoSaveFrame, NumberOfQuestsBetweenStartAndFinishScreens;
    QuestManager(void* world, void* scriptInfoManager, void* gameInterface)
        : FrameBoastsLastUpdated(0), ScriptInfoManager(scriptInfoManager), World(world), GameInterface(gameInterface),
          CurrentQuestID(1), PQuestCurrentlyRunning(0), DayActive(false), NightActive(false), WorldDestructed(false),
          QuestCardsNeedUpdating(false), HeroMapNumber(-1), LastAutoSaveFrame(0),
          NumberOfQuestsBetweenStartAndFinishScreens(0)
    {
        PGameScriptThingManager.Reset(new ThingManager);
    }
};
int main()
{
    int w = 0, s = 0, g = 0;
    QuestManager qm(&w, &s, &g);
    bool ok = qm.World == &w && qm.ScriptInfoManager == &s && qm.GameInterface == &g
        && qm.CurrentQuestID == 1 && qm.HeroMapNumber == -1 && qm.PQuestCurrentlyRunning == 0
        && !qm.DayActive && !qm.NightActive && !qm.WorldDestructed && !qm.QuestCardsNeedUpdating
        && qm.FrameBoastsLastUpdated == 0 && qm.LastAutoSaveFrame == 0 && qm.NumberOfQuestsBetweenStartAndFinishScreens == 0
        && qm.ActiveQuests.empty() && qm.PendingQuestsToDeactivate.empty() && qm.AvailableQuestCardTypes.empty()
        && qm.ActiveQuestCards.empty() && qm.CompletedQuestCards.empty() && qm.Boasts.empty() && qm.AcceptedBoasts.empty()
        && qm.TempBoasts.empty() && qm.Objectives.empty()
        && qm.PGameScriptThingManager.Data != 0 && qm.PGameScriptThingManager.Data->created == 1
        && qm.PGameScriptThingManager.resets == 1;
    delete qm.PGameScriptThingManager.Data;
    if (!ok) { std::printf("004b4590_TEST FAIL\n"); return 1; }
    std::printf("004b4590_TEST PASS\n");
    return 0;
}
