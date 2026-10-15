#include <cstdio>
static long g_frame = 0;
static long g_FramesPerSecond = 15;
struct CWorld { static long GetWorldFrame() { return g_frame; } static long GetFramesPerSecond() { return g_FramesPerSecond; } };
struct CHeroMarriageDef { float SecsInLoveAndHusbandPresentBeforeSexOffered; };
static int g_worldCalls = 0;
struct CTCWife {
    const CHeroMarriageDef* PHeroMarriageDef; long FrameEnteredLoveWithHusbandPresentAtHome;
    const CWorld* GetWorld() const { ++g_worldCalls; return 0; }
    bool IsOfferingSex() const
    {
        long framesPerSecond = GetWorld()->GetFramesPerSecond();
        const CHeroMarriageDef* def = PHeroMarriageDef;
        long framesInLove = CWorld::GetWorldFrame() - FrameEnteredLoveWithHusbandPresentAtHome;
        return framesInLove > framesPerSecond * def->SecsInLoveAndHusbandPresentBeforeSexOffered;
    }
};
int main()
{
    CHeroMarriageDef d; d.SecsInLoveAndHusbandPresentBeforeSexOffered = 10.0f;
    CTCWife w; w.PHeroMarriageDef = &d; w.FrameEnteredLoveWithHusbandPresentAtHome = 100;
    g_frame = 100 + 150; if (w.IsOfferingSex()) { std::printf("006ceea0_TEST FAIL\n"); return 1; }
    g_frame = 100 + 151; if (!w.IsOfferingSex() || g_worldCalls != 2) { std::printf("006ceea0_TEST FAIL\n"); return 1; }
    std::printf("006ceea0_TEST PASS\n");
    return 0;
}