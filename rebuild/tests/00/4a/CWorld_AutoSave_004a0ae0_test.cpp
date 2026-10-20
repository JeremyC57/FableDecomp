// Behaviour test for CWorld::AutoSave(bool) 0x004A0AE0: quest-start copy path vs full autosave fallback.
#include <cstdio>
#include <cstring>

struct CWideString {
    char text[32];
    CWideString(const char* t) { std::strcpy(text, t); }
    ~CWideString() {}
};
static bool g_between = false, g_questStartExists = false, g_internalResult = false;
static int g_copies = 0, g_starts = 0, g_stops = 0, g_internalCalls = 0;
static char g_lastInternal[32];
static bool g_lastInternalFlag = false;

struct CUserProfileManager {
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    CWideString GetAutoSaveFileName() const { return CWideString("AutoSave"); }
    CWideString GetAutoSavePathName() const { return CWideString("dir/AutoSave"); }
    CWideString GetAutoSaveQuestStartPathName() const { return CWideString("dir/AutoSave.qs"); }
};
static bool PathExists(const CWideString& p) { return std::strcmp(p.text, "dir/AutoSave.qs") == 0 && g_questStartExists; }
struct CQuestManager { bool IsBetweenQuestStartAndFinishScreens() const { return g_between; } };
static CQuestManager g_questManager;
static CQuestManager* g_pQuestManager = &g_questManager;

struct CWorld {
    long MostRecentSaveType;
    long MostRecentSaveTypeBeforeManualSave;
    void CopyFileInternal(const CWideString& s, const CWideString& d) const
    { if (!std::strcmp(s.text, "dir/AutoSave.qs") && !std::strcmp(d.text, "dir/AutoSave")) ++g_copies; }
    void StartAutoSaveProgress() { ++g_starts; }
    void StopAutoSaveProgress() { ++g_stops; }
    bool AutoSaveInternal(const CWideString& name, bool b)
    { ++g_internalCalls; std::strcpy(g_lastInternal, name.text); g_lastInternalFlag = b; return g_internalResult; }
    bool AutoSave(bool ignoreQuestStart);
};

bool CWorld::AutoSave(bool ignoreQuestStart)
{
    MostRecentSaveType = 2;
    MostRecentSaveTypeBeforeManualSave = 2;
    bool saved = false;
    if (!ignoreQuestStart && g_pQuestManager->IsBetweenQuestStartAndFinishScreens())
    {
        CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
        if (PathExists(questStart))
        {
            StartAutoSaveProgress();
            CWideString autoSave = CUserProfileManager::Get().GetAutoSavePathName();
            CopyFileInternal(questStart, autoSave);
            StopAutoSaveProgress();
            saved = true;
        }
    }
    if (!saved)
        saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveFileName(), true);
    return saved;
}

int main()
{
    CWorld w; w.MostRecentSaveType = 0; w.MostRecentSaveTypeBeforeManualSave = 1;
    bool ok = true;
    // quest-start copy path
    g_between = true; g_questStartExists = true; g_internalResult = false;
    if (!w.AutoSave(false) || g_copies != 1 || g_starts != 1 || g_stops != 1 || g_internalCalls != 0) ok = false;
    if (w.MostRecentSaveType != 2 || w.MostRecentSaveTypeBeforeManualSave != 2) ok = false;
    // ignore flag forces the full autosave
    g_internalResult = true;
    if (!w.AutoSave(true) || g_copies != 1 || g_internalCalls != 1 || std::strcmp(g_lastInternal, "AutoSave") || !g_lastInternalFlag) ok = false;
    // no quest-start file: fallback result is returned
    g_questStartExists = false; g_internalResult = false;
    if (w.AutoSave(false) || g_copies != 1 || g_internalCalls != 2 || g_starts != 1) ok = false;
    // not between screens
    g_between = false; g_questStartExists = true; g_internalResult = true;
    if (!w.AutoSave(false) || g_copies != 1 || g_internalCalls != 3) ok = false;
    if (!ok) { std::printf("004a0ae0_TEST FAIL\n"); return 1; }
    std::printf("004a0ae0_TEST PASS\n");
    return 0;
}