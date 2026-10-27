// 0062d380 behaviour test for CWorld::EAControlledCreatureJump(const CGameEvent*)
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
static int g_actions = 0;
static const void* g_actionOwner = 0;
class CThingPlayerCreature;
class CCreatureActionHeroJump {
public:
    CThingPlayerCreature* Owner;
    explicit CCreatureActionHeroJump(CThingPlayerCreature& c) : Owner(&c) { }
};
class CThingPlayerCreature {
public:
    unsigned char Dead : 1;
    void SetCurrentAction(const CCreatureActionHeroJump& action) { ++g_actions; g_actionOwner = action.Owner; }
};
class CPlayer {
public:
    CThingPlayerCreature* Creature;
    CThingPlayerCreature* GetPControlledCreature() const { return Creature; }
};
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    CPlayer Players[2];
    CPlayer* GetPlayer(unsigned long player) { return &Players[player]; }
    void EAControlledCreatureJump(const CGameEvent* event);
};
void CWorld::EAControlledCreatureJump(const CGameEvent* event)
{
    CThingPlayerCreature* creature = GetPlayer(event->Player)->GetPControlledCreature();
    if (creature && !creature->Dead)
        creature->SetCurrentAction(CCreatureActionHeroJump(*creature));
}
static void run()
{
    CThingPlayerCreature hero; hero.Dead = 0;
    CWorld w; w.Players[0].Creature = 0; w.Players[1].Creature = &hero;
    CGameEvent e; e.Type = 50; e.Player = 0;
    w.EAControlledCreatureJump(&e);
    CHECK(g_actions == 0);
    e.Player = 1;
    w.EAControlledCreatureJump(&e);
    CHECK(g_actions == 1 && g_actionOwner == &hero);
    hero.Dead = 1;
    w.EAControlledCreatureJump(&e);
    CHECK(g_actions == 1);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0062d380_TEST FAIL\n");
        return 1;
    }
    std::printf("0062d380_TEST PASS\n");
    return 0;
}