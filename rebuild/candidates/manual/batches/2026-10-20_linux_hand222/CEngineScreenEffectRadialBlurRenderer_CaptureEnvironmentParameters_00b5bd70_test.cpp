// Behaviour test for 0x00B5BD70 CEngineScreenEffectRadialBlurRenderer::CaptureEnvironmentParameters (model copy).
#include <cstdio>

struct V3 { float X, Y, Z; };
struct SBlur {
    V3 BlurCenter, AreaCenter;
    float TR, TG, TB, pad1, IR, IG, IB, pad2, Length, Fade, Range, Twist;
    long BlurTrackMode, AreaTrackMode;
};
struct SSettings { char pad[0x60]; SBlur RadialBlur; };
struct CEnv { SSettings s; SSettings* GetSettings() { return &s; } };
struct CProv { CEnv e; CEnv* GetCurrentEnvironment() { return &e; } };
struct SState { CProv* m_pEnvironmentProvider; };
static SState g_state;

struct CRenderer {
    float BlurCenter_X, BlurCenter_Y, AreaCenter_X, AreaCenter_Y;
    V3 BlurTrack, AreaTrack;
    float TR, TG, TB, IR, IG, IB, Length, Fade, Range, Twist;
    bool AreaTrack3DPos, BlurTrack3DPos;
    void Capture()
    {
        const SBlur& env = g_state.m_pEnvironmentProvider->GetCurrentEnvironment()->GetSettings()->RadialBlur;
        Fade = env.Fade; Range = env.Range; Length = env.Length; Twist = env.Twist;
        IR = env.IR; IG = env.IG; IB = env.IB; TR = env.TR; TG = env.TG; TB = env.TB;
        AreaTrack3DPos = env.AreaTrackMode == 1;
        BlurTrack3DPos = env.BlurTrackMode == 1;
        if (AreaTrack3DPos) AreaTrack = env.AreaCenter; else { AreaCenter_X = env.AreaCenter.X; AreaCenter_Y = env.AreaCenter.Y; }
        if (BlurTrack3DPos) BlurTrack = env.BlurCenter; else { BlurCenter_X = env.BlurCenter.X; BlurCenter_Y = env.BlurCenter.Y; }
    }
};

int main()
{
    static CProv prov;
    g_state.m_pEnvironmentProvider = &prov;
    SBlur& b = prov.e.s.RadialBlur;
    b.BlurCenter.X = 1; b.BlurCenter.Y = 2; b.BlurCenter.Z = 3;
    b.AreaCenter.X = 4; b.AreaCenter.Y = 5; b.AreaCenter.Z = 6;
    b.TR = 7; b.IG = 8; b.Fade = 9; b.Twist = 10;
    b.AreaTrackMode = 1; b.BlurTrackMode = 2;
    CRenderer r;
    r.BlurTrack.X = r.BlurTrack.Y = r.BlurTrack.Z = 0; r.AreaCenter_X = r.AreaCenter_Y = 0;
    r.Capture();
    if (!r.AreaTrack3DPos || r.BlurTrack3DPos || r.AreaTrack.Z != 6 || r.AreaCenter_X != 0 ||
        r.BlurCenter_X != 1 || r.BlurCenter_Y != 2 || r.BlurTrack.X != 0 || r.TR != 7 || r.IG != 8 ||
        r.Fade != 9 || r.Twist != 10) { std::printf("00b5bd70_TEST FAIL\n"); return 1; }
    b.AreaTrackMode = 0; b.BlurTrackMode = 1;
    r.Capture();
    if (r.AreaTrack3DPos || !r.BlurTrack3DPos || r.AreaCenter_X != 4 || r.AreaCenter_Y != 5 || r.BlurTrack.Z != 3)
    { std::printf("00b5bd70_TEST FAIL\n"); return 1; }
    std::printf("00b5bd70_TEST PASS\n");
    return 0;
}
