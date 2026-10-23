#include <cstdio>
#include <list>
#include <vector>
#include <string>
#include <set>
// Behaviour test for CQuestManager::~CQuestManager (0x004B4770): unless the world was already destructed, all quests are
// deactivated, the boast and accepted-boast lists are cleared and the boast frame stamped; the counted
// GameScriptThingManager is released (destroyed on its last reference) whether or not the world was destructed.
static int g_Deactivated;
static int g_ManagerDestroyed;
static int g_Frame = 77;
static int g_LastStamp;
struct CThingManager { ~CThingManager() { ++g_ManagerDestroyed; } };
struct CCountBlock { int Refs; CThingManager* Object; };
struct CQuestManagerModel {
    CThingManager* Data; CCountBlock* Info;
    std::vector<std::string> RegisteredQuestNames;
    std::list<int> ActiveQuests, Boasts, AcceptedBoasts, TempBoasts;
    unsigned FrameBoastsLastUpdated;
    bool WorldDestructed;
    std::set<std::string> QuestsWithResourcesLoaded;
    CQuestManagerModel() : Data(0), Info(0), FrameBoastsLastUpdated(0), WorldDestructed(false) {}
    void DeactivateAllQuests() { ++g_Deactivated; ActiveQuests.clear(); }
    ~CQuestManagerModel()
    {
        if (!WorldDestructed) {
            DeactivateAllQuests();
            Boasts.clear();
            AcceptedBoasts.clear();
            FrameBoastsLastUpdated = g_Frame;
            g_LastStamp = (int)FrameBoastsLastUpdated;
        }
        if (Info) {
            --Info->Refs;
            if (Info->Refs == 0) { delete Info->Object; delete Info; }
        }
        Data = 0;
        Info = 0;
    }
};
static void Make(bool destructed, int refs, CCountBlock** keep)
{
    CQuestManagerModel* m = new CQuestManagerModel;
    m->WorldDestructed = destructed;
    m->Data = new CThingManager;
    m->Info = new CCountBlock; m->Info->Refs = refs; m->Info->Object = m->Data;
    *keep = m->Info;
    m->RegisteredQuestNames.push_back("Q_Arena");
    m->Boasts.push_back(1); m->AcceptedBoasts.push_back(2); m->ActiveQuests.push_back(3);
    m->QuestsWithResourcesLoaded.insert("Q_Lychfield");
    delete m;
}
int main()
{
    CCountBlock* keep = 0;
    Make(false, 1, &keep);
    bool ok = g_Deactivated == 1 && g_LastStamp == 77 && g_ManagerDestroyed == 1;
    g_Frame = 90;
    Make(true, 2, &keep);
    ok = ok && g_Deactivated == 1 && g_LastStamp == 77 && g_ManagerDestroyed == 1 && keep->Refs == 1;
    delete keep->Object; delete keep;
    if (!ok) { std::printf("004b4770_TEST FAIL %d %d %d\n", g_Deactivated, g_LastStamp, g_ManagerDestroyed); return 1; }
    std::printf("004b4770_TEST PASS\n");
    return 0;
}
