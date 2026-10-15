// Behaviour test for 0x00885210 CEnvironmentSpellEffectInterface::SetRadialBlurCenterAreaScreenPos (Area centre, screen position)
#include <cstdio>
struct C2DVector { float x, y; };
struct C3DVector { float x, y, z; C3DVector() : x(9), y(9), z(9) {} C3DVector(float a, float b, float c) : x(a), y(b), z(c) {} };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; long AreaTrackPos, BlurTrackPos; };
struct CEnvironmentSpellEffectInterface {
    CRadialBlurPositionParams RadialBlurPositionParams, PrevFrameRadialBlurPositionParams;
    void SetRadialBlurCenterAreaScreenPos(const C2DVector& position, bool interpolate)
    {
        C3DVector centre(position.x, position.y, 0.0f);
        RadialBlurPositionParams.AreaCenter = centre; RadialBlurPositionParams.AreaTrackPos = 2;
        if (!interpolate) { PrevFrameRadialBlurPositionParams.AreaCenter = centre; PrevFrameRadialBlurPositionParams.AreaTrackPos = 2; }
    }
};
int main()
{
    CEnvironmentSpellEffectInterface s;
    s.RadialBlurPositionParams.AreaTrackPos = s.RadialBlurPositionParams.BlurTrackPos = 0;
    s.PrevFrameRadialBlurPositionParams.AreaTrackPos = s.PrevFrameRadialBlurPositionParams.BlurTrackPos = 0;
    C2DVector p = { 0.25f, 0.75f };
    s.SetRadialBlurCenterAreaScreenPos(p, true);
    bool ok = s.RadialBlurPositionParams.AreaCenter.x == 0.25f && s.RadialBlurPositionParams.AreaCenter.y == 0.75f && s.RadialBlurPositionParams.AreaCenter.z == 0.0f
           && s.RadialBlurPositionParams.AreaTrackPos == 2 && s.RadialBlurPositionParams.BlurTrackPos == 0 && s.RadialBlurPositionParams.BlurCenter.x == 9
           && s.PrevFrameRadialBlurPositionParams.AreaTrackPos == 0;
    s.SetRadialBlurCenterAreaScreenPos(p, false);
    ok = ok && s.PrevFrameRadialBlurPositionParams.AreaTrackPos == 2 && s.PrevFrameRadialBlurPositionParams.AreaCenter.y == 0.75f && s.PrevFrameRadialBlurPositionParams.BlurTrackPos == 0;
    if (!ok) { std::printf("00885210_TEST FAIL\n"); return 1; }
    std::printf("00885210_TEST PASS\n");
    return 0;
}
