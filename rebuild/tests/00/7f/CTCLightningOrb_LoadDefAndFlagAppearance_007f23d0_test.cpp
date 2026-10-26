#include <cstdio>
static int g_orbDef;
struct CLightningOrbDefPointer { const void* m_pDef; };
class CThingBaseDef { public:
    long m_count; int destroyed; int lookups;
    virtual ~CThingBaseDef() { }
    virtual void Destroy() { ++destroyed; }
    void AddRef() { ++m_count; }
    void Release() { if (--m_count == 0) Destroy(); }
    bool GetSubDef(CLightningOrbDefPointer& out) { ++lookups; out.m_pDef = &g_orbDef; return m_count == 2; }
};
struct CThingBaseDefPointer {
    CThingBaseDef* m_pDef;
    CThingBaseDefPointer(CThingBaseDef* p) : m_pDef(p) { }
    CThingBaseDefPointer(const CThingBaseDefPointer& o) : m_pDef(o.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CThingBaseDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } }
    CThingBaseDef* operator->() const { return m_pDef; }
};
struct CTCGraphicAppearance { unsigned char m_updateFlags; };
class CThing { public: CTCGraphicAppearance* PTCGraphicAppearance; CThingBaseDefPointer PBaseDef; CThing(CThingBaseDef* d) : PBaseDef(d) { } };
class CTCLightningOrb { public:
    CThing* m_pThing; CLightningOrbDefPointer PDef;
    void LoadDefAndFlagAppearance()
    {
        { CThingBaseDefPointer def = m_pThing->PBaseDef; def->GetSubDef(PDef); }
        m_pThing->PTCGraphicAppearance->m_updateFlags |= 2;
    }
};
int main()
{
    CThingBaseDef d; d.m_count = 1; d.destroyed = 0; d.lookups = 0;
    CTCGraphicAppearance app; app.m_updateFlags = 0x81;
    CThing* t = new CThing(&d); t->PTCGraphicAppearance = &app;
    CTCLightningOrb o; o.m_pThing = t; o.PDef.m_pDef = 0;
    o.LoadDefAndFlagAppearance();
    if (d.lookups != 1 || d.m_count != 1 || d.destroyed != 0 || o.PDef.m_pDef != &g_orbDef || app.m_updateFlags != 0x83) { std::printf("007f23d0_TEST FAIL\n"); return 1; }
    t->PBaseDef.m_pDef = 0; delete t;
    std::printf("007f23d0_TEST PASS\n");
    return 0;
}