// Behaviour test for 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath: the row's logic over stub callees.
#include <stdio.h>
#include <deque>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
class CBezierCurve { public: C3DVector Point1, Point2, Point3, Point4; };
static const C3DVector HeightProbeOffset(0.0f, 0.0f, 10.0f);
static float g_ground = 0.0f;        // ground height under any probe
static long g_mapOf(const C3DVector& p) { return p.X > 1000.0f ? 2 : 1; }
static int g_flightCalls = 0, g_failAt = -1;
static float g_random = 0.25f;
struct CFlying {
    float MinHeightToKeepAboveGround, MinHeightToKeepAboveGroundInFlight, MaxHeightAboveGround, FlightRange;
    float GetHeightZBelowAt(const C3DVector& p) const { return g_ground + (p.Z - p.Z); }
    bool GetRandomFlightPos(const C3DVector& from, float, C3DVector& out, bool) const
    {
        if (g_flightCalls++ == g_failAt) return false;
        out = C3DVector(from.X + 5.0f * g_flightCalls, from.Y, 20.0f);
        return true;
    }
    bool AddRandomBezierCurveToPath(std::deque<CBezierCurve>& path)
    {
        CBezierCurve curve;
        C3DVector end = path.back().Point4;
        curve.Point1 = end;
        C3DVector dir = end - path.back().Point3;
        C3DVector control = dir + end;
        long mapNumber = g_mapOf(end);
        float height = GetHeightZBelowAt(HeightProbeOffset + control);
        long tries = 10;
        while ((height + MinHeightToKeepAboveGroundInFlight > control.Z || control.X < 0.0f || control.Y < 0.0f
                || g_mapOf(control) != mapNumber) && tries > 0) {
            dir *= 0.5f;
            control = dir + end;
            height = GetHeightZBelowAt(HeightProbeOffset + control);
            --tries;
        }
        curve.Point2 = control;
        for (unsigned long i = 0; i < 2; ++i) {
            C3DVector pos;
            if (!GetRandomFlightPos(end, FlightRange, pos, false))
                return false;
            if (i == 0) { pos.Z = pos.Z + MinHeightToKeepAboveGroundInFlight; curve.Point3 = pos; }
            else { pos.Z = g_random * MaxHeightAboveGround + pos.Z + MinHeightToKeepAboveGroundInFlight; curve.Point4 = pos; }
        }
        path.push_back(curve);
        return true;
    }
};
static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    int fails = 0;
    CFlying f; f.MinHeightToKeepAboveGround = 1.0f; f.MinHeightToKeepAboveGroundInFlight = 2.0f; f.MaxHeightAboveGround = 8.0f; f.FlightRange = 50.0f;
    std::deque<CBezierCurve> path;
    CBezierCurve c0; c0.Point1 = C3DVector(0, 0, 10); c0.Point2 = C3DVector(1, 0, 10); c0.Point3 = C3DVector(8, 10, 10); c0.Point4 = C3DVector(10, 10, 10);
    path.push_back(c0);
    // mirrored control point: end + (end - P3) = (12, 10, 10); well above ground
    if (!f.AddRandomBezierCurveToPath(path) || path.size() != 2) ++fails;
    const CBezierCurve& c1 = path.back();
    if (!near(c1.Point1.X, 10) || !near(c1.Point2.X, 12) || !near(c1.Point2.Y, 10) || !near(c1.Point2.Z, 10)) ++fails;
    if (!near(c1.Point3.X, 15) || !near(c1.Point3.Z, 22)) ++fails;                       // 20 + 2
    if (!near(c1.Point4.X, 20) || !near(c1.Point4.Z, 24)) ++fails;                       // 0.25*8 + 20 + 2
    // ground too high: the control direction is halved ten times at most
    g_ground = 100.0f;
    if (!f.AddRandomBezierCurveToPath(path)) ++fails;
    const CBezierCurve& c2 = path.back();
    C3DVector expect = c1.Point4 - c1.Point3; for (int k = 0; k < 10; ++k) expect *= 0.5f;
    if (!near(c2.Point2.X, c1.Point4.X + expect.X) || !near(c2.Point2.Z, c1.Point4.Z + expect.Z)) ++fails;
    // a failing flight-position search returns false and appends nothing
    g_ground = 0.0f; g_failAt = g_flightCalls + 1;
    unsigned long before = (unsigned long)path.size();
    if (f.AddRandomBezierCurveToPath(path) || path.size() != before) ++fails;
    if (fails) { printf("00912840_TEST FAIL %d\n", fails); return 1; }
    printf("00912840_TEST PASS\n");
    return 0;
}