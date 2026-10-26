// behaviour test for 0x00B1B030 CPSCSpline::GetUpdateParam
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CParticleUpdateParameters { int setups; };
static float g_frameTime = 2.0f;
float GetParticleFrameTime_00aeaa00() { return g_frameTime; }
struct CPSCSpline {
    bool UpdateStarted; C3DVector points[4]; int count;
    float SplineCurrentPosition; bool ReverseSpline, SplineBounce; float SplineAnimSpeed;
    float lastPos, lastScale;
    void SetupUpdate(CParticleUpdateParameters& p) { ++p.setups; }
    int GetControlPointCount() const { return count; }
    void GetPointAtPos(CParticleUpdateParameters&, C3DVector* out, float pos, float scale) { lastPos = pos; lastScale = scale; out->X = pos; }
    bool GetUpdateParam(C3DVector* position, long, float timeScale, long, CParticleUpdateParameters& params, long)
    {
        if (!UpdateStarted) SetupUpdate(params);
        float frameTime = GetParticleFrameTime_00aeaa00();
        float step = frameTime * (SplineAnimSpeed / GetControlPointCount()) * timeScale;
        float splinePos;
        if (SplineBounce && ReverseSpline) splinePos = SplineCurrentPosition - step;
        else splinePos = step + SplineCurrentPosition;
        GetPointAtPos(params, position, splinePos, timeScale);
        return true;
    }
};
int main()
{
    CPSCSpline s; s.UpdateStarted = false; s.count = 4; s.SplineCurrentPosition = 1.0f; s.ReverseSpline = false; s.SplineBounce = true; s.SplineAnimSpeed = 8.0f;
    CParticleUpdateParameters p = { 0 }; C3DVector out;
    bool r = s.GetUpdateParam(&out, 0, 0.5f, 0, p, 0);   // step = 2 * (8/4) * 0.5 = 2
    int bad = !r || p.setups != 1 || s.lastPos != 3.0f || s.lastScale != 0.5f || out.X != 3.0f;
    s.UpdateStarted = true; s.ReverseSpline = true;
    s.GetUpdateParam(&out, 0, 0.5f, 0, p, 0);
    bad |= p.setups != 1 || s.lastPos != -1.0f;
    if (bad) { std::printf("00b1b030_TEST FAIL\n"); return 1; }
    std::printf("00b1b030_TEST PASS\n");
    return 0;
}