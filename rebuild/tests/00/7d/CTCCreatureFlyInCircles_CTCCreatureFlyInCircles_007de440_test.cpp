#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x007DE440: spline points freed, PSwoopTarget then PBurninationEffect released, then ~CTCBase.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CTCBase { virtual void v00() {} void* ParentThing; ~CTCBase() { Log('B'); } };
struct CBaseIntelligentPointer { virtual void v00() {} char m_tag; ~CBaseIntelligentPointer() { Log(m_tag); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
struct C3DVector { float x, y, z; };
struct CCardinalSpline {
    C3DVector* m_pControlPoints; long m_count; long m_capacity; long m_reserved; float Tension; bool EndPointsConnected;
    ~CCardinalSpline() { if (m_pControlPoints) { Log('F'); std::free(m_pControlPoints); } }
};
struct CTCCreatureFlyInCircles : CTCBase {
    bool Active;
    CIntelligentPointer PBurninationEffect;
    bool HasBeenHit;
    CIntelligentPointer PSwoopTarget;
    CCardinalSpline SwoopSpline;
    virtual void v00() {}
    ~CTCCreatureFlyInCircles();
};
CTCCreatureFlyInCircles::~CTCCreatureFlyInCircles()
{
}
int main()
{
    {
        CTCCreatureFlyInCircles c; c.PBurninationEffect.m_tag = 'E'; c.PSwoopTarget.m_tag = 'S';
        c.SwoopSpline.m_pControlPoints = (C3DVector*)std::malloc(sizeof(C3DVector) * 3);
    }
    if (std::strcmp(g_log, "FSEB") != 0) { std::printf("007de440_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CTCCreatureFlyInCircles c; c.PBurninationEffect.m_tag = 'E'; c.PSwoopTarget.m_tag = 'S'; c.SwoopSpline.m_pControlPoints = 0; }
    if (std::strcmp(g_log, "SEB") != 0) { std::printf("007de440_TEST FAIL\n"); return 1; }
    std::printf("007de440_TEST PASS\n");
    return 0;
}