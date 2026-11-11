// Behaviour test for 0x006585EE CTCVillage::NotifyHasApologised (re-identified; was CTCHeroExperience::AddSuccessfulBlockParry):
// the world frame is stored to LastFrameApologised (+0xd8); the GetWorld() call is made and its result unused.
#include <cstdio>
#include <cstddef>
class CWorld { public: static long GetFrame(); };
static long g_frame = 0x1234;
static int g_getWorldCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
class CTCVillage {
public:
    char  pad[0xd4];
    int   HeroEnemyOfGuardsUntil;                          // +0xd4
    int   LastFrameApologised;                             // +0xd8
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    void NotifyHasApologised() { LastFrameApologised = GetWorld().GetFrame(); }
};
int main()
{
    CTCVillage village;
    village.HeroEnemyOfGuardsUntil = 7;
    village.LastFrameApologised = 0;
    village.NotifyHasApologised();
    bool ok = village.LastFrameApologised == 0x1234 && village.HeroEnemyOfGuardsUntil == 7 && g_getWorldCalls == 1
              && offsetof(CTCVillage, LastFrameApologised) == 0xd8;
    g_frame = 99;
    village.NotifyHasApologised();
    ok = ok && village.LastFrameApologised == 99;
    if (ok) {
        std::printf("CTCHeroExperience_006585ee_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
