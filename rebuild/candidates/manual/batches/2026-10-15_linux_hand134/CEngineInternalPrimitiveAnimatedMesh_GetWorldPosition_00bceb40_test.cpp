#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CSet { float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate; };
struct CInfo { CSet Sets[6]; long WFServerCurrentTime; CSet InterpolationSetList[3]; };
static CInfo g_info;
struct CMesh {
    long InterpolationSet; C3DVector m_positionStart, m_positionEnd;
    bool GetWorldPosition(C3DVector& position)
    {
        float t = g_info.InterpolationSetList[InterpolationSet].WFInterpolate;
        position = (m_positionEnd - m_positionStart) * t + m_positionStart;
        return true;
    }
};
int main()
{
    g_info.InterpolationSetList[0].WFInterpolate = 0.0f; g_info.InterpolationSetList[2].WFInterpolate = 0.25f;
    CMesh m; m.InterpolationSet = 2; m.m_positionStart = C3DVector(0.0f, 4.0f, 8.0f); m.m_positionEnd = C3DVector(4.0f, 8.0f, 0.0f);
    C3DVector p(0.0f, 0.0f, 0.0f);
    if (!m.GetWorldPosition(p) || p.X != 1.0f || p.Y != 5.0f || p.Z != 6.0f) { std::printf("00bceb40_TEST FAIL\n"); return 1; }
    m.InterpolationSet = 0; m.GetWorldPosition(p);
    if (p.X != 0.0f || p.Y != 4.0f || p.Z != 8.0f) { std::printf("00bceb40_TEST FAIL\n"); return 1; }
    std::printf("00bceb40_TEST PASS\n");
    return 0;
}
