// Behaviour test for 0x0076D2F0: CTCEnemy::AreThingsEnemies (host model: the row's body over stub components, the callees
// replaced by table-driven stubs)
#include <cstdio>

class CFaction { public: int Id; };
class CThing {
public:
    int Type;
    bool Dead;
    class CTCEnemy* Enemy;                      // the thing's own enemy component (0 = none)
    const CThing* Proxy;                        // CalculateEnemyTC's proxy thing (0 = itself)
    unsigned long GetType() const { return (unsigned long)Type; }
    bool PeekTC(const class CTCEnemy** tc) const { if (Enemy == 0) return false; *tc = Enemy; return true; }
};
typedef CThing CThingPlayerCreature;

static const CThingPlayerCreature* g_controlled = 0;
static int g_calls = 0;

class CTCEnemy {
public:
    bool FriendsWithEverythingFlag;
    const CFaction* PFaction;
    const CThing* Enemies[4];                   // IsEnemyOf answers true for these
    const CFaction* Allied;                     // IsAlliedWithFaction answers true for this faction
    const CFaction* GetPFaction() const { return PFaction; }
    bool IsFriendsWithEverything() const { return FriendsWithEverythingFlag; }
    bool IsEnemyOf(const CThing& thing) const {
        ++g_calls;
        for (int i = 0; i < 4; ++i) if (Enemies[i] == &thing) return true;
        return false;
    }
    bool IsAlliedWithFaction(const CFaction* faction) const { return faction != 0 && faction == Allied; }
    static bool CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing, bool) {
        if (thing.Proxy != 0) { *enemy_tc = thing.Proxy->Enemy; enemy_thing = thing.Proxy; return true; }
        *enemy_tc = thing.Enemy;
        return false;
    }
    static bool AreThingsEnemies(const CThing& thing1, const CThing& thing2);
};

template <class T> static bool IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->Dead; }

// the row's body (CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature() is g_controlled)
bool CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2)
{
    const CTCEnemy* tc1;
    const CTCEnemy* tc2;
    const CThing* other1;
    const CThing* other2;
    {
        const CTCEnemy* enemy_tc1 = 0;
        const CTCEnemy* enemy_tc2 = 0;
        const CThing* enemy_thing1 = &thing1;
        const CThing* enemy_thing2 = &thing2;
        CalculateEnemyTC(thing1, &enemy_tc1, enemy_thing1, thing2.GetType() == 2);
        CalculateEnemyTC(thing2, &enemy_tc2, enemy_thing2, thing1.GetType() == 2);
        tc1 = enemy_tc1; tc2 = enemy_tc2; other1 = enemy_thing1; other2 = enemy_thing2;
    }
    if (tc1 == 0 || tc1->IsFriendsWithEverything() || tc2 == 0 || tc2->IsFriendsWithEverything())
        return false;
    bool enemies1 = tc1->IsEnemyOf(*other2);
    bool enemies2 = tc2->IsEnemyOf(*other1);
    bool enemies = enemies1 || enemies2;
    if (!enemies && thing1.GetType() != 2 && thing2.GetType() != 2) {
        const CThingPlayerCreature* player = g_controlled;
        const CTCEnemy* player_tc;
        if (IS_THING_ALIVE(player) && player->PeekTC(&player_tc)) {
            const CFaction* player_faction = player_tc->GetPFaction();
            if ((tc1->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *other2)) ||
                (tc2->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *other1)))
                enemies = true;
        }
    }
    return enemies;
}

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static void Init(CTCEnemy& e, const CFaction* f) {
    e.FriendsWithEverythingFlag = false; e.PFaction = f; e.Allied = 0;
    for (int i = 0; i < 4; ++i) e.Enemies[i] = 0;
}
static void Init(CThing& t, int type, CTCEnemy* e) { t.Type = type; t.Dead = false; t.Enemy = e; t.Proxy = 0; }

int main()
{
    CFaction heroes, bandits, villagers;
    CTCEnemy ea, eb, ep;
    Init(ea, &villagers); Init(eb, &bandits); Init(ep, &heroes);
    CThing a, b, p, none;
    Init(a, 1, &ea); Init(b, 1, &eb); Init(p, 1, &ep); Init(none, 1, 0);

    // no component on one side, or friends with everything: never enemies (and no query is asked)
    g_calls = 0;
    Check(!CTCEnemy::AreThingsEnemies(a, none) && g_calls == 0);
    eb.FriendsWithEverythingFlag = true; eb.Enemies[0] = &a;
    Check(!CTCEnemy::AreThingsEnemies(a, b));
    eb.FriendsWithEverythingFlag = false;
    // either side's component counts the other as an enemy: both queries are asked
    g_calls = 0;
    Check(CTCEnemy::AreThingsEnemies(a, b) && g_calls == 2);
    Check(CTCEnemy::AreThingsEnemies(b, a));
    eb.Enemies[0] = 0;
    Check(!CTCEnemy::AreThingsEnemies(a, b));
    // the player rule: a is allied with the player's faction and the player is an enemy of b
    g_controlled = &p; ea.Allied = &heroes; ep.Enemies[0] = &b;
    Check(CTCEnemy::AreThingsEnemies(a, b));
    Check(CTCEnemy::AreThingsEnemies(b, a));                // the second arm: tc2 allied, player v other1
    p.Dead = true;
    Check(!CTCEnemy::AreThingsEnemies(a, b));               // a dead player gives no rule
    p.Dead = false; b.Type = 2;
    Check(!CTCEnemy::AreThingsEnemies(a, b));               // type-2 things skip the player rule
    b.Type = 1; ea.Allied = 0;
    Check(!CTCEnemy::AreThingsEnemies(a, b));
    // a proxy thing speaks for its follower
    CThing follower; Init(follower, 1, 0); follower.Proxy = &a; ea.Enemies[0] = &b;
    Check(CTCEnemy::AreThingsEnemies(follower, b));
    if (g_fail) { std::printf("0076d2f0_TEST FAIL\n"); return 1; }
    std::printf("0076d2f0_TEST PASS\n");
    return 0;
}