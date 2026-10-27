// 00629f30 behaviour test for CWorld::EAClickPastText(const CGameEvent*)
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
static int g_clicks = 0;
class CSubtitleRenderer { public: void ClickPast() { ++g_clicks; } };
class CPlayerGui { public: long Other; CSubtitleRenderer SubtitleRenderer; };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CGameEvent { public: long Type; signed char Player; };
class CWorld { public: void EAClickPastText(const CGameEvent* event); };
void CWorld::EAClickPastText(const CGameEvent* event)
{
    (void)event;
    g_pPlayerGui->SubtitleRenderer.ClickPast();
}
static void run()
{
    CWorld w;
    CGameEvent e; e.Type = 25; e.Player = 0;
    w.EAClickPastText(&e);
    w.EAClickPastText(&e);
    CHECK(g_clicks == 2);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00629f30_TEST FAIL\n");
        return 1;
    }
    std::printf("00629f30_TEST PASS\n");
    return 0;
}