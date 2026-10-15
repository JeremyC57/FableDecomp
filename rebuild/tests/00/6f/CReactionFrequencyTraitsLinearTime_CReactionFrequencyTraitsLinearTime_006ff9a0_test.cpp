#include <cstdio>
// Behaviour model of 0x006FF9A0: CReactionFrequencyTraits_LinearTime(min, max).
struct CReactionFrequencyTraits_LinearTime {
    virtual ~CReactionFrequencyTraits_LinearTime() {}
    float MinWait; float WaitRange; unsigned long FrameHappened;
    CReactionFrequencyTraits_LinearTime(float minWait, float maxWait) : MinWait(minWait), WaitRange(maxWait - minWait) {}
};
int main()
{
    CReactionFrequencyTraits_LinearTime t(2.5f, 10.0f);
    if (t.MinWait != 2.5f || t.WaitRange != 7.5f) { std::printf("006ff9a0_TEST FAIL\n"); return 1; }
    CReactionFrequencyTraits_LinearTime u(4.0f, 1.0f);
    if (u.MinWait != 4.0f || u.WaitRange != -3.0f) { std::printf("006ff9a0_TEST FAIL\n"); return 1; }
    std::printf("006ff9a0_TEST PASS\n");
    return 0;
}