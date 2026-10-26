// behaviour test for 0x00B21530 CPSCSplineBase::CPSCSplineBase
#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector() {} };
class CCardinalSpline { public:
    C3DVector points[4]; int count; float Tension; bool EndPointsConnected;
    CCardinalSpline(const C3DVector& p0, const C3DVector& p1, float tension, bool connected)
        : count(0), Tension(tension), EndPointsConnected(connected) { points[count++] = p0; points[count++] = p1; }
};
struct CPSCBase { void* ParticleSystem; CPSCBase() : ParticleSystem(0) {} };
template <class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) {} };
struct CPSCSplineBase : CPSCBase {
    float SplineAnimSpeed; CCardinalSpline Spline; CCountedPointer<int> SplineRandomOffsetData; CCountedPointer<int> SplinePosParamData;
    CPSCSplineBase() : Spline(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(0.0f, 0.0f, 0.0f), 0.5f, false) {}
};
int main()
{
    CPSCSplineBase s;
    if (s.Spline.count != 2 || s.Spline.Tension != 0.5f || s.Spline.EndPointsConnected || s.Spline.points[1].Z != 0.0f
        || s.SplineRandomOffsetData.Data != 0 || s.SplinePosParamData.Info != 0)
    { std::printf("00b21530_TEST FAIL\n"); return 1; }
    std::printf("00b21530_TEST PASS\n");
    return 0;
}