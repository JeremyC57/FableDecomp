#include <cstdio>
#include <cstdlib>
static int g_log[16]; static int g_n;
class CThing { public: int id; };
class CTCBase { public: CTCBase() { } virtual ~CTCBase() { g_log[g_n++] = 100; } };
struct CBaseIntelligentPointer { int m_tag; CBaseIntelligentPointer(int t) : m_tag(t) { } virtual ~CBaseIntelligentPointer() { g_log[g_n++] = m_tag; } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer(int t) : CBaseIntelligentPointer(t) { } };
struct CDefBase {
    long m_count; int destroyed;
    virtual ~CDefBase() { }
    virtual void Destroy() { ++destroyed; }
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CNymphDefPointer {
    CDefBase* m_pDef;
    CNymphDefPointer() : m_pDef(0) { }
    ~CNymphDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } g_log[g_n++] = 12; }
};
class C3DVector { public: float x, y, z; };
struct CSplinePointVector {
    C3DVector* _M_start;
    CSplinePointVector() : _M_start(0) { }
    ~CSplinePointVector() { if (_M_start) { std::free(_M_start); g_log[g_n++] = 44; } }
};
class CCardinalSpline { public: CSplinePointVector m_points; };
class CTCNymph : public CTCBase { public:
    CNymphDefPointer PDef; CCardinalSpline FlightSpline; bool FlyingAround;
    CIntelligentPointer<CThing> PEffect, POldEffect; long FadeCounter; CIntelligentPointer<CThing> PTarget;
    CTCNymph() : PEffect(65), POldEffect(73), PTarget(104) { }
};
int main()
{
    CDefBase def; def.m_count = 1; def.destroyed = 0;
    CTCNymph* n = new CTCNymph; n->PDef.m_pDef = &def; n->FlightSpline.m_points._M_start = (C3DVector*)std::malloc(sizeof(C3DVector) * 4);
    delete n;
    int expect[6] = { 104, 73, 65, 44, 12, 100 };
    if (g_n != 6 || def.destroyed != 1) { std::printf("004e33bb_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (g_log[i] != expect[i]) { std::printf("004e33bb_TEST FAIL\n"); return 1; }
    std::printf("004e33bb_TEST PASS\n");
    return 0;
}