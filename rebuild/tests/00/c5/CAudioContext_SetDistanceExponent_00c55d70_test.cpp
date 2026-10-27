// Test for 0x00C55D70 CAudioContext::SetDistanceExponent: model 2 and factor 2^-n.
#include <cstdio>
#include <cmath>
struct CContext { float m_distanceFactor; int m_distanceModel;
    void SetDistanceExponent(int n) { m_distanceModel = 2; m_distanceFactor = (float)std::pow(2.0, -(double)n); } };
int main()
{
    CContext c; c.m_distanceModel = 0; c.m_distanceFactor = 0;
    c.SetDistanceExponent(3); bool ok = c.m_distanceModel == 2 && c.m_distanceFactor == 0.125f;
    c.SetDistanceExponent(-1); ok = ok && c.m_distanceFactor == 2.0f;
    if (!ok) { std::printf("00c55d70_TEST FAIL\n"); return 1; }
    std::printf("00c55d70_TEST PASS\n");
    return 0;
}