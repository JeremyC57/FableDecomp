// Behaviour test for 007bb800: body copied from src/007bb800.cpp (CTCDrunkenness::CTCDrunkenness), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CDefClassBase;
class CSpecialAbilitiesDrunkennessDef;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};
static int g_destroyed = 0;
class CDefClassBase { public:
    virtual void v00() {}
    virtual void DestroySelf() { ++g_destroyed; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
    bool GetSubDef(struct CDrunkennessDefPointer* out);
};
class CSpecialAbilitiesDrunkennessDef { public:
    virtual void v00() {}
    virtual void DestroySelf() { ++g_destroyed; }
    long          RefCount;
    unsigned char _pad08[0x28 - 0x08];
    float         DrunkennessThreshold;
    void Release() { if (--RefCount == 0) DestroySelf(); }
};
struct CDrunkennessDefPointer {
    CSpecialAbilitiesDrunkennessDef* Object;
    CDrunkennessDefPointer() : Object(0) { }
    ~CDrunkennessDefPointer() { Object->Release(); }
    CSpecialAbilitiesDrunkennessDef* operator->() const { return Object; }
};
static CSpecialAbilitiesDrunkennessDef* g_abilityDef = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
bool CDefClassBase::GetSubDef(CDrunkennessDefPointer* out)
{
    ++g_getSubDef; g_countAtGet = RefCount;
    ++g_abilityDef->RefCount; out->Object = g_abilityDef; return true;
}
class CThing { public:
    unsigned char             _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
};
long g_FrameRate = 50;
static float g_random = 0.25f; static int g_randomCalls = 0; static float g_randomRange = 0; static unsigned long* g_randomSeed = 0;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { ++g_randomCalls; g_randomRange = range; g_randomSeed = seed; return g_random; }
static unsigned long g_seed = 77; static int g_baseCtor = 0; static CThing* g_baseThing = 0;
class CTCBase { public:
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; g_baseThing = thing; }
    unsigned long* GetPLocalSeed() const { return &g_seed; }
};
class CTCDrunkenness : public CTCBase { public:
    bool  Drunk;
    float Drunkenness;
    long  FrameLastUpdatedSobering;
    float SecondsPerFrame;
    float ThisDrunkennessThreshold;
    virtual void v00() {}
    CTCDrunkenness(CThing* thing);
};

CTCDrunkenness::CTCDrunkenness(CThing* thing)
    : CTCBase(thing)
    , FrameLastUpdatedSobering(0)
    , SecondsPerFrame(1.0f / g_FrameRate)
{
    CDrunkennessDefPointer def;
    ParentThing->GetDef()->GetSubDef(&def);
    float factor = GFFloatRandom(1.0f, GetPLocalSeed()) + 0.5f;
    ThisDrunkennessThreshold = factor * def->DrunkennessThreshold;
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CSpecialAbilitiesDrunkennessDef ability;
    thing.PDef.Object = &classDef; classDef.RefCount = 1; ability.RefCount = 1; ability.DrunkennessThreshold = 8.0f; g_abilityDef = &ability;
    {
        CTCDrunkenness d(&thing);
        CHECK(g_baseCtor == 1 && g_baseThing == &thing && d.ParentThing == &thing);
        CHECK(d.FrameLastUpdatedSobering == 0 && d.SecondsPerFrame == 1.0f / 50);
        CHECK(g_getSubDef == 1 && g_countAtGet == 2 && classDef.RefCount == 1);
        CHECK(g_randomCalls == 1 && g_randomRange == 1.0f && g_randomSeed == &g_seed);
        CHECK(d.ThisDrunkennessThreshold == (0.25f + 0.5f) * 8.0f);
        CHECK(ability.RefCount == 1 && g_destroyed == 0);
    }
    // another frame rate and random draw; defs held only by the lookup are destroyed
    g_FrameRate = 25; g_random = 1.0f; classDef.RefCount = 0; ability.RefCount = 0;
    { CTCDrunkenness d(&thing); CHECK(d.SecondsPerFrame == 1.0f / 25 && d.ThisDrunkennessThreshold == 12.0f); }
    CHECK(g_destroyed == 2);
    if (fails == 0) printf("007bb800_TEST PASS\n"); else printf("007bb800_TEST FAIL\n");
    return fails;
}