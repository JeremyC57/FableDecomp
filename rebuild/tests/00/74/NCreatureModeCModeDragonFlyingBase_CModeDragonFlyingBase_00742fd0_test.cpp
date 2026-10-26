// behaviour test for 00742fd0: ~CModeDragonFlyingBase destruction order -- the flight spline's
// control-point storage is freed first, then the CModeBase part (whose weapon filter's group string
// is destroyed before the filter's base); a spline without storage frees nothing.
#include <cstdio>
#include <cstdlib>
static char g_log[16];
static int g_len = 0;
static void Log(char c) { if (g_len < 15) { g_log[g_len++] = c; g_log[g_len] = 0; } }
class CCharString { public: ~CCharString() { Log('S'); } };
class C3DVector { public: float X, Y, Z; };
struct CArrayC3DVector {
    C3DVector* Data; long Size; long Capacity; long GrowBy;
    ~CArrayC3DVector() { if (Data) { Log('F'); std::free(Data); } }
};
class CCardinalSpline { public: CArrayC3DVector ControlPoints; float Tension; bool EndPointsConnected; };
class CAnimFilterBase { public: ~CAnimFilterBase() { Log('A'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned long GroupCRC; };
struct CModeBase {
    void* ParentThing;
    CAnimFilterOfGroup WeaponBasedFilter;
    long FramesBeforeRemovingSelf;
    bool Active;
    ~CModeBase() { Log('M'); }
};
struct CModeDragonFlyingBase : CModeBase {
    CCardinalSpline FlightSpline;
    float SplinePercentageComplete;
    float LegAnimAlpha;
    ~CModeDragonFlyingBase() { }
};
int main()
{
    int ok = 1;
    {
        CModeDragonFlyingBase mode;
        mode.FlightSpline.ControlPoints.Data = (C3DVector*)std::malloc(sizeof(C3DVector) * 4);
    }
    if (g_log[0] != 'F' || g_log[1] != 'M' || g_log[2] != 'S' || g_log[3] != 'A' || g_len != 4) ok = 0;
    g_len = 0;
    {
        CModeDragonFlyingBase mode;
        mode.FlightSpline.ControlPoints.Data = 0;
    }
    if (g_log[0] != 'M' || g_len != 3) ok = 0;
    if (!ok) { std::printf("00742fd0_TEST FAIL\n"); return 1; }
    std::printf("00742fd0_TEST PASS\n");
    return 0;
}