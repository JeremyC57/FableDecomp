// CEngineScreenEffectRadialBlurRenderer::CaptureEnvironmentParameters() -- copy the current environment's radial-blur settings into CurrentParameters and the blur/area centres (3D track positions when the environment tracks them, else the 2D screen centres)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BD70, length 262 bytes.
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (returns the current
// environment), virtual slot 1 of that environment (returns its settings record; the radial-blur block sits at +0x60).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h (CurrentParameters +0x198.., centres +0x170.., track flags
// +0x1c0/+0x1c1). The environment's radial-blur block is modelled locally: blur centre C3DVector +0, area centre
// C3DVector +0x0c, thresholds +0x18, intensities +0x28, length/fade/range/twist +0x38.., track modes +0x48/+0x4c (longs).
// Shape: the parameters are copied member by member in retail's store order (fade, range, length, twist, intensities,
// thresholds); each track flag is `mode == 1`; the 3D positions are whole C3DVector assignments (both addresses formed
// with `lea`), the 2D centres take X and Y separately.
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

class C3DVector { public:
    float X, Y, Z;
};

struct SEnvironmentRadialBlur {
    C3DVector     BlurCenter;                      // +0x00
    C3DVector     AreaCenter;                      // +0x0c
    float         ThresholdR;                      // +0x18
    float         ThresholdG;                      // +0x1c
    float         ThresholdB;                      // +0x20
    unsigned char _pad24[4];
    float         IntensityR;                      // +0x28
    float         IntensityG;                      // +0x2c
    float         IntensityB;                      // +0x30
    unsigned char _pad34[4];
    float         Length;                          // +0x38
    float         Fade;                            // +0x3c
    float         Range;                           // +0x40
    float         Twist;                           // +0x44
    long          BlurTrackMode;                   // +0x48
    long          AreaTrackMode;                   // +0x4c
};

struct SEnvironmentSettings {
    unsigned char          _pad[0x60];
    SEnvironmentRadialBlur RadialBlur;             // +0x60
};

class CEnvironment { public:
    virtual void v00();
    virtual SEnvironmentSettings* GetSettings();   // slot 1
};

struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};

struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

struct CEngineScreenEffectRadialBlurRenderer_Env : CEngineScreenEffectRadialBlurRenderer {
    C3DVector& BlurTrackPosition() { return *reinterpret_cast<C3DVector*>(&BlurCenterWorldTrackPosition_X); }
    C3DVector& AreaTrackPosition() { return *reinterpret_cast<C3DVector*>(&AreaCenterWorldTrackPosition_X); }
    void CaptureEnvironmentParameters();
};

void CEngineScreenEffectRadialBlurRenderer_Env::CaptureEnvironmentParameters()
{
    const SEnvironmentRadialBlur& env = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetSettings()->RadialBlur;
    CurrentParameters_Fade = env.Fade;
    CurrentParameters_Range = env.Range;
    CurrentParameters_Length = env.Length;
    CurrentParameters_Twist = env.Twist;
    CurrentParameters_IntensityR = env.IntensityR;
    CurrentParameters_IntensityG = env.IntensityG;
    CurrentParameters_IntensityB = env.IntensityB;
    CurrentParameters_ThresholdR = env.ThresholdR;
    CurrentParameters_ThresholdG = env.ThresholdG;
    CurrentParameters_ThresholdB = env.ThresholdB;
    AreaTrack3DPos = env.AreaTrackMode == 1;
    BlurTrack3DPos = env.BlurTrackMode == 1;
    if (AreaTrack3DPos) {
        AreaTrackPosition() = env.AreaCenter;
    } else {
        AreaCenter_X = env.AreaCenter.X;
        AreaCenter_Y = env.AreaCenter.Y;
    }
    if (BlurTrack3DPos) {
        BlurTrackPosition() = env.BlurCenter;
    } else {
        BlurCenter_X = env.BlurCenter.X;
        BlurCenter_Y = env.BlurCenter.Y;
    }
}