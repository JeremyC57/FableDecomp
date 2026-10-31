// Behaviour test for CWorld::EAControlledCreatureSheatheWeapon 0x0062B7C0: the source's logic over mock players/things.
#include <cstdio>

enum EPlayerMode { PLAYER_MODE_RANGED = 0x22, PLAYER_MODE_0x2F = 0x2f };
static int g_actions, g_cancels, g_resets, g_removedMode;

class CThing;
class CTCBase { public: };
class CTCCarrying : public CTCBase { public: CThing* primary; CThing* GetThingInPrimarySlot() const { return primary; } };
class CTCHeroRangedAttacks : public CTCBase { public:
    void CancelAimingWeapon() { ++g_cancels; }
    void ResetProjectileWeaponMode() { ++g_resets; }
};
class CPlayer;
class CThing {
public:
    unsigned long UsingComponents[9];
    bool Dead;
    CTCCarrying carrying;
    CTCHeroRangedAttacks ranged;
    CPlayer* player;
    CThing() : Dead(false), player(0) { for (int i = 0; i < 9; ++i) UsingComponents[i] = 0; carrying.primary = 0; }
    void Add(long type) { UsingComponents[type >> 5] |= 1UL << (type & 31); }
    bool HasTC(long type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    template <class T> T* GetTC(long type) { return type == 0x46 ? (T*)(CTCBase*)&carrying : (T*)(CTCBase*)&ranged; }
    CPlayer& GetPlayer() { return *player; }
};
class CCreatureActionBase { };
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase&) { ++g_actions; return true; }
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase&, CCreatureActionBase*, long) { }
};
class CPlayer {
public:
    long modes;
    CThingPlayerCreature* creature;
    bool HasMode(EPlayerMode mode) const { return (modes & (1L << (mode & 31))) != 0; }
    CThingPlayerCreature* GetPControlledCreature() const { return creature; }
    void RemoveMode(EPlayerMode mode) { g_removedMode = mode; }
};
class CGameEvent { public: long Type; signed char Player; };
class CWorld {
public:
    CPlayer players[2];
    CPlayer* GetPlayer(unsigned long number) { return &players[number]; }
    void EAControlledCreatureSheatheWeapon(const CGameEvent* event);
};

void CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(PLAYER_MODE_0x2F))
        return;
    player = GetPlayer(event->Player);
    CThingPlayerCreature* hero = player->GetPControlledCreature();
    if (hero && !hero->Dead && hero->HasTC(0x46)) {
        CThing* item = hero->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (item && !item->Dead && item->HasTC(0x48))
            hero->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*hero, 0, 100));
        if (hero->HasTC(0x81)) {
            CTCHeroRangedAttacks* ranged = hero->GetTC<CTCHeroRangedAttacks>(0x81);
            ranged->CancelAimingWeapon();
            ranged->ResetProjectileWeaponMode();
            hero->GetPlayer().RemoveMode(PLAYER_MODE_RANGED);
        }
    }
}

static void Reset() { g_actions = g_cancels = g_resets = g_removedMode = 0; }
static int fail(int line) { std::printf("0062b7c0_TEST FAIL (line %d)\n", line); return 1; }

int main()
{
    CWorld world;
    CThingPlayerCreature hero;
    CThing sword;
    CGameEvent event; event.Type = 0; event.Player = 1;
    world.players[0].modes = 0; world.players[0].creature = 0;
    world.players[1].modes = 0; world.players[1].creature = &hero;
    hero.player = &world.players[1];
    hero.Add(0x46); hero.Add(0x81); sword.Add(0x48); hero.carrying.primary = &sword;

    Reset(); world.EAControlledCreatureSheatheWeapon(&event);          // full path
    if (g_actions != 1 || g_cancels != 1 || g_resets != 1 || g_removedMode != 0x22) return fail(__LINE__);

    Reset(); world.players[1].modes = 1L << 0x0f;                      // mode 0x2F blocks everything
    world.EAControlledCreatureSheatheWeapon(&event);
    if (g_actions || g_cancels || g_removedMode) return fail(__LINE__);
    world.players[1].modes = 0;

    Reset(); sword.Dead = true;                                        // dead item: no sheathe, ranged still reset
    world.EAControlledCreatureSheatheWeapon(&event);
    if (g_actions != 0 || g_cancels != 1 || g_removedMode != 0x22) return fail(__LINE__);
    sword.Dead = false;

    Reset(); hero.UsingComponents[0x81 >> 5] = 0;                      // no ranged component
    world.EAControlledCreatureSheatheWeapon(&event);
    if (g_actions != 1 || g_cancels != 0 || g_removedMode != 0) return fail(__LINE__);

    Reset(); hero.Dead = true;                                         // dead hero: nothing
    world.EAControlledCreatureSheatheWeapon(&event);
    if (g_actions || g_cancels) return fail(__LINE__);
    hero.Dead = false;

    Reset(); event.Player = 0;                                         // player without a creature
    world.EAControlledCreatureSheatheWeapon(&event);
    if (g_actions || g_cancels) return fail(__LINE__);

    std::printf("0062b7c0_TEST PASS\n");
    return 0;
}