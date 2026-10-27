// 0062c6e0 behaviour test for CWorld::EAOpenPCMSNChatMenu(const CGameEvent*)
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
static int g_fetches = 0;
static unsigned long g_playerAsked = 99;
class CThingPlayerCreature { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const { ++g_fetches; return 0; } };
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    CPlayer Players[4];
    CPlayer* GetPlayer(unsigned long player) { g_playerAsked = player; return &Players[player]; }
    void EAOpenPCMSNChatMenu(const CGameEvent* event);
};
void CWorld::EAOpenPCMSNChatMenu(const CGameEvent* event)
{
    CThingPlayerCreature* hero = GetPlayer(event->Player)->GetPControlledCreature();
    (void)hero;
}
static void run()
{
    CWorld w;
    CGameEvent e; e.Type = 71; e.Player = 2;
    w.EAOpenPCMSNChatMenu(&e);
    CHECK(g_playerAsked == 2);
    CHECK(g_fetches == 1);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0062c6e0_TEST FAIL\n");
        return 1;
    }
    std::printf("0062c6e0_TEST PASS\n");
    return 0;
}