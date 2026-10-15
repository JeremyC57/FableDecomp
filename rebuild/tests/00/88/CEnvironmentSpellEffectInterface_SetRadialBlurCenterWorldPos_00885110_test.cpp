// Behaviour test for 0x00885110 CEnvironmentSpellEffectInterface::SetRadialBlurCenterWorldPos
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; long AreaTrackPos, BlurTrackPos; };
struct CEnvironmentSpellEffectInterface {
    CRadialBlurPositionParams RadialBlurPositionParams, PrevFrameRadialBlurPositionParams;
    void SetRadialBlurCenterWorldPos(const C3DVector& position, bool interpolate)
    {
        RadialBlurPositionParams.BlurCenter = position; RadialBlurPositionParams.BlurTrackPos = 1;
        if (!interpolate) { PrevFrameRadialBlurPositionParams.BlurCenter = position; PrevFrameRadialBlurPositionParams.BlurTrackPos = 1; }
        RadialBlurPositionParams.AreaCenter = position; RadialBlurPositionParams.AreaTrackPos = 1;
        if (!interpolate) { PrevFrameRadialBlurPositionParams.AreaCenter = position; PrevFrameRadialBlurPositionParams.AreaTrackPos = 1; }
    }
};
int main()
{
    CEnvironmentSpellEffectInterface s;
    s.RadialBlurPositionParams.AreaTrackPos = s.RadialBlurPositionParams.BlurTrackPos = 0;
    s.PrevFrameRadialBlurPositionParams.AreaTrackPos = s.PrevFrameRadialBlurPositionParams.BlurTrackPos = 0;
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    s.SetRadialBlurCenterWorldPos(p, true);
    bool ok = s.RadialBlurPositionParams.BlurCenter.y == 2.0f && s.RadialBlurPositionParams.AreaCenter.z == 3.0f
           && s.RadialBlurPositionParams.AreaTrackPos == 1 && s.RadialBlurPositionParams.BlurTrackPos == 1 && s.PrevFrameRadialBlurPositionParams.BlurTrackPos == 0;
    s.SetRadialBlurCenterWorldPos(p, false);
    ok = ok && s.PrevFrameRadialBlurPositionParams.BlurTrackPos == 1 && s.PrevFrameRadialBlurPositionParams.AreaTrackPos == 1 && s.PrevFrameRadialBlurPositionParams.AreaCenter.x == 1.0f;
    if (!ok) { std::printf("00885110_TEST FAIL\n"); return 1; }
    std::printf("00885110_TEST PASS\n");
    return 0;
}