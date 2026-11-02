// Behaviour test for 0x007B0300 ~CTCFishingSpot: RemoveParticles runs first (spot disabled), then PParticleFish, PParticle2,
// PParticle1 are destroyed, then ~CTCBase.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CTCBase { public: virtual ~CTCBase() { Log("B"); } void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(const char* t) : Tag(t), Item(1) { } virtual ~CBaseIntelligentPointer() { Log(Tag); } const char* Tag; int Item; };
class CTCFishingSpot : public CTCBase {
public:
    CTCFishingSpot() : PParticle1("1"), PParticle2("2"), PParticleFish("F"), Enabled(true) { }
    virtual ~CTCFishingSpot() { RemoveParticles(); }
    void RemoveParticles() { PParticle1.Item = 0; PParticle2.Item = 0; PParticleFish.Item = 0; Enabled = false; Log("X"); }
    CBaseIntelligentPointer PParticle1;
    CBaseIntelligentPointer PParticle2;
    CBaseIntelligentPointer PParticleFish;
    bool                    Enabled;
};
int main()
{
    g_log[0] = 0;
    CTCBase* spot = new CTCFishingSpot;
    delete spot;
    if (std::strcmp(g_log, "XF21B") != 0) { std::printf("007b0300_TEST FAIL\n"); return 1; }
    std::printf("007b0300_TEST PASS\n");
    return 0;
}