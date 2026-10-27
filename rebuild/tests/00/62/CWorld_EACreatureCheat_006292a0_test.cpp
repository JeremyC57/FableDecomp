// 006292a0 behaviour test for CWorld::EACreatureCheat(const CGameEvent*)
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
static int g_kills = 0;
static bool g_killArg = false;
class CThingPlayerCreature { public: void Kill(bool b) { ++g_kills; g_killArg = b; } };
class CPlayer {
public:
    CThingPlayerCreature* Creature;
    CThingPlayerCreature* GetPControlledCreature() const { return Creature; }
};
class CPlayerManager { public: CPlayer Main; CPlayer& GetMainPlayer() { return Main; } };
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    CPlayerManager* PlayerManager;
    void EACreatureCheat(const CGameEvent* event);
};
void CWorld::EACreatureCheat(const CGameEvent* event)
{
    (void)event;
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (creature)
        creature->Kill(true);
}
static void run()
{
    CThingPlayerCreature hero;
    CPlayerManager pm; pm.Main.Creature = 0;
    CWorld w; w.PlayerManager = &pm;
    CGameEvent e; e.Type = 7; e.Player = 0;
    w.EACreatureCheat(&e);
    CHECK(g_kills == 0);
    pm.Main.Creature = &hero;
    w.EACreatureCheat(&e);
    CHECK(g_kills == 1 && g_killArg);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("006292a0_TEST FAIL\n");
        return 1;
    }
    std::printf("006292a0_TEST PASS\n");
    return 0;
}