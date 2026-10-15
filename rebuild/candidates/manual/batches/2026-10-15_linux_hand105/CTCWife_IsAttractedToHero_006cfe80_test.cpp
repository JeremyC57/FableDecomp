#include <cstdio>
static const float g_zero = 0.0f;
struct CTCWife {
    bool courting; int OpinionAttitude; float LoveAttitudeValue; int courtingCalls;
    bool IsCourtingHero() const { ++const_cast<CTCWife*>(this)->courtingCalls; return courting; }
    bool IsInLoveWithHero() const { return IsCourtingHero() && OpinionAttitude == 12; }
    bool IsAttractedToHero() const { return IsCourtingHero() && (LoveAttitudeValue > g_zero || IsInLoveWithHero()); }
};
int main()
{
    CTCWife w; w.courting = false; w.OpinionAttitude = 12; w.LoveAttitudeValue = 1.0f; w.courtingCalls = 0;
    if (w.IsAttractedToHero() || w.courtingCalls != 1) { std::printf("006cfe80_TEST FAIL\n"); return 1; }
    w.courting = true; if (!w.IsAttractedToHero()) { std::printf("006cfe80_TEST FAIL\n"); return 1; }
    w.LoveAttitudeValue = 0.0f; w.courtingCalls = 0; if (!w.IsAttractedToHero() || w.courtingCalls != 2) { std::printf("006cfe80_TEST FAIL\n"); return 1; }
    w.OpinionAttitude = 3; if (w.IsAttractedToHero()) { std::printf("006cfe80_TEST FAIL\n"); return 1; }
    std::printf("006cfe80_TEST PASS\n");
    return 0;
}
