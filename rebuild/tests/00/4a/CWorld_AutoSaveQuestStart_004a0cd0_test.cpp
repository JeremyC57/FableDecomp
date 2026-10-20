// Behaviour test for CWorld::AutoSaveQuestStart() 0x004A0CD0: save as ".qs", copy it over the autosave, drop the checkpoint.
#include <cstdio>
#include <cstring>

struct CWideString {
    char text[32];
    CWideString(const char* t) { std::strcpy(text, t); }
    ~CWideString() {}
};
static bool g_internalResult = false;
static int g_copies = 0, g_starts = 0, g_stops = 0, g_deletes = 0, g_internalCalls = 0;
static char g_lastInternal[32];
static bool g_lastInternalFlag = true;

struct CUserProfileManager {
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    CWideString GetAutoSaveQuestStartFileName() const { return CWideString("AutoSave.qs"); }
    CWideString GetAutoSavePathName() const { return CWideString("dir/AutoSave"); }
    CWideString GetAutoSaveQuestStartPathName() const { return CWideString("dir/AutoSave.qs"); }
};

struct CWorld {
    long MostRecentSaveType;
    long MostRecentSaveTypeBeforeManualSave;
    bool AutoSaveCheckPointExists;
    void CopyFileInternal(const CWideString& s, const CWideString& d) const
    { if (!std::strcmp(s.text, "dir/AutoSave.qs") && !std::strcmp(d.text, "dir/AutoSave")) ++g_copies; }
    void StartAutoSaveProgress() { ++g_starts; }
    void StopAutoSaveProgress() { ++g_stops; }
    static void DeleteCheckPointSave() { ++g_deletes; }
    bool AutoSaveInternal(const CWideString& name, bool b)
    { ++g_internalCalls; std::strcpy(g_lastInternal, name.text); g_lastInternalFlag = b; return g_internalResult; }
    bool AutoSaveQuestStart();
};

bool CWorld::AutoSaveQuestStart()
{
    MostRecentSaveType = 3;
    MostRecentSaveTypeBeforeManualSave = 3;
    bool saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveQuestStartFileName(), false);
    if (saved)
    {
        StartAutoSaveProgress();
        CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
        CWideString autoSave = CUserProfileManager::Get().GetAutoSavePathName();
        CopyFileInternal(questStart, autoSave);
        DeleteCheckPointSave();
        AutoSaveCheckPointExists = false;
        StopAutoSaveProgress();
        return true;
    }
    return false;
}

int main()
{
    CWorld w; w.MostRecentSaveType = 0; w.MostRecentSaveTypeBeforeManualSave = 0; w.AutoSaveCheckPointExists = true;
    bool ok = true;
    g_internalResult = false;
    if (w.AutoSaveQuestStart() || g_copies || g_starts || g_deletes || !w.AutoSaveCheckPointExists) ok = false;
    if (w.MostRecentSaveType != 3 || w.MostRecentSaveTypeBeforeManualSave != 3) ok = false;
    if (std::strcmp(g_lastInternal, "AutoSave.qs") || g_lastInternalFlag) ok = false;
    g_internalResult = true;
    if (!w.AutoSaveQuestStart() || g_copies != 1 || g_starts != 1 || g_stops != 1 || g_deletes != 1) ok = false;
    if (w.AutoSaveCheckPointExists || g_internalCalls != 2) ok = false;
    if (!ok) { std::printf("004a0cd0_TEST FAIL\n"); return 1; }
    std::printf("004a0cd0_TEST PASS\n");
    return 0;
}