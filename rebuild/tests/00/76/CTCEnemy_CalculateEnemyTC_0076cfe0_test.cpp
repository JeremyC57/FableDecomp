// Behaviour test for 0x0076CFE0 CTCEnemy::CalculateEnemyTC: own enemy component by default; a live turncoat originator
// replaces it (and becomes the enemy thing); a follower of the player's creature takes the player's component unless the
// flag argument, the global switch or its own component's EnableFollowersEnemyProxy forbids it, or the thing is of type 2.
#include <cstdio>
struct Enemy { bool EnableFollowersEnemyProxy; };
struct Thing {
    int Type; bool Dead; Enemy* EnemyTC; bool Turncoated; Thing* Originator; bool HasFollowed; const Thing* Follower;
};
static bool g_switch = true;
static Thing* g_player = 0;
static bool CalculateEnemyTC(const Thing& thing, const Enemy** tc, const Thing*& who, bool no_proxy)
{
    *tc = 0;
    bool proxied = false;
    if (thing.EnemyTC) *tc = thing.EnemyTC;
    if (thing.Turncoated && thing.Originator && !thing.Originator->Dead) {
        if (thing.Originator->EnemyTC) *tc = thing.Originator->EnemyTC;
        who = thing.Originator; proxied = true;
    }
    if (!no_proxy && g_switch && (*tc == 0 || (*tc)->EnableFollowersEnemyProxy) && thing.Type != 2) {
        const Thing* p = g_player;
        if (p && !p->Dead && p->HasFollowed && p->Follower == &thing) {
            if (p->EnemyTC) *tc = p->EnemyTC;
            who = p; proxied = true;
        }
    }
    return proxied;
}
int main()
{
    Enemy e_hero = { true }, e_dog = { true }, e_bandit = { false }, e_witch = { true };
    Thing hero = { 2, false, &e_hero, false, 0, true, 0 };
    Thing dog = { 1, false, &e_dog, false, 0, false, 0 };
    Thing witch = { 1, false, &e_witch, false, 0, false, 0 };
    Thing bandit = { 1, false, &e_bandit, true, &witch, false, 0 };
    g_player = &hero; hero.Follower = &dog;
    const Enemy* tc; const Thing* who = 0;
    bool ok = !CalculateEnemyTC(witch, &tc, who, false) && tc == &e_witch && who == 0;
    ok = ok && CalculateEnemyTC(dog, &tc, who, false) && tc == &e_hero && who == &hero;
    who = 0;
    ok = ok && !CalculateEnemyTC(dog, &tc, who, true) && tc == &e_dog && who == 0;
    g_switch = false;
    ok = ok && !CalculateEnemyTC(dog, &tc, who, false) && tc == &e_dog;
    g_switch = true;
    ok = ok && CalculateEnemyTC(bandit, &tc, who, false) && tc == &e_witch && who == &witch;
    hero.Follower = &hero;
    ok = ok && !CalculateEnemyTC(hero, &tc, who, false) && tc == &e_hero;
    if (!ok) { std::printf("0076cfe0_TEST FAIL\n"); return 1; }
    std::printf("0076cfe0_TEST PASS\n");
    return 0;
}