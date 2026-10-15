// Behaviour test for 0x008851a0 CEnvironmentSpellEffectInterface::SetRadialBlurCenterScreenPos (Blur centre, screen position)
#include <cstdio>
struct C2DVector { float x, y; };
struct C3DVector { float x, y, z; C3DVector() : x(9), y(9), z(9) {} C3DVector(float a, float b, float c) : x(a), y(b), z(c) {} };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; long AreaTrackPos, BlurTrackPos; };
struct CEnvironmentSpellEffectInterface {
    CRadialBlurPositionParams RadialBlurPositionParams, PrevFrameRadialBlurPositionParams;
    void SetRadialBlurCenterScreenPos(const C2DVector& position, bool interpolate)
    {
        C3DVector centre(position.x, position.y, 0.0f);
        RadialBlurPositionParams.BlurCenter = centre; RadialBlurPositionParams.BlurTrackPos = 2;
        if (!interpolate) { PrevFrameRadialBlurPositionParams.BlurCenter = centre; PrevFrameRadialBlurPositionParams.BlurTrackPos = 2; }
    }
};
int main()
{
    CEnvironmentSpellEffectInterface s;
    s.RadialBlurPositionParams.AreaTrackPos = s.RadialBlurPositionParams.BlurTrackPos = 0;
    s.PrevFrameRadialBlurPositionParams.AreaTrackPos = s.PrevFrameRadialBlurPositionParams.BlurTrackPos = 0;
    C2DVector p = { 0.25f, 0.75f };
    s.SetRadialBlurCenterScreenPos(p, true);
    bool ok = s.RadialBlurPositionParams.BlurCenter.x == 0.25f && s.RadialBlurPositionParams.BlurCenter.y == 0.75f && s.RadialBlurPositionParams.BlurCenter.z == 0.0f
           && s.RadialBlurPositionParams.BlurTrackPos == 2 && s.RadialBlurPositionParams.AreaTrackPos == 0 && s.RadialBlurPositionParams.AreaCenter.x == 9
           && s.PrevFrameRadialBlurPositionParams.BlurTrackPos == 0;
    s.SetRadialBlurCenterScreenPos(p, false);
    ok = ok && s.PrevFrameRadialBlurPositionParams.BlurTrackPos == 2 && s.PrevFrameRadialBlurPositionParams.BlurCenter.y == 0.75f && s.PrevFrameRadialBlurPositionParams.AreaTrackPos == 0;
    if (!ok) { std::printf("008851a0_TEST FAIL\n"); return 1; }
    std::printf("008851a0_TEST PASS\n");
    return 0;
}