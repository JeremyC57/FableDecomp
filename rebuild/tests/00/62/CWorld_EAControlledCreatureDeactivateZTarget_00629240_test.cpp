// 00629240 behaviour test for CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent*)
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
enum EPlayerMode { PLAYER_MODE_Z_TARGET = 2 };
static int g_removed = -1;
static unsigned long g_playerAsked = 99;
class CPlayer { public: int Id; void RemoveMode(EPlayerMode mode) { g_removed = mode * 100 + Id; } };
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    CPlayer Players[4];
    CPlayer* GetPlayer(unsigned long player) { g_playerAsked = player; return &Players[player]; }
    void EAControlledCreatureDeactivateZTarget(const CGameEvent* event);
};
void CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_Z_TARGET);
}
static void run()
{
    CWorld w;
    for (int i = 0; i < 4; ++i) w.Players[i].Id = i;
    CGameEvent e; e.Type = 62; e.Player = 1;
    w.EAControlledCreatureDeactivateZTarget(&e);
    CHECK(g_playerAsked == 1);
    CHECK(g_removed == 201);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00629240_TEST FAIL\n");
        return 1;
    }
    std::printf("00629240_TEST PASS\n");
    return 0;
}