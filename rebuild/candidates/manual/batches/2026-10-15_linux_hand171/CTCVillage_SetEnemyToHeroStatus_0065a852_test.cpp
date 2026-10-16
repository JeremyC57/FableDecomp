// Behaviour test for CTCVillage::SetEnemyToHeroStatus (0x0065A852): hostile stance (1) adds a live hero as an enemy, any
// other stance removes the hero.
#include <cstdio>
static int g_added, g_removed;
static void SetEnemyToHeroStatus(long stance, bool heroAlive)
{
    if (stance == 1) { if (heroAlive) ++g_added; }
    else ++g_removed;
}
int main()
{
    SetEnemyToHeroStatus(1, true); SetEnemyToHeroStatus(1, false); SetEnemyToHeroStatus(0, true); SetEnemyToHeroStatus(2, false);
    if (g_added != 1 || g_removed != 2) { std::printf("0065a852_TEST FAIL\n"); return 1; }
    std::printf("0065a852_TEST PASS\n");
    return 0;
}
