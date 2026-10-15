// Test for 007cede0 CTCCreatureTintEffect::Reset: clock zeroed, fade times = (long)(seconds * fps), colour copied.
#include <cstdio>
struct CSpecialAbilitiesCreatureTintDef { float FadeInTimeSecs; float FadeOutTimeSecs; long* Color; };
long g_GameFramesPerSecond = 15;
struct CTCCreatureTintEffect {
    CSpecialAbilitiesCreatureTintDef* PDef;
    long CurTime, FadeInTime, FadeOutTime;
    long Color[4];
    void Reset()
    {
        CurTime = 0;
        FadeInTime = (long)(PDef->FadeInTimeSecs * g_GameFramesPerSecond);
        FadeOutTime = (long)(PDef->FadeOutTimeSecs * g_GameFramesPerSecond);
        for (long i = 0; i < 4; ++i)
            Color[i] = PDef->Color[i];
    }
};
int main()
{
    long colour[4] = { 10, 20, 30, 255 };
    CSpecialAbilitiesCreatureTintDef d = { 1.5f, 2.9f, colour };
    CTCCreatureTintEffect e; e.PDef = &d; e.CurTime = 99; e.FadeInTime = e.FadeOutTime = -1;
    for (int i = 0; i < 4; ++i) e.Color[i] = 0;
    e.Reset();
    // 1.5 * 15 = 22.5 -> 22 (truncation), 2.9 * 15 = 43.5 -> 43
    if (e.CurTime != 0 || e.FadeInTime != 22 || e.FadeOutTime != 43 || e.Color[0] != 10 || e.Color[1] != 20
        || e.Color[2] != 30 || e.Color[3] != 255) { std::printf("007cede0_TEST FAIL\n"); return 1; }
    g_GameFramesPerSecond = 30;
    e.Reset();
    if (e.FadeInTime != 45 || e.FadeOutTime != 87) { std::printf("007cede0_TEST FAIL\n"); return 1; }
    std::printf("007cede0_TEST PASS\n");
    return 0;
}