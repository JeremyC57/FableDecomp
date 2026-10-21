// Behaviour test for CAT_Phase1Archer::Init (0x00F08A30): enemy links with the hero and each living bandit, early
// return when the script is done, awareness of the hero and one registered callback.
#include <cstdio>
#include <cstring>
struct Thing { int id; bool alive; };
static int g_enemy[32][2]; static int g_nEnemy;
static int g_aware[2]; static int g_added;
static void Enemy(const Thing& a, const Thing& b) { g_enemy[g_nEnemy][0] = a.id; g_enemy[g_nEnemy][1] = b.id; ++g_nEnemy; }
static void Init(const Thing& me, const Thing& hero, const Thing* bandits, unsigned int n, int doneAt)
{
    Enemy(me, hero);
    Enemy(hero, me);
    int checks = 0;
    for (unsigned int i = 0; i < n; ++i) {
        if (bandits[i].alive) {
            if (checks++ == doneAt)
                return;
            Enemy(me, bandits[i]);
            Enemy(bandits[i], me);
        }
    }
    g_aware[0] = me.id; g_aware[1] = hero.id;
    ++g_added;
}
int main()
{
    Thing me = { 1, true }, hero = { 2, true };
    Thing b[3] = { { 10, true }, { 11, false }, { 12, true } };
    g_nEnemy = 0; g_added = 0; g_aware[0] = 0;
    Init(me, hero, b, 3, -1);
    bool ok = g_nEnemy == 6 && g_enemy[0][0] == 1 && g_enemy[0][1] == 2 && g_enemy[1][0] == 2
        && g_enemy[2][1] == 10 && g_enemy[3][0] == 10 && g_enemy[4][1] == 12 && g_aware[0] == 1 && g_aware[1] == 2
        && g_added == 1;
    g_nEnemy = 0; g_added = 0; g_aware[0] = 0;
    Init(me, hero, b, 3, 1);
    ok = ok && g_nEnemy == 4 && g_added == 0 && g_aware[0] == 0;
    if (!ok) { std::printf("00f08a30_TEST FAIL\n"); return 1; }
    std::printf("00f08a30_TEST PASS\n");
    return 0;
}
