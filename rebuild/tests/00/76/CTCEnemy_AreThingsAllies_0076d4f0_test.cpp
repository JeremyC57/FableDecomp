// Behaviour test for 0x0076D4F0 CTCEnemy::AreThingsAllies(thing1, thing2)
#include <cstdio>

struct Enemy;
struct Thing {
    unsigned char Type;
    Enemy* EnemyTC;          // what CalculateEnemyTC resolves for this thing
    int Faction;
};
struct Enemy {
    bool FriendsWithEverything;
    int Faction;
    bool IsAllyOf(const Thing& t) const { return t.Faction == Faction; }
};

static int g_flags[2];
static int g_calls = 0;
static bool CalculateEnemyTC(const Thing& thing, const Enemy** enemy, const Thing*& owner, bool otherIsType2)
{
    g_flags[g_calls++ & 1] = otherIsType2;
    *enemy = thing.EnemyTC;
    owner = &thing;
    return *enemy != 0;
}

static bool AreThingsAllies(const Thing& thing1, const Thing& thing2)
{
    const Enemy* enemy1 = 0;
    const Enemy* enemy2 = 0;
    const Thing* owner1 = &thing1;
    const Thing* owner2 = &thing2;
    CalculateEnemyTC(thing1, &enemy1, owner1, thing2.Type == 2);
    CalculateEnemyTC(thing2, &enemy2, owner2, thing1.Type == 2);
    if (enemy1 == 0 || enemy1->FriendsWithEverything)
        return false;
    if (enemy2 == 0 || enemy2->FriendsWithEverything)
        return false;
    bool ally1 = enemy1->IsAllyOf(*owner2);
    bool ally2 = enemy2->IsAllyOf(*owner1);
    return ally1 && ally2;
}

int main()
{
    Enemy e1 = { false, 7 }, e2 = { false, 7 }, e3 = { false, 9 }, friendly = { true, 7 };
    Thing a = { 2, &e1, 7 }, b = { 1, &e2, 7 }, c = { 1, &e3, 9 }, d = { 1, 0, 7 }, f = { 1, &friendly, 7 };
    if (!AreThingsAllies(a, b)) { std::printf("0076d4f0_TEST FAIL\n"); return 1; }
    if (g_flags[0] != 0 || g_flags[1] != 1) { std::printf("0076d4f0_TEST FAIL\n"); return 1; }   // each flag tests the OTHER thing
    if (AreThingsAllies(a, c) || AreThingsAllies(c, a)) { std::printf("0076d4f0_TEST FAIL\n"); return 1; }
    if (AreThingsAllies(a, d) || AreThingsAllies(d, a)) { std::printf("0076d4f0_TEST FAIL\n"); return 1; }
    if (AreThingsAllies(a, f) || AreThingsAllies(f, b)) { std::printf("0076d4f0_TEST FAIL\n"); return 1; }
    std::printf("0076d4f0_TEST PASS\n");
    return 0;
}