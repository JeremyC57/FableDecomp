// Behaviour test for CQuestManager::ActivateMultipleQuests 0x004B4260: a copy of the model with recording callees.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* text) : Text(text) { }
};
struct CScriptInfo { int Tag; };
struct CActivateQuestInfo {
    CCharString QuestName;
    CScriptInfo* PScriptInfo;
    bool GameScript;
    bool LoadResources;
    CActivateQuestInfo(const CCharString& quest_name, const CScriptInfo* script_info, bool game_script, bool load_resources)
        : QuestName(quest_name), PScriptInfo((CScriptInfo*)script_info), GameScript(game_script), LoadResources(load_resources) { }
};
struct CNameArray {
    const CCharString* Start;
    unsigned int Count;
    unsigned int size() const { return Count; }
    const CCharString& operator[](unsigned int i) const { return Start[i]; }
};
struct CInfoArray {
    const char* Names[8];
    CScriptInfo* Infos[8];
    bool GameScript[8];
    bool LoadResources[8];
    unsigned int Count;
    CInfoArray() : Count(0) { }
    void push_back(const CActivateQuestInfo& x)
    {
        Names[Count] = x.QuestName.Text;
        Infos[Count] = x.PScriptInfo;
        GameScript[Count] = x.GameScript;
        LoadResources[Count] = x.LoadResources;
        ++Count;
    }
};
static int g_progress_calls;
static bool g_progress_ok = true;
static void DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2)
{
    ++g_progress_calls;
    if (std::strcmp(text.Text, "QuestManager: Activate Quest") != 0 || progress != -1.0f || flag1 || flag2)
        g_progress_ok = false;
}
static CScriptInfo g_info_a = { 1 };
static CScriptInfo g_info_c = { 3 };
struct CScriptInfoManager {
    CScriptInfo* GetPScriptInfo(const CCharString& name)
    {
        if (std::strcmp(name.Text, "QuestA") == 0) return &g_info_a;
        if (std::strcmp(name.Text, "QuestC") == 0) return &g_info_c;
        return 0;                                   // registered quests without a script
    }
};
struct CQuestManager {
    CScriptInfoManager* ScriptInfoManager;
    int ActivateQuestsCalls;
    CInfoArray Activated;
    bool IsQuestRegistered(const CCharString& name) const
    {
        return std::strcmp(name.Text, "QuestA") == 0 || std::strcmp(name.Text, "QuestB") == 0 || std::strcmp(name.Text, "QuestC") == 0;
    }
    void ActivateQuests(const CInfoArray& quests) { ++ActivateQuestsCalls; Activated = quests; }
    bool ActivateMultipleQuests(const CNameArray& quest_names, bool game_script, bool load_resources)
    {
        CInfoArray quests;
        bool activated = false;
        for (unsigned int i = 0; i < quest_names.size(); ++i) {
            {
                CCharString text("QuestManager: Activate Quest");
                DisplayProgress(text, -1.0f, false, false);
            }
            const CCharString& name = quest_names[i];
            if (IsQuestRegistered(name)) {
                const CScriptInfo* info = ScriptInfoManager->GetPScriptInfo(name);
                if (info) {
                    quests.push_back(CActivateQuestInfo(name, info, game_script, load_resources));
                    activated = true;
                }
                else {
                    quests.push_back(CActivateQuestInfo(name, 0, game_script, load_resources));
                    activated = true;
                }
            }
        }
        ActivateQuests(quests);
        return activated;
    }
};
static int Fail() { std::printf("004b4260_TEST FAIL\n"); return 1; }
int main()
{
    CScriptInfoManager infos;
    CQuestManager manager;
    manager.ScriptInfoManager = &infos;
    manager.ActivateQuestsCalls = 0;

    // 1. a mixed list: unregistered names are skipped, registered ones queue in order with or without a script info
    CCharString names[5] = { "Unknown1", "QuestA", "QuestB", "Unknown2", "QuestC" };
    CNameArray list = { names, 5 };
    g_progress_calls = 0;
    if (!manager.ActivateMultipleQuests(list, true, false)) return Fail();
    if (g_progress_calls != 5 || !g_progress_ok) return Fail();
    if (manager.ActivateQuestsCalls != 1 || manager.Activated.Count != 3) return Fail();
    if (std::strcmp(manager.Activated.Names[0], "QuestA") != 0 || manager.Activated.Infos[0] != &g_info_a) return Fail();
    if (std::strcmp(manager.Activated.Names[1], "QuestB") != 0 || manager.Activated.Infos[1] != 0) return Fail();
    if (std::strcmp(manager.Activated.Names[2], "QuestC") != 0 || manager.Activated.Infos[2] != &g_info_c) return Fail();
    for (unsigned int i = 0; i < 3; ++i)
        if (!manager.Activated.GameScript[i] || manager.Activated.LoadResources[i]) return Fail();

    // 2. the flags travel separately (second argument -> GameScript, third -> LoadResources)
    CNameArray one = { names + 2, 1 };
    if (!manager.ActivateMultipleQuests(one, false, true)) return Fail();
    if (manager.ActivateQuestsCalls != 2 || manager.Activated.Count != 1) return Fail();
    if (manager.Activated.GameScript[0] || !manager.Activated.LoadResources[0] || manager.Activated.Infos[0] != 0) return Fail();

    // 3. nothing registered: false, but ActivateQuests still runs with an empty list and progress is shown per name
    CNameArray none = { names + 3, 1 };
    g_progress_calls = 0;
    if (manager.ActivateMultipleQuests(none, true, true)) return Fail();
    if (g_progress_calls != 1 || manager.ActivateQuestsCalls != 3 || manager.Activated.Count != 0) return Fail();

    // 4. an empty list: false, no progress, ActivateQuests with an empty list
    CNameArray empty = { names, 0 };
    g_progress_calls = 0;
    if (manager.ActivateMultipleQuests(empty, true, true)) return Fail();
    if (g_progress_calls != 0 || manager.ActivateQuestsCalls != 4 || manager.Activated.Count != 0) return Fail();

    std::printf("004b4260_TEST PASS\n");
    return 0;
}
