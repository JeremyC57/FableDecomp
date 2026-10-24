#include <cstdio>

// Model of NParticleEngine::CPSCSplineBase::StartUpdate (retail 0x00B215E0).
struct CCardinalSpline { int id; };
struct CParams { int id; };

static int g_offsetUpdates = 0;
static int g_posUpdates = 0;
static const CCardinalSpline* g_splines[4];

struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline  ThisSpline;
    void Update() { ++g_offsetUpdates; }
};
struct CSplinePosParamData {
    void Update(const CParams& params, CCardinalSpline& spline) { (void)params; g_splines[g_posUpdates++] = &spline; }
};

struct CPSCSplineBase {
    bool UpdateStarted;
    CCardinalSpline Spline;
    CSplineRandomOffsetData* SplineRandomOffsetData;
    CSplinePosParamData* SplinePosParamData;
    void StartUpdate(const CParams& params)
    {
        UpdateStarted = true;
        if (SplineRandomOffsetData)
            SplineRandomOffsetData->Update();
        if (SplinePosParamData) {
            SplinePosParamData->Update(params, Spline);
            if (SplineRandomOffsetData)
                SplinePosParamData->Update(params, SplineRandomOffsetData->ThisSpline);
        }
    }
};

int main()
{
    CParams params; params.id = 1;
    CSplineRandomOffsetData offs; offs.InitialSpline = 0; offs.ThisSpline.id = 2;
    CSplinePosParamData pos;
    CPSCSplineBase s; s.UpdateStarted = false; s.Spline.id = 1; s.SplineRandomOffsetData = 0; s.SplinePosParamData = 0;
    s.StartUpdate(params);
    if (!s.UpdateStarted || g_offsetUpdates != 0 || g_posUpdates != 0) { std::printf("00b215e0_TEST FAIL\n"); return 1; }
    s.SplinePosParamData = &pos;
    s.StartUpdate(params);
    if (g_posUpdates != 1 || g_splines[0] != &s.Spline) { std::printf("00b215e0_TEST FAIL\n"); return 1; }
    s.SplineRandomOffsetData = &offs;
    s.StartUpdate(params);
    if (g_offsetUpdates != 1 || g_posUpdates != 3 || g_splines[1] != &s.Spline || g_splines[2] != &offs.ThisSpline) { std::printf("00b215e0_TEST FAIL\n"); return 1; }
    std::printf("00b215e0_TEST PASS\n");
    return 0;
}