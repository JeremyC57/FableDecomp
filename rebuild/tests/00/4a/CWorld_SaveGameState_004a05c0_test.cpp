// Behaviour test for 0x004A05C0 CWorld::SaveGameState(const CWideString&, bool questScreens, bool flag): a bare file name
// (no directory) is resolved through the profile path (kept as-is when the profile lookup is empty); the memory pool is
// shut down around the save only when the getter returns one and BeginShutdown succeeded; quest-screen saves raise the
// two flags around the hero-state save, other saves delete a stale "<path>.hs" when it exists.
#include <cstdio>
#include <cstring>

struct CWideString {
    char text[96];
    bool assigned;
    CWideString() : assigned(false) { text[0] = 0; }
    CWideString(const char* s) : assigned(true) { std::strcpy(text, s); }
};
static void SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name)
{
    const char* slash = std::strrchr(path.text, '\\');
    *drive = CWideString();
    if (slash) {
        char d[96]; int n = (int)(slash - path.text) + 1; std::strncpy(d, path.text, n); d[n] = 0;
        *directory = CWideString(d); *name = CWideString(slash + 1);
    }
    else
        *name = CWideString(path.text);
}
static bool IsEmpty(const CWideString& s)
{
    if (!s.assigned)
        return std::strcmp("", "") == 0;
    return std::strlen(s.text) == 0;
}
static const char* g_profileDir = "C:\\Profile\\";
static CWideString GetPathNameFromFileName(const CWideString& file)
{
    if (!g_profileDir) return CWideString("");
    char p[96]; std::strcpy(p, g_profileDir); std::strcat(p, file.text); return CWideString(p);
}

static bool g_SavingQuestScreenState = false, g_SavingHeroState = false;
static int g_beginCalls = 0, g_endCalls = 0, g_deleted = 0;
static bool g_beginResult = true, g_hsExists = true;
static char g_savedPath[96], g_heroName[96], g_deletedPath[96];
static bool g_flagsAtSave[2], g_flagsAtHero[2];
struct CHandleBasedMemoryPool {
    bool BeginShutdown(float timeout) { ++g_beginCalls; return timeout == 2.0f && g_beginResult; }
    void EndShutdown() { ++g_endCalls; }
};
static CHandleBasedMemoryPool g_pool;
static CHandleBasedMemoryPool* g_poolOrNull = &g_pool;

static void SaveGameStateInternal(const CWideString& path, bool)
{
    std::strcpy(g_savedPath, path.text);
    g_flagsAtSave[0] = g_SavingQuestScreenState; g_flagsAtSave[1] = g_SavingHeroState;
}
static void SaveHeroStateInternal(const CWideString& name, bool)
{
    std::strcpy(g_heroName, name.text);
    g_flagsAtHero[0] = g_SavingQuestScreenState; g_flagsAtHero[1] = g_SavingHeroState;
}

static void SaveGameState(const CWideString& name, bool questScreens, bool flag)
{
    CWideString path(name);
    CWideString drive, directory, fileName;
    SplitPath(name, &drive, &directory, &fileName);
    if (IsEmpty(directory)) {
        CWideString profilePath = GetPathNameFromFileName(name);
        if (!IsEmpty(profilePath))
            path = profilePath;
    }
    CHandleBasedMemoryPool* pool = g_poolOrNull;
    bool shutDown = false;
    if (pool)
        shutDown = pool->BeginShutdown(2.0f);
    if (questScreens)
        g_SavingQuestScreenState = true;
    SaveGameStateInternal(path, flag);
    if (questScreens) {
        g_SavingQuestScreenState = false;
        g_SavingHeroState = true;
        SaveHeroStateInternal(name, flag);
    }
    else {
        CWideString heroState(path.text);
        std::strcat(heroState.text, ".hs");
        if (g_hsExists) { ++g_deleted; std::strcpy(g_deletedPath, heroState.text); }
    }
    g_SavingHeroState = false;
    if (shutDown)
        pool->EndShutdown();
}

int main()
{
    int ok = 1;
    SaveGameState(CWideString("Save1"), false, false);       // bare name -> profile path, stale .hs deleted
    if (std::strcmp(g_savedPath, "C:\\Profile\\Save1") != 0 || g_deleted != 1
        || std::strcmp(g_deletedPath, "C:\\Profile\\Save1.hs") != 0) ok = 0;
    if (g_beginCalls != 1 || g_endCalls != 1 || g_flagsAtSave[0] || g_flagsAtSave[1]) ok = 0;
    SaveGameState(CWideString("D:\\Saves\\Save2"), true, true);   // explicit directory kept; quest-screen hero save
    if (std::strcmp(g_savedPath, "D:\\Saves\\Save2") != 0 || std::strcmp(g_heroName, "D:\\Saves\\Save2") != 0) ok = 0;
    if (!g_flagsAtSave[0] || g_flagsAtSave[1] || g_flagsAtHero[0] || !g_flagsAtHero[1]) ok = 0;
    if (g_SavingQuestScreenState || g_SavingHeroState || g_deleted != 1) ok = 0;
    g_profileDir = 0; g_beginResult = false; g_hsExists = false;   // empty profile path; shutdown refused
    SaveGameState(CWideString("Save3"), false, false);
    if (std::strcmp(g_savedPath, "Save3") != 0 || g_deleted != 1 || g_beginCalls != 3 || g_endCalls != 2) ok = 0;
    g_poolOrNull = 0; g_beginResult = true;                         // no pool: neither call
    SaveGameState(CWideString("Save4"), false, false);
    if (g_beginCalls != 3 || g_endCalls != 2) ok = 0;
    if (!ok) { std::printf("004a05c0_TEST FAIL\n"); return 1; }
    std::printf("004a05c0_TEST PASS\n");
    return 0;
}