// Behaviour test for CCreatureAction_WaspQueenDie::FrameUpdate (0x008C3F20): alpha = 255 - 51*LoopNum - 51*frames/total, truncated
// to a byte and set on the creature's graphic appearance; then the base FrameUpdate.
#include <cstdio>
static int g_baseCalls;
struct CTCGraphicAppearance { unsigned char Alpha; void SetAlpha(unsigned char a) { Alpha = a; } };
class CThing { public: CTCGraphicAppearance* PTCGraphicAppearance; CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; } };
struct CPointer { CThing* Item; CThing* operator->() const { return Item; } };
class CCreatureAction_WaspQueenDie { public:
    CPointer PCreature;
    long TotalTime, FramesRunning, LoopNum;
    long GetTotalTimeWorldFrames() const { return TotalTime; }
    long GetFramesRunning() const { return FramesRunning; }
    void FrameUpdate()
    {
        long totalTime = GetTotalTimeWorldFrames();
        float startAlpha = 255.0f - LoopNum * 51.0f;
        float alpha = startAlpha - (float)GetFramesRunning() / totalTime * 51.0f;
        PCreature->GetTCGraphicAppearance()->SetAlpha((unsigned char)(long)alpha);
        ++g_baseCalls;
    }
};
int main()
{
    CTCGraphicAppearance app = { 0 };
    CThing queen = { &app };
    CCreatureAction_WaspQueenDie a;
    a.PCreature.Item = &queen;
    a.TotalTime = 10; a.FramesRunning = 5; a.LoopNum = 1;
    a.FrameUpdate();
    int fail = app.Alpha != 178 || g_baseCalls != 1;          // 255 - 51 - 25.5 = 178.5 -> 178
    a.LoopNum = 0; a.FramesRunning = 0; a.FrameUpdate();
    if (app.Alpha != 255 || g_baseCalls != 2) fail = 1;
    a.LoopNum = 4; a.FramesRunning = 10; a.FrameUpdate();     // 255 - 204 - 51 = 0
    if (app.Alpha != 0) fail = 1;
    if (fail) { std::printf("008c3f20_TEST FAIL\n"); return 1; }
    std::printf("008c3f20_TEST PASS\n");
    return 0;
}