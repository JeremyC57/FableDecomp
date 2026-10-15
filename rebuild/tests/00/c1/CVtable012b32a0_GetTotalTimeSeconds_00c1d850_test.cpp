// Test for 00c1d850 GetTotalTimeSeconds: total frames (unsigned) divided by the owner's frames per second.
#include <cstdio>
struct CFrameRateSource {
    unsigned long Fps;
    virtual ~CFrameRateSource() {}
    virtual unsigned long GetFramesPerSecond() { return Fps; }
};
struct CScriptModuleOwner { CFrameRateSource* PFrameRate; };
struct CModule {
    CScriptModuleOwner* POwner; unsigned long Frames;
    unsigned long GetTotalFrames() { return Frames; }
    unsigned long GetTotalTimeSeconds() { unsigned long frames = GetTotalFrames(); return frames / POwner->PFrameRate->GetFramesPerSecond(); }
};
int main()
{
    CFrameRateSource f; f.Fps = 30;
    CScriptModuleOwner o = { &f };
    CModule m = { &o, 95 };
    if (m.GetTotalTimeSeconds() != 3) { std::printf("00c1d850_TEST FAIL\n"); return 1; }
    m.Frames = 0xF0000000UL;                            // unsigned division (`xor edx,edx ; div`)
    if (m.GetTotalTimeSeconds() != 0xF0000000UL / 30) { std::printf("00c1d850_TEST FAIL\n"); return 1; }
    std::printf("00c1d850_TEST PASS\n");
    return 0;
}