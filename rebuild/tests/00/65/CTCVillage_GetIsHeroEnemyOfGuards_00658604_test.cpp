// Behaviour test for 0x00658604 CTCVillage::GetIsHeroEnemyOfGuards: true while the world frame is before
// HeroEnemyOfGuardsUntil (+0xd4, signed compare); the GetWorld() call is made and its result unused.
#include <cstdio>
#include <cstddef>
class CWorld { public: static long GetFrame(); };
static long g_frame = 0;
static int g_getWorldCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
class CTCVillage {
public:
    char  pad[0xd4];
    int   HeroEnemyOfGuardsUntil;                          // +0xd4
    int   LastFrameApologised;                             // +0xd8
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    bool GetIsHeroEnemyOfGuards() { return GetWorld().GetFrame() < HeroEnemyOfGuardsUntil; }
};
int main()
{
    CTCVillage village;
    village.HeroEnemyOfGuardsUntil = 100;
    bool ok = offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4;
    g_frame = 99;  ok = ok && village.GetIsHeroEnemyOfGuards();
    g_frame = 100; ok = ok && !village.GetIsHeroEnemyOfGuards();
    g_frame = -5;  ok = ok && village.GetIsHeroEnemyOfGuards();
    village.HeroEnemyOfGuardsUntil = -10;
    g_frame = 0;   ok = ok && !village.GetIsHeroEnemyOfGuards();
    ok = ok && g_getWorldCalls == 4;
    if (ok) {
        std::printf("CTCVILLAGE_00658604_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
