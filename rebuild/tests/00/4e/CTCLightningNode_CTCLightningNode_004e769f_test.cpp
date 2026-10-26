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
struct CLightningDefPointer {
    CDefBase* m_pDef;
    CLightningDefPointer() : m_pDef(0) { }
    ~CLightningDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } g_log[g_n++] = 24; }
};
struct CLightningBranchVector { ~CLightningBranchVector() { g_log[g_n++] = 44; } };
class CTCLightningNode : public CTCBase { public:
    CIntelligentPointer<CThing> PParent; CIntelligentPointer<const CThing> PSourceOfFaction; CIntelligentPointer<CThing> PSourceOfHit;
    CLightningDefPointer PDef; long Timer; CLightningBranchVector LightningBranches;
    CTCLightningNode() : PParent(12), PSourceOfFaction(20), PSourceOfHit(28), Timer(0) { }
};
int main()
{
    CDefBase def; def.m_count = 1; def.destroyed = 0;
    CTCLightningNode* n = new CTCLightningNode; n->PDef.m_pDef = &def;
    delete n;
    int expect[6] = { 44, 24, 28, 20, 12, 100 };
    if (g_n != 6 || def.destroyed != 1 || def.m_count != 0) { std::printf("004e769f_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (g_log[i] != expect[i]) { std::printf("004e769f_TEST FAIL\n"); return 1; }
    std::printf("004e769f_TEST PASS\n");
    return 0;
}