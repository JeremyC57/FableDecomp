// Behaviour test for CTCStealth::ClearDangerStatus (0x00747090): the cool-off timer always resets; only the main
// player's creature switches danger music off and rain on.
#include <cstdio>

static int g_musicCalls = 0, g_rainCalls = 0, g_worldCalls = 0;
static bool g_musicArg = true, g_rainArg = false;

struct CPlayer {
    bool m_main;
    bool IsMainPlayer() const { return m_main; }
};
struct CMusicManager { void EnableDangerMusic(bool enable) { ++g_musicCalls; g_musicArg = enable; } };
struct CEnvironment { void SetRainEnabled(bool enabled) { ++g_rainCalls; g_rainArg = enabled; } };
struct CWorld {
    CEnvironment*  PEnvironment;
    CMusicManager* PMusicManager;
};
static CMusicManager g_music;
static CEnvironment g_environment;
static CWorld g_world = { &g_environment, &g_music };

struct CThing {
    unsigned char m_thingType;
    CPlayer*      m_pPlayer;
    CPlayer* GetPlayer() const { return m_pPlayer; }
};

struct CTCStealth {
    CThing* ParentThing;
    long    InDangerCoolOffTimer;
    CWorld* GetWorld() { ++g_worldCalls; return &g_world; }
    void ClearDangerStatus();
};

void CTCStealth::ClearDangerStatus()
{
    InDangerCoolOffTimer = -1;
    if (ParentThing->GetPlayer()->IsMainPlayer() && ParentThing->m_thingType == 2) {
        CWorld* world = GetWorld();
        world->PMusicManager->EnableDangerMusic(false);
        CWorld* world2 = GetWorld();
        world2->PEnvironment->SetRainEnabled(true);
    }
}

int main()
{
    CPlayer player; player.m_main = false;
    CThing thing; thing.m_thingType = 2; thing.m_pPlayer = &player;
    CTCStealth s; s.ParentThing = &thing; s.InDangerCoolOffTimer = 30;
    s.ClearDangerStatus();
    if (s.InDangerCoolOffTimer != -1 || g_musicCalls || g_rainCalls) { std::printf("00747090_TEST FAIL\n"); return 1; }
    player.m_main = true; thing.m_thingType = 3;
    s.ClearDangerStatus();
    if (g_musicCalls || g_rainCalls) { std::printf("00747090_TEST FAIL\n"); return 1; }
    thing.m_thingType = 2;
    s.ClearDangerStatus();
    if (g_musicCalls != 1 || g_musicArg || g_rainCalls != 1 || !g_rainArg || g_worldCalls != 2) { std::printf("00747090_TEST FAIL\n"); return 1; }
    std::printf("00747090_TEST PASS\n");
    return 0;
}