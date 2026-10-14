#include <cstdio>
static int g_base = 0;
struct COcclusionBase_00c20390 { virtual void v00() { } unsigned char _pad[0xc]; COcclusionBase_00c20390() { ++g_base; } };
struct SFloatRange_00c1bd20 { float m_min; float m_max; SFloatRange_00c1bd20(float a, float b) : m_min(a), m_max(b) { } };
struct CEnginePrimitiveOcclusionManager : COcclusionBase_00c20390 { long m_generation; SFloatRange_00c1bd20 m_distanceRange; SFloatRange_00c1bd20 m_fadeRange; long m_a; long m_b; float m_factor0, m_factor1, m_factor2, m_factor3; short m_countA; short m_countB;
    CEnginePrimitiveOcclusionManager() : COcclusionBase_00c20390(), m_generation(0), m_distanceRange(1.0f, 10.0f), m_fadeRange(0.0f, 0.0f), m_a(0), m_b(0), m_factor0(1.0f), m_factor1(1.0f), m_factor2(1.0f), m_factor3(1.0f), m_countA(0), m_countB(0) { } };
int main() {
    CEnginePrimitiveOcclusionManager m; if (g_base != 1 || m.m_generation || m.m_distanceRange.m_min != 1.0f || m.m_distanceRange.m_max != 10.0f || m.m_fadeRange.m_min != 0.0f || m.m_fadeRange.m_max != 0.0f || m.m_a || m.m_b || m.m_factor0 != 1.0f || m.m_factor3 != 1.0f || m.m_countA || m.m_countB) { std::printf("00c34320_TEST FAIL\n"); return 1; }
    std::printf("00c34320_TEST PASS\n"); return 0;
}