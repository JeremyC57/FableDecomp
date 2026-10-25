// Behaviour test for 00c1d820: CScriptModuleTimer::GetRemainingTimeSeconds -- the frames still to run divided (unsigned) by the owner's frames-per-second
#include <cstdio>

struct CFrameRateSource {
    unsigned int m_fps;
    virtual void v00() { }
    virtual void v01() { }
    virtual unsigned int GetFramesPerSecond() { return m_fps; }
};
struct CScriptModuleOwner { void* _pad00; CFrameRateSource* PFrameRate; };

struct CScriptModuleTimer {
    CScriptModuleOwner* POwner;
    unsigned int m_frames;
    unsigned int FramesRemaining() { return m_frames; }
    unsigned int GetRemainingTimeSeconds()
    {
        unsigned int frames = FramesRemaining();
        return frames / POwner->PFrameRate->GetFramesPerSecond();
    }
};

int main()
{
    CFrameRateSource rate; rate.m_fps = 30;
    CScriptModuleOwner owner = { 0, &rate };
    CScriptModuleTimer t; t.POwner = &owner;
    t.m_frames = 95;
    if (t.GetRemainingTimeSeconds() != 3) { std::printf("00c1d820_TEST FAIL\n"); return 1; }
    t.m_frames = 0x90000000u; rate.m_fps = 2;           // unsigned divide
    if (t.GetRemainingTimeSeconds() != 0x48000000u) { std::printf("00c1d820_TEST FAIL\n"); return 1; }
    t.m_frames = 29; rate.m_fps = 30;
    if (t.GetRemainingTimeSeconds() != 0) { std::printf("00c1d820_TEST FAIL\n"); return 1; }
    std::printf("00c1d820_TEST PASS\n");
    return 0;
}