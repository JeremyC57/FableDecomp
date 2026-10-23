// 005252b0 behaviour test: CThingManager::SaveThingsOnLevelReadyForUnloading (final.cpp body over standard containers)
#include <stdio.h>
#include <list>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CThing {
    long CurrentMapNumber;
    bool KillOnLevelUnload, GloballyLoaded, PersistOnUnloadWhenInLimbo, InLimbo, DeletedOnOwnerLevelOutOfScope, Dead;
    bool GameState, LevelPersistent;
    int Prepared;
    void PrepareForLimbo() { ++Prepared; }
    void SetInLimbo(bool b) { InLimbo = b; }
    bool IsGameStatePersisted() { return GameState; }
    bool IsLevelPersistent() { return LevelPersistent; }
};
static std::vector<CThing*> g_deactivated;
struct CQuestManager { void OnDeactivateEntity(CThing* t, bool) { g_deactivated.push_back(t); } };
static CQuestManager g_quests; static CQuestManager* g_pQuestManager = &g_quests;
typedef std::vector<CThing*> CThingVector;
struct CThingManager {
    std::list<CThing*> Things;
    std::vector<CThing*> SavedGame, SavedLevel; long SavedMap;
    void SavePersistentState(long mapNumber, CThingVector& things, bool gameState)
    {
        SavedMap = mapNumber;
        for (size_t i = 0; i < things.size(); ++i) (gameState ? SavedGame : SavedLevel).push_back(things[i]);
    }
    void SaveThingsOnLevelReadyForUnloading(long mapNumber, bool savePersistent, CThingVector* toDelete);
};
void CThingManager::SaveThingsOnLevelReadyForUnloading(long mapNumber, bool savePersistent, CThingVector* toDelete)
{
    CThingVector gameStateThings;
    CThingVector levelThings;
    for (std::list<CThing*>::iterator node = Things.begin(); node != Things.end(); ++node) {
        CThing* thing = *node;
        if (thing->CurrentMapNumber != mapNumber)
            continue;
        if (!thing->KillOnLevelUnload) {
            thing->PrepareForLimbo();
            thing->SetInLimbo(true);
            g_pQuestManager->OnDeactivateEntity(thing, true);
        } else if (!thing->GloballyLoaded && (thing->PersistOnUnloadWhenInLimbo || !thing->InLimbo)
                   && thing->DeletedOnOwnerLevelOutOfScope) {
            toDelete->push_back(thing);
            if (savePersistent && !thing->Dead) {
                if (thing->IsGameStatePersisted())
                    gameStateThings.push_back(thing);
                else if (thing->IsLevelPersistent())
                    levelThings.push_back(thing);
            }
        }
    }
    SavePersistentState(mapNumber, gameStateThings, true);
    SavePersistentState(mapNumber, levelThings, false);
}
static CThing make(long map, bool kill, bool gs, bool lp)
{
    CThing t = { map, kill, false, false, false, true, false, gs, lp, 0 };
    return t;
}
int main()
{
    CThing a = make(3, false, false, false), b = make(3, true, true, false), c = make(3, true, false, true),
           d = make(4, true, true, false), e = make(3, true, false, false), f = make(3, true, true, false);
    f.Dead = true;
    CThing g = make(3, true, true, false); g.GloballyLoaded = true;
    CThingManager m; m.SavedMap = -1;
    m.Things.push_back(&a); m.Things.push_back(&b); m.Things.push_back(&c); m.Things.push_back(&d);
    m.Things.push_back(&e); m.Things.push_back(&f); m.Things.push_back(&g);
    CThingVector del;
    m.SaveThingsOnLevelReadyForUnloading(3, true, &del);
    CHECK(a.Prepared == 1 && a.InLimbo && g_deactivated.size() == 1 && g_deactivated[0] == &a);
    CHECK(del.size() == 4 && del[0] == &b && del[1] == &c && del[2] == &e && del[3] == &f);
    CHECK(m.SavedGame.size() == 1 && m.SavedGame[0] == &b);
    CHECK(m.SavedLevel.size() == 1 && m.SavedLevel[0] == &c && m.SavedMap == 3);
    CThingVector del2; m.SavedGame.clear(); m.SavedLevel.clear();
    m.SaveThingsOnLevelReadyForUnloading(4, false, &del2);
    CHECK(del2.size() == 1 && del2[0] == &d && m.SavedGame.empty());
    if (!g_fail) printf("005252b0_TEST PASS\n");
    return g_fail;
}