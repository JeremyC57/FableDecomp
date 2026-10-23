#include <cstdio>
#include <new>
// Behaviour test for CTCTeleporter::CTCTeleporter (0x007DFEE0).
struct CTeleporterDef { int id; };
static CTeleporterDef g_def;
struct CDefPointer { CTeleporterDef* m_p; CDefPointer() : m_p(0) {} };
struct CDefClassBase {
    int calls; CDefPointer* lastOut;
    bool GetSubDef_007e0540(CDefPointer* out) { ++calls; lastOut = out; out->m_p = &g_def; return true; }
};
struct CThingDefs { CDefClassBase* m_pDefinition; };
class CThing : public CThingDefs {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    virtual ~CBaseIntelligentPointer() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() : CBaseIntelligentPointer() {}
};
struct CTCBase {
    virtual void v00() {}
    virtual ~CTCBase() {}
    CThingDefs* m_pParentThing;
    CTCBase(CThing* parent) : m_pParentThing(parent) {}
};
struct CTCTeleporter : CTCBase {
    CDefPointer PDef;
    bool Active;
    bool Activated;
    bool HeroInsideRadius;
    CIntelligentPointer PParticle;
    bool WaitingForTutorialToFinish;
    CTCTeleporter(CThing* parent);
    virtual void v00() {}
};
CTCTeleporter::CTCTeleporter(CThing* parent)
    : CTCBase(parent), Active(false), Activated(false), HeroInsideRadius(true)
{
    WaitingForTutorialToFinish = false;
    m_pParentThing->m_pDefinition->GetSubDef_007e0540(&PDef);
}
int main()
{
    CDefClassBase defs; defs.calls = 0; defs.lastOut = 0;
    CThing thing; thing.m_pDefinition = &defs;
    static unsigned char raw[sizeof(CTCTeleporter) + 8];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CTCTeleporter* t = new (raw) CTCTeleporter(&thing);
    if (t->m_pParentThing != &thing || t->Active || t->Activated || !t->HeroInsideRadius || t->WaitingForTutorialToFinish
        || t->PParticle.m_pItem != 0 || defs.calls != 1 || defs.lastOut != &t->PDef || t->PDef.m_p != &g_def)
    { std::printf("007dfee0_TEST FAIL\n"); return 1; }
    std::printf("007dfee0_TEST PASS\n");
    return 0;
}