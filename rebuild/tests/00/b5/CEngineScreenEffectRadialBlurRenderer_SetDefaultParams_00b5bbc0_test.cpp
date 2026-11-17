// behaviour test for 0x00B5BBC0 CEngineScreenEffectRadialBlurRenderer::SetDefaultParams()
// (model: the default stores into the renderer's parameter block +0x198..+0x1BC, the blur / area centres +0x170 / +0x178 and the two
// tracking flags +0x1C0 / +0x1C1; the neighbouring fields are left alone). Rewritten by agentNF59 on the PDB name (the old test embedded
// a copy of the flattened header and the made-up ResetParameters wrapper); the catalog PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct Renderer {
    char  _pad[0x170];
    float BlurX, BlurY, AreaX, AreaY;                                  // +0x170
    float BlurTrack[3], AreaTrack[3];                                  // +0x180
    float ThresholdR, ThresholdG, ThresholdB, IntensityR, IntensityG, IntensityB, Length, Fade, Range, Twist;   // +0x198
    bool  AreaTrack3DPos, BlurTrack3DPos;                              // +0x1C0
    char  _pad1c2[2];
    long  ShowDebug;                                                   // +0x1C4
    void SetDefaultParams()
    {
        ThresholdR = 0.5f; ThresholdG = 0.5f; ThresholdB = 0.5f;
        IntensityR = 0.8f; IntensityG = 0.8f; IntensityB = 0.8f;
        Length = 6.0f; Range = 0.0f; Twist = 1.0f; Fade = 0.0f;
        BlurX = 0.5f; BlurY = 0.5f; AreaX = 0.5f; AreaY = 0.5f;
        AreaTrack3DPos = false; BlurTrack3DPos = false;
    }
};
}
static int bad;
static void Check(bool b) { if (!b) bad = 1; }
int main()
{
    static model::Renderer x;
    x.AreaTrack3DPos = true; x.BlurTrack3DPos = true; x.ShowDebug = 17; x.BlurTrack[0] = 12; x.AreaTrack[2] = -9;
    x.SetDefaultParams();
    Check(offsetof(model::Renderer, ThresholdR) == 0x198 && offsetof(model::Renderer, AreaTrack3DPos) == 0x1c0 && offsetof(model::Renderer, ShowDebug) == 0x1c4);
    Check(x.ThresholdR == .5f && x.ThresholdG == .5f && x.ThresholdB == .5f);
    Check(x.IntensityR == .8f && x.IntensityG == .8f && x.IntensityB == .8f);
    Check(x.Length == 6.0f && x.Range == 0.0f && x.Twist == 1.0f && x.Fade == 0.0f);
    Check(x.BlurX == .5f && x.BlurY == .5f && x.AreaX == .5f && x.AreaY == .5f);
    Check(!x.AreaTrack3DPos && !x.BlurTrack3DPos);
    Check(x.ShowDebug == 17 && x.BlurTrack[0] == 12 && x.AreaTrack[2] == -9);
    printf("00b5bbc0_TEST %s\n", bad ? "FAIL" : "PASS");
    return bad;
}
