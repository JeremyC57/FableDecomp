#include <cstdio>
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
struct CLightningOrbDefPointer {
    CDefBase* m_pDef;
    CLightningOrbDefPointer() : m_pDef(0) { }
    ~CLightningOrbDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } g_log[g_n++] = 28; }
};
struct CSplinePointer { ~CSplinePointer() { g_log[g_n++] = 44; } };
class CTCLightningOrb : public CTCBase { public:
    CIntelligentPointer<CThing> PParent; CIntelligentPointer<CThing> PTarget;
    CLightningOrbDefPointer PDef; long Timer, LifeTime, NumFramesForSplineTraversal; CSplinePointer PSpline;
    CTCLightningOrb() : PParent(12), PTarget(20), Timer(-1), LifeTime(-1), NumFramesForSplineTraversal(1) { }
};
int main()
{
    CDefBase def; def.m_count = 2; def.destroyed = 0;
    CTCLightningOrb* o = new CTCLightningOrb; o->PDef.m_pDef = &def;
    delete o;
    int expect[5] = { 44, 28, 20, 12, 100 };
    if (g_n != 5 || def.destroyed != 0 || def.m_count != 1) { std::printf("004dcedd_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (g_log[i] != expect[i]) { std::printf("004dcedd_TEST FAIL\n"); return 1; }
    std::printf("004dcedd_TEST PASS\n");
    return 0;
}