// Behaviour test for 00556470: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_volumeCalls = 0;
static unsigned long g_masks[8];
class CSoundManagerLH {
public:
    virtual ~CSoundManagerLH() { }
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime)   // stub for 0x00A3E9D0
    {
        CHECK(volume == 0.0f && fadeTime == 0.0f);
        g_masks[g_volumeCalls++ & 7] = groupMask;
    }
};
static CSoundManagerLH* g_pSoundSystem = 0;
static bool SoundResetOk(int base) { return g_masks[base] == 0x10 && g_masks[base + 1] == 0x80 && g_masks[base + 2] == 0x40; }

static int g_loads = 0;
static bool g_flagDuringLoad = true;
class CUserProfileManager {
public:
    bool ApplyVideoValuesOnLoad;
    CUserProfileManager() : ApplyVideoValuesOnLoad(true) { }
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }   // stub for 0x0040D2A0
    void Load() { ++g_loads; g_flagDuringLoad = ApplyVideoValuesOnLoad; }           // stub for 0x0040D350
};
class CRedefinerList {
public:
    bool IsActivated;
    int  Deactivations;
    CRedefinerList() : IsActivated(false), Deactivations(0) { }
    virtual ~CRedefinerList() { }
    virtual void OnDeactivate() { ++Deactivations; IsActivated = false; }
    static CRedefinerList* s_pActiveList;
    static void Cancel()
    {
        if (s_pActiveList != 0 && s_pActiveList->IsActivated) {
            s_pActiveList->OnDeactivate();
            CUserProfileManager::Get().ApplyVideoValuesOnLoad = false;
            CUserProfileManager::Get().Load();
            CUserProfileManager::Get().ApplyVideoValuesOnLoad = true;
            if (g_pSoundSystem != 0) {
                g_pSoundSystem->SetGroupVolume(0x10, 0.0f, 0.0f);
                g_pSoundSystem->SetGroupVolume(0x80, 0.0f, 0.0f);
                g_pSoundSystem->SetGroupVolume(0x40, 0.0f, 0.0f);
            }
        }
    }
};
CRedefinerList* CRedefinerList::s_pActiveList = 0;
static void RunTest()
{
    CRedefinerList::Cancel();                                  // no active list
    CHECK(g_loads == 0);
    CRedefinerList list;
    CRedefinerList::s_pActiveList = &list;
    CRedefinerList::Cancel();                                  // not activated
    CHECK(g_loads == 0 && list.Deactivations == 0);
    list.IsActivated = true;
    CRedefinerList::Cancel();                                  // no sound system
    CHECK(list.Deactivations == 1 && g_loads == 1 && !g_flagDuringLoad);
    CHECK(CUserProfileManager::Get().ApplyVideoValuesOnLoad && g_volumeCalls == 0);
    CSoundManagerLH sound;
    g_pSoundSystem = &sound;
    list.IsActivated = true;
    CRedefinerList::Cancel();
    CHECK(list.Deactivations == 2 && g_loads == 2 && g_volumeCalls == 3 && SoundResetOk(0));
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00556470_TEST FAIL\n");
        return 1;
    }
    std::printf("00556470_TEST PASS\n");
    return 0;
}