#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
class CThing { public: int id; };
class CPlayer;
class CCoopSpiritDef;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { note("B"); }
};
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CDefPointer_CoopSpiritDef { const CCoopSpiritDef* Object; CDefPointer_CoopSpiritDef() : Object(0) { } };
class C3DVector { public: float X, Y, Z; };
class CTCCoopSpirit : public CTCBase_Calls { public:
    long NoMovementTimer; long Score; long OffscreenCounter; C3DVector MovementVector; CPlayer* Master;
    CDefPointer_CoopSpiritDef PCoopSpiritDef;
    CIntelligentPointer PParticleEmitter;
    virtual void v00() { }
    CTCCoopSpirit(CThing* thing) : CTCBase_Calls(thing), PCoopSpiritDef(), PParticleEmitter() { }
};
int main()
{
    CThing thing = { 3 };
    CTCCoopSpirit* c = new CTCCoopSpirit(&thing);
    if (std::strcmp(g_seq, "BI") || c->ParentThing != &thing || c->PCoopSpiritDef.Object != 0 || c->PParticleEmitter.m_pItem != 0
        || c->ToBeRemoved || c->IsInGlobalUpdateTCs) { std::printf("00670050_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00670050_TEST PASS\n");
    return 0;
}