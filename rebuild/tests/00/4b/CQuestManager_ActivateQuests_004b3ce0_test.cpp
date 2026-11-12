// Behaviour test for CQuestManager::ActivateQuests 0x004B3CE0: the two-pass logic on a small stand-alone model
// (resource pass: queue + remember + code section; activation pass: scripted or bare active quest, list, set, messages).
#include <cstdio>
#include <cstring>

enum { MAXN = 8 };
struct CNameSet {
    const char* names[MAXN]; int count;
    CNameSet() : count(0) {}
    bool contains(const char* n) const { for (int i = 0; i < count; ++i) if (std::strcmp(names[i], n) == 0) return true; return false; }
    void insert(const char* n) { if (!contains(n)) names[count++] = n; }
    void erase(const char* n) { for (int i = 0; i < count; ++i) if (std::strcmp(names[i], n) == 0) { names[i] = names[--count]; return; } }
};
class CScriptInfo { public: bool MasterScript; };
struct CActivateQuestInfo { const char* QuestName; const CScriptInfo* PScriptInfo; bool GameScript; bool LoadResources; };
struct CActiveQuest { long ID; const char* QuestName; bool HasScript; bool HasOwnMasterData; bool LoadedResources; const char* RestoredText; };
struct CPersist { const char* QuestName; const char* Text; };

class CQuestManager { public:
    CNameSet AlreadyActive;                 // IsQuestActive answers from here plus ActiveQuests
    CNameSet QuestsWithResourcesLoadedThatAreNotActivated;
    CNameSet RegisteredSections;            // code sections registered under a long name
    CNameSet LoadedSections;
    const char* ResourcesLoadedFor[MAXN]; int ResourceCalls, ResourceCount;
    CPersist Persist[MAXN]; int PersistCount;
    CActiveQuest ActiveQuests[MAXN]; int ActiveCount;
    long CurrentQuestID;
    bool ScriptsEnabled;
    int Activations, Messages, MasterLoads, MasterLookups;
    long LastMessageIndex;

    CQuestManager() : ResourceCalls(0), ResourceCount(0), PersistCount(0), ActiveCount(0), CurrentQuestID(0), ScriptsEnabled(true),
                      Activations(0), Messages(0), MasterLoads(0), MasterLookups(0), LastMessageIndex(-1) {}
    bool IsQuestActive(const char* n) const
    {
        if (AlreadyActive.contains(n)) return true;
        for (int i = 0; i < ActiveCount; ++i) if (std::strcmp(ActiveQuests[i].QuestName, n) == 0) return true;
        return false;
    }
    const char* GetQuestScriptPersistScript(const char* n) const
    {
        for (int i = 0; i < PersistCount; ++i) if (std::strcmp(Persist[i].QuestName, n) == 0) return Persist[i].Text;
        return 0;
    }
    long AllocateQuestID() { return CurrentQuestID++; }
    long GetIndexFromRegisteredQuest(const char* n) const { return (long)std::strlen(n); }
    void LoadResourcesForQuests(const char** names, int count)
    {
        ++ResourceCalls;
        for (int i = 0; i < count; ++i) ResourcesLoadedFor[ResourceCount++] = names[i];
    }
    // The logic of 0x004B3CE0.
    void ActivateQuests(const CActivateQuestInfo* quests, unsigned int size)
    {
        const char* quests_to_load_resources_for[MAXN]; int to_load = 0;
        unsigned int i = 0;
        while (i < size) {
            const CActivateQuestInfo& info = quests[i];
            if (!IsQuestActive(info.QuestName) && !QuestsWithResourcesLoadedThatAreNotActivated.contains(info.QuestName)) {
                if (info.LoadResources) {
                    quests_to_load_resources_for[to_load++] = info.QuestName;
                    QuestsWithResourcesLoadedThatAreNotActivated.insert(info.QuestName);
                }
                if (RegisteredSections.contains(info.QuestName))
                    LoadedSections.insert(info.QuestName);
            }
            ++i;
        }
        if (to_load != 0)
            LoadResourcesForQuests(quests_to_load_resources_for, to_load);
        unsigned int j = 0;
        while (j < size) {
            const CActivateQuestInfo& info = quests[j];
            if (!IsQuestActive(info.QuestName)) {
                const char* persist_script = GetQuestScriptPersistScript(info.QuestName);
                CActiveQuest quest;
                if (info.PScriptInfo && ScriptsEnabled) {
                    quest.ID = AllocateQuestID();
                    quest.HasOwnMasterData = false;
                    if (info.PScriptInfo->MasterScript) { quest.HasOwnMasterData = true; ++MasterLoads; }
                    else ++MasterLookups;
                    quest.QuestName = info.QuestName; quest.HasScript = true; quest.LoadedResources = info.LoadResources;
                    quest.RestoredText = 0;
                    QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
                    ++Activations;
                    if (persist_script) quest.RestoredText = persist_script;        // the quest has a script: text restored
                    ActiveQuests[ActiveCount++] = quest;
                    ++Messages; LastMessageIndex = GetIndexFromRegisteredQuest(info.QuestName);
                }
                else {
                    quest.ID = AllocateQuestID();
                    quest.QuestName = info.QuestName; quest.HasScript = false; quest.HasOwnMasterData = false;
                    quest.LoadedResources = info.LoadResources; quest.RestoredText = 0;   // no script: LoadFromGameState does nothing
                    ActiveQuests[ActiveCount++] = quest;
                    QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
                }
            }
            ++j;
        }
    }
};

static int Fail(const char*) { std::printf("004b3ce0_TEST FAIL\n"); return 1; }

int main()
{
    static const CScriptInfo plain = { false }, master = { true };
    CQuestManager m;
    m.AlreadyActive.insert("Q_Active");
    m.QuestsWithResourcesLoadedThatAreNotActivated.insert("Q_Preloaded");
    m.RegisteredSections.insert("Q_Script");
    m.RegisteredSections.insert("Q_Active");          // active quests are skipped in the resource pass
    m.Persist[m.PersistCount].QuestName = "Q_Script"; m.Persist[m.PersistCount++].Text = "script text";
    m.Persist[m.PersistCount].QuestName = "Q_Bare";   m.Persist[m.PersistCount++].Text = "bare text";
    const CActivateQuestInfo quests[5] = {
        { "Q_Script",    &plain,  true, true  },      // scripted, resources wanted, section registered, persisted text
        { "Q_Bare",      0,       true, false },      // no script info: bare record, persisted text ignored (no script)
        { "Q_Active",    &plain,  true, true  },      // already active: untouched by both passes
        { "Q_Preloaded", &master, true, true  },      // resources already loaded: not queued again; master script
        { "Q_Master",    &master, true, false },      // master script, no resources
    };
    m.ActivateQuests(quests, 5);
    if (m.ResourceCalls != 1 || m.ResourceCount != 1 || std::strcmp(m.ResourcesLoadedFor[0], "Q_Script") != 0) return Fail("resources");
    if (m.LoadedSections.count != 1 || !m.LoadedSections.contains("Q_Script")) return Fail("sections");
    if (m.ActiveCount != 4 || m.CurrentQuestID != 4) return Fail("count");
    if (m.QuestsWithResourcesLoadedThatAreNotActivated.count != 0) return Fail("loaded set");
    if (m.ActiveQuests[0].ID != 0 || !m.ActiveQuests[0].HasScript || m.ActiveQuests[0].HasOwnMasterData || !m.ActiveQuests[0].LoadedResources ||
        !m.ActiveQuests[0].RestoredText || std::strcmp(m.ActiveQuests[0].RestoredText, "script text") != 0) return Fail("scripted quest");
    if (m.ActiveQuests[1].ID != 1 || m.ActiveQuests[1].HasScript || m.ActiveQuests[1].RestoredText || m.ActiveQuests[1].LoadedResources) return Fail("bare quest");
    if (m.ActiveQuests[2].ID != 2 || std::strcmp(m.ActiveQuests[2].QuestName, "Q_Preloaded") != 0 || !m.ActiveQuests[2].HasOwnMasterData) return Fail("master quest");
    if (m.ActiveQuests[3].ID != 3 || !m.ActiveQuests[3].HasOwnMasterData) return Fail("second master quest");
    if (m.Activations != 3 || m.Messages != 3 || m.MasterLoads != 2 || m.MasterLookups != 1 || m.LastMessageIndex != 8) return Fail("script calls");
    // Scripts disabled: every record becomes a bare quest, no activation, no message; a second call activates nothing new.
    CQuestManager n;
    n.ScriptsEnabled = false;
    n.ActivateQuests(quests, 2);
    if (n.ActiveCount != 2 || n.Activations != 0 || n.Messages != 0 || n.ActiveQuests[0].HasScript || n.ResourceCalls != 1) return Fail("scripts disabled");
    n.ActivateQuests(quests, 2);
    if (n.ActiveCount != 2 || n.CurrentQuestID != 2 || n.ResourceCalls != 1) return Fail("repeat call");
    // An empty batch does nothing.
    CQuestManager e;
    e.ActivateQuests(quests, 0);
    if (e.ActiveCount != 0 || e.ResourceCalls != 0 || e.CurrentQuestID != 0) return Fail("empty");
    std::printf("004b3ce0_TEST PASS\n");
    return 0;
}