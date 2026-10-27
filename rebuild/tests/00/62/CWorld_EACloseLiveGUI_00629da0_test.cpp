// 00629da0 behaviour test for CWorld::EACloseLiveGUI(const CGameEvent*)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
enum EPlayerMode { PLAYER_MODE_LIVE_GUI = 0x2b };
static int g_seq[8]; static int g_n = 0;
static int g_removedMode = -1;
class CPlayer { public: void RemoveMode(EPlayerMode mode) { g_removedMode = mode; g_seq[g_n++] = 2; } };
class CHeroInfoScreensManager { public: void Close() { g_seq[g_n++] = 3; } };
class CPlayerGui {
public:
    CHeroInfoScreensManager Screens;
    CHeroInfoScreensManager* GetPHeroInfoScreensManager() { return &Screens; }
};
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    bool Paused;
    CPlayer Players[2];
    void SetAsPaused(bool paused) { Paused = paused; g_seq[g_n++] = 1; }
    CPlayer* GetPlayer(unsigned long player) { return &Players[player]; }
    void EACloseLiveGUI(const CGameEvent* event);
};
void CWorld::EACloseLiveGUI(const CGameEvent* event)
{
    SetAsPaused(false);
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_LIVE_GUI);
    g_pPlayerGui->GetPHeroInfoScreensManager()->Close();
}
static void run()
{
    CWorld w; w.Paused = true;
    CGameEvent e; e.Type = 66; e.Player = 1;
    w.EACloseLiveGUI(&e);
    CHECK(!w.Paused);
    CHECK(g_removedMode == 0x2b);
    CHECK(g_n == 3 && g_seq[0] == 1 && g_seq[1] == 2 && g_seq[2] == 3);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00629da0_TEST FAIL\n");
        return 1;
    }
    std::printf("00629da0_TEST PASS\n");
    return 0;
}