// 004b3a20 behaviour test: CQuestManager::DeactivateQuest (match.cpp body)
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CCharString { public:
    std::string S;
    CCharString(const char* t) : S(t) {}
    bool operator==(const CCharString& o) const { return S == o.S; }
    bool operator<(const CCharString& o) const { return S < o.S; }
};
CCharString operator+(const CCharString& l, const char* r) { return CCharString((l.S + r).c_str()); }
CCharString operator+(const CCharString& l, const CCharString& r) { return CCharString((l.S + r.S).c_str()); }
static CCharString g_DayOnlySuffix("DayOnly"), g_NightOnlySuffix("NightOnly");
static std::vector<std::string> g_log;
class CScriptBase { public: int Shutdowns; void* RunningAtShutdown; void Shutdown(); };
struct CActiveQuest { long ID; CScriptBase* PScript; bool LoadedResources; CCharString QuestName; CActiveQuest(const char* n) : QuestName(n) {} };
class CQuestManager;
static CQuestManager* g_qm;
template <class T> class CCountedPointer { public:      // modelled as a shared count
    T* Data; int* Count;
    CCountedPointer(T* d) : Data(d), Count(new int(1)) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Count(o.Count) { if (Count) ++*Count; }
    ~CCountedPointer() { Release(); }
    void Release() { if (Count && --*Count == 0) { delete Count; } Count = 0; }
    void Reset() { Release(); Data = 0; Count = 0; }
};
typedef std::set<CCharString> CSectionSet;
class CThingManager { public: void DeleteThingsWithSectionName(const CCharString& n) { g_log.push_back("delete:" + n.S); } };
class CWorldMap { public: void UpdateNavMaps(const CSectionSet& s, bool) { std::string t = "nav:"; for (CSectionSet::const_iterator i = s.begin(); i != s.end(); ++i) t += i->S + ","; g_log.push_back(t); } };
class CWorld { public: CThingManager TM; CWorldMap WM; CThingManager* GetThingManager() { return &TM; } CWorldMap* GetWorldMap() { return &WM; } };
struct CDrawQuestInfo { void RemoveElementsFromScript(long id) { char b[32]; sprintf(b, "gui:%ld", id); g_log.push_back(b); } };
class CPlayerGui { public: CDrawQuestInfo* PDrawQuestInfo; };
static CDrawQuestInfo g_dqi; static CPlayerGui g_gui = { &g_dqi }; static CPlayerGui* g_pPlayerGui = &g_gui;
class CCodeSectionManager { public:
    std::set<std::string> Loaded;
    bool Contains(const CCharString& n) { return Loaded.count(n.S) != 0; }
    long GetSectionHandle(const CCharString& n) { return (long)n.S.size(); }
    void UnloadSection(long h) { char b[32]; sprintf(b, "unload:%ld", h); g_log.push_back(b); }
};
static CCodeSectionManager g_csm;
CCodeSectionManager* GetCodeSectionManager() { return &g_csm; }
class CQuestManager { public:
    std::list<CCountedPointer<CActiveQuest> > ActiveQuests;
    std::vector<CCharString> BetweenStartAndEndScreenQuests;
    CWorld* World;
    CActiveQuest* PQuestCurrentlyRunning;
    bool DayActive, NightActive;
    long NumberOfQuestsBetweenStartAndFinishScreens;
    std::set<std::string> StillActive;
    void ClearQuestPersistScript(const CCharString& n) { g_log.push_back("clear:" + n.S); }
    void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString& n, bool b) { g_log.push_back("card:" + n.S + (b ? ":1" : ":0")); }
    bool IsQuestActive(const CCharString& n) { return StillActive.count(n.S) != 0; }
    void DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason);
};
void CScriptBase::Shutdown() { ++Shutdowns; RunningAtShutdown = g_qm->PQuestCurrentlyRunning; }
void CQuestManager::DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason)
{
    CActiveQuest* activeQuest = quest.Data;
    CCharString questName = activeQuest->QuestName;
    bool loadedResources = activeQuest->LoadedResources;
    g_pPlayerGui->PDrawQuestInfo->RemoveElementsFromScript(activeQuest->ID);
    ClearQuestPersistScript(questName);
    if (activeQuest->PScript) {
        CActiveQuest* previous = PQuestCurrentlyRunning;
        PQuestCurrentlyRunning = activeQuest;
        activeQuest->PScript->Shutdown();
        PQuestCurrentlyRunning = previous;
    }
    CCharString sectionName = activeQuest->QuestName;
    std::vector<CCharString>::iterator last = BetweenStartAndEndScreenQuests.end();
    std::vector<CCharString>::iterator it = std::find(BetweenStartAndEndScreenQuests.begin(), last, questName);
    if (it != last) {
        if (NumberOfQuestsBetweenStartAndFinishScreens > 0)
            --NumberOfQuestsBetweenStartAndFinishScreens;
        BetweenStartAndEndScreenQuests.erase(it);
    }
    for (std::list<CCountedPointer<CActiveQuest> >::iterator i = ActiveQuests.begin(); i != ActiveQuests.end();)
        if (i->Data == quest.Data) i = ActiveQuests.erase(i); else ++i;
    quest.Reset();
    bool makeAvailable = true;
    if (reason & 2)
        makeAvailable = false;
    RemoveActiveQuestCardAndMakeAvailableAgain(questName, makeAvailable);
    if (!IsQuestActive(questName) && !(reason & 1)) {
        World->GetThingManager()->DeleteThingsWithSectionName(sectionName);
        if (DayActive)
            World->GetThingManager()->DeleteThingsWithSectionName(sectionName + "%" + g_DayOnlySuffix);
        if (NightActive)
            World->GetThingManager()->DeleteThingsWithSectionName(sectionName + "%" + g_NightOnlySuffix);
        if (loadedResources) {
            CSectionSet sections;
            for (std::list<CCountedPointer<CActiveQuest> >::iterator node = ActiveQuests.begin(); node != ActiveQuests.end(); ++node)
                sections.insert(node->Data->QuestName);
            World->GetWorldMap()->UpdateNavMaps(sections, false);
        }
    }
    CCodeSectionManager* manager = GetCodeSectionManager();
    if (manager->Contains(questName)) {
        CCharString name = questName;
        manager->UnloadSection(manager->GetSectionHandle(name));
    }
}
int main()
{
    CWorld world; CQuestManager qm; g_qm = &qm; qm.World = &world; qm.PQuestCurrentlyRunning = 0; qm.DayActive = true; qm.NightActive = false;
    CActiveQuest a("QA"), b("QB"); a.ID = 5; b.ID = 6; a.LoadedResources = true; b.LoadedResources = false;
    CScriptBase script = { 0, 0 }; a.PScript = &script; b.PScript = 0;
    CCountedPointer<CActiveQuest> pa(&a), pb(&b);
    qm.ActiveQuests.push_back(pa); qm.ActiveQuests.push_back(pb);
    qm.BetweenStartAndEndScreenQuests.push_back(CCharString("QZ")); qm.BetweenStartAndEndScreenQuests.push_back(CCharString("QA"));
    qm.NumberOfQuestsBetweenStartAndFinishScreens = 2;
    g_csm.Loaded.insert("QA");
    qm.DeactivateQuest(pa, 0);
    CHECK(script.Shutdowns == 1 && script.RunningAtShutdown == &a && qm.PQuestCurrentlyRunning == 0);
    CHECK(qm.BetweenStartAndEndScreenQuests.size() == 1 && qm.BetweenStartAndEndScreenQuests[0].S == "QZ");
    CHECK(qm.NumberOfQuestsBetweenStartAndFinishScreens == 1);
    CHECK(qm.ActiveQuests.size() == 1 && qm.ActiveQuests.front().Data == &b);
    CHECK(*pa.Count == 1);                       // only the caller's reference remains
    CHECK(g_log.size() == 7);
    CHECK(g_log[0] == "gui:5" && g_log[1] == "clear:QA" && g_log[2] == "card:QA:1");
    CHECK(g_log[3] == "delete:QA" && g_log[4] == "delete:QA%DayOnly" && g_log[5] == "nav:QB," && g_log[6] == "unload:2");
    // reason bit 1: keep things; bit 2: not made available; quest still active elsewhere suppresses deletion as well
    g_log.clear(); qm.StillActive.insert("QB");
    qm.DeactivateQuest(pb, 2);
    CHECK(g_log.size() == 3 && g_log[2] == "card:QB:0");
    CHECK(qm.ActiveQuests.empty());
    CActiveQuest c("QC"); c.ID = 7; c.PScript = 0; c.LoadedResources = true; CCountedPointer<CActiveQuest> pc(&c); qm.ActiveQuests.push_back(pc);
    g_log.clear(); qm.DeactivateQuest(pc, 1);
    CHECK(g_log.size() == 3 && g_log[2] == "card:QC:1");
    CHECK(qm.NumberOfQuestsBetweenStartAndFinishScreens == 1);   // QC was not in the screen list
    if (!g_fail) printf("004b3a20_TEST PASS\n");
    return g_fail;
}