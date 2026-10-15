#include <cstdio>
// Behaviour test for 00b21a00: the constructor stores the spline, starts with an empty parameter array and resizes it
// to one zero entry per control point (12-byte control points).
struct C3DVector { float X, Y, Z; };
static unsigned long g_resizeCount = 0; static unsigned long g_resizeValue = 99; static int g_resizeCalls = 0;
static unsigned long g_storage[16];
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    long Size() const { return (long)(m_end - m_begin); }
    void resize(unsigned long newSize, const T& value);
};
template<> void CArray<unsigned long>::resize(unsigned long newSize, const unsigned long& value)
{
    ++g_resizeCalls; g_resizeCount = newSize; g_resizeValue = value;
    if (m_begin == 0) { m_begin = g_storage; m_end = g_storage; m_capacity = g_storage + 16; }
    for (unsigned long i = 0; i < newSize; ++i) m_begin[i] = value;
    m_end = m_begin + newSize;
}
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float Tension;
    bool EndPointsConnected;
};
struct CParticleSystem;
struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;
    CCardinalSpline*      Spline;
    bool                  MaintainSplineShape;
    CArray<unsigned long> SplinePosParam;
    CSplinePosParamData(CCardinalSpline* spline);
};
CSplinePosParamData::CSplinePosParamData(CCardinalSpline* spline)
    : Spline(spline)
{
    unsigned long count = Spline->ControlPoints.Size();
    SplinePosParam.resize(count, 0);
}
int main()
{
    C3DVector points[5];
    CCardinalSpline spline;
    spline.ControlPoints.m_begin = points; spline.ControlPoints.m_end = points + 5; spline.ControlPoints.m_capacity = points + 5;
    for (int i = 0; i < 16; ++i) g_storage[i] = 77;
    CSplinePosParamData data(&spline);
    if (data.Spline != &spline || g_resizeCalls != 1 || g_resizeCount != 5 || g_resizeValue != 0 || data.SplinePosParam.Size() != 5
        || data.SplinePosParam.m_begin[0] != 0 || data.SplinePosParam.m_begin[4] != 0 || g_storage[5] != 77) {
        std::printf("00b21a00_TEST FAIL\n"); return 1;
    }
    std::printf("00b21a00_TEST PASS\n");
    return 0;
}