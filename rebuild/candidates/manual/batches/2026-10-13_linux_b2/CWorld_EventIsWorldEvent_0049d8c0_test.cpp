#include <cstdio>

struct CGameEvent { long Type; };
struct CGameEventInfo { void* WorldHandler; unsigned char rest[0x3C]; };
CGameEventInfo g_GameEventInfo[3];
struct W { bool EventIsWorldEvent(CGameEvent* e) { return g_GameEventInfo[e->Type].WorldHandler != 0; } };
int main()
{
    int x; g_GameEventInfo[2].WorldHandler = &x; g_GameEventInfo[1].WorldHandler = 0;
    CGameEvent e; W w; e.Type = 2; if (!w.EventIsWorldEvent(&e)) { std::printf("0049d8c0_TEST FAIL\n"); return 1; }
    e.Type = 1; if (w.EventIsWorldEvent(&e)) { std::printf("0049d8c0_TEST FAIL\n"); return 1; }
    std::printf("0049d8c0_TEST PASS\n");
    return 0;
}
