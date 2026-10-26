// Behaviour test for 0x00cb5d80 CScriptInfoManager::Init
#include <cstdio>
#include <cstring>

static char g_log[64]; static int g_n = 0;
static void Log(char c) { if (g_n < 63) { g_log[g_n++] = c; g_log[g_n] = 0; } }

class CCharString { public: const char* text; CCharString(const char* t, long = -1) : text(t) { } };
static const char* g_progress[4]; static int g_progressN = 0; static float g_progressValue;
static void DisplayProgress(const CCharString& text, float progress, bool, bool)
{ Log('D'); g_progress[g_progressN++] = text.text; g_progressValue = progress; }

class CDefinitionManager {
public:
    virtual ~CDefinitionManager() { }
    virtual void LoadDefinitions() { Log('L'); }
};
class CScriptDefinitionManager : public CDefinitionManager { public: CScriptDefinitionManager() { Log('N'); } };
static CDefinitionManager* g_definitionManager = 0;
static void GFSetPDefinitionManager(CDefinitionManager* m) { Log('S'); g_definitionManager = m; }
static CDefinitionManager* GFGetPDefinitionManager() { return g_definitionManager; }

struct SScriptEntry { int key; };
struct CKeyPairCompareLess { };
static int g_sortCalls = 0;
static void Sort(SScriptEntry* first, SScriptEntry* last, CKeyPairCompareLess)
{   // insertion sort by key
    ++g_sortCalls; Log('Q');
    for (SScriptEntry* i = first + 1; i < last; ++i)
        for (SScriptEntry* j = i; j > first && j->key < (j - 1)->key; --j) { SScriptEntry t = *j; *j = *(j - 1); *(j - 1) = t; }
}
struct CScriptMap {
    SScriptEntry* _M_start; SScriptEntry* _M_finish; SScriptEntry* _M_end_of_storage; CKeyPairCompareLess Compare; bool Dirty;
    void Sort() { if (Dirty) { ::Sort(_M_start, _M_finish, Compare); Dirty = false; } }
};
static SScriptEntry g_entries[3];
class CScriptInfoManager { public:
    long CurrentScriptID; CScriptMap Scripts;
    void RegisterDefs() { Log('d'); }
    void RegisterScripts()
    {   // registering appends unsorted entries and marks the map dirty
        Log('r'); g_entries[0].key = 3; g_entries[1].key = 1; g_entries[2].key = 2;
        Scripts._M_start = g_entries; Scripts._M_finish = g_entries + 3; Scripts.Dirty = true;
    }
    void Init()
    {
        GFSetPDefinitionManager(new CScriptDefinitionManager());
        DisplayProgress("Registering Script Defs", -1.0f, false, false);
        RegisterDefs();
        GFGetPDefinitionManager()->LoadDefinitions();
        DisplayProgress("Registering Scripts", -1.0f, false, false);
        RegisterScripts();
        Scripts.Sort();
    }
};

int main()
{
    CScriptInfoManager m; m.Scripts.Dirty = false;
    m.Init();
    float minusOne = -1.0f;
    if (std::strcmp(g_log, "NSDdLDrQ") != 0 || g_progressN != 2 || std::strcmp(g_progress[0], "Registering Script Defs") != 0
        || std::strcmp(g_progress[1], "Registering Scripts") != 0 || g_progressValue != minusOne || m.Scripts.Dirty
        || g_entries[0].key != 1 || g_entries[1].key != 2 || g_entries[2].key != 3) { std::printf("00cb5d80_TEST FAIL\n"); return 1; }
    m.Scripts.Sort();   // clean map: no second sort
    if (g_sortCalls != 1) { std::printf("00cb5d80_TEST FAIL\n"); return 1; }
    delete g_definitionManager;
    std::printf("00cb5d80_TEST PASS\n");
    return 0;
}