// Behaviour test for 00744ed0: body copied from src/00744ed0.cpp (CTCPerceiveThing::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
class CDefClassBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CThingDef;
struct CThingDefOut {
    CThingDef* Object;
    CThingDefOut() : Object(0) {}
};
class CPerceivedThingDef : public CDefClassBase { public:
    unsigned char _pad08[0x24];
    float GiveUpChaseRadius;
    float SightRadius;
    float ExtendedSightRadius;
    float SoundRadius;
    float SmellRadius;
};
static CPerceivedThingDef g_perceived; static int g_getSubDef = 0;
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CPerceivedThingDef>* out) { ++g_getSubDef; ++g_perceived.RefCount; out->Object = &g_perceived; return true; }
};
static CThingDef g_thingDef; static int g_getDef = 0; static long g_getDefIndex = -1;
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CThingDefOut* out) { ++g_getDef; g_getDefIndex = globalIndex; ++g_thingDef.RefCount; out->Object = &g_thingDef; return true; }
};
static CDefinitionManager g_manager;
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;
};
static int g_reset = 0;
class CTCPerceiveThing { public:
    void*     __vftable;
    CThing*   ParentThing;
    bool      ToBeRemoved;
    bool      IsInGlobalUpdateTCs;
    unsigned char _pad0a[0x26];
    bool      DirectAwareness;
    unsigned char _pad31[3];
    CDefPointer<CPerceivedThingDef> PDef;
    long      UpdateOffset;
    float     SoundRadius;
    float     SightRadius;
    float     ExtendedSightRadius;
    float     SmellRadius;
    float     GiveUpChaseRadius;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    const CPerceivedThingDef* GetDef() const { return PDef.Object; }
    void ResetPerceptionVariables() { ++g_reset; }
    void SetSightRadius(float radius)
    {
        if (radius != -1.0f) {
            SightRadius = radius;
            return;
        }
        SightRadius = GetDef()->SightRadius;
        if (SightRadius > ExtendedSightRadius)
            ExtendedSightRadius = SightRadius;
    }
    void SetSoundRadius(float radius)
    {
        if (radius != -1.0f) {
            SoundRadius = radius;
            return;
        }
        SoundRadius = GetDef()->SoundRadius;
    }
    void SetSmellRadius(float radius)
    {
        if (radius != -1.0f) {
            SmellRadius = radius;
            return;
        }
        SmellRadius = GetDef()->SmellRadius;
    }
    void SetExtendedSightRadius(float radius)
    {
        if (radius != -1.0f) {
            ExtendedSightRadius = radius;
            return;
        }
        ExtendedSightRadius = GetDef()->ExtendedSightRadius;
        if (ExtendedSightRadius < SightRadius)
            ExtendedSightRadius = SightRadius;
    }
    void OnCreate();
};
void CTCPerceiveThing::OnCreate()
{
    CThingDefOut thingDef;
    CThing* thing = ParentThing;
    GetDefinitionManager()->GetDef(thing->DefGlobalIndex, &thingDef);
    CThingDef* def = thingDef.Object;
    def->GetSubDef(&PDef);
    ResetPerceptionVariables();
    SetSightRadius(-1.0f);
    SetSoundRadius(-1.0f);
    SetSmellRadius(-1.0f);
    SetExtendedSightRadius(-1.0f);
    GiveUpChaseRadius = GetDef()->GiveUpChaseRadius;
    DirectAwareness = false;
    if (def && --def->RefCount == 0)
        def->Destroy();
}
int main()
{
    static CThing thing; thing.DefGlobalIndex = 0x3e8; g_thingDef.RefCount = 1; g_perceived.RefCount = 1;
    g_perceived.GiveUpChaseRadius = 50; g_perceived.SightRadius = 20; g_perceived.ExtendedSightRadius = 10; g_perceived.SoundRadius = 15; g_perceived.SmellRadius = 5;
    {
        CTCPerceiveThing tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing; tc.DirectAwareness = true; tc.PDef.Object = 0;
        tc.OnCreate();
        CHECK(g_getDef == 1 && g_getDefIndex == 0x3e8 && g_getSubDef == 1 && g_reset == 1);
        CHECK(tc.PDef.Object == &g_perceived && g_perceived.RefCount == 2);
        // sight 20 raises the extended sight (10) to 20; extended then stays 20
        CHECK(tc.SightRadius == 20 && tc.ExtendedSightRadius == 20 && tc.SoundRadius == 15 && tc.SmellRadius == 5 && tc.GiveUpChaseRadius == 50);
        CHECK(!tc.DirectAwareness); CHECK(g_thingDef.RefCount == 1 && g_destroyed == 0);
    }
    CHECK(g_perceived.RefCount == 1);
    // extended sight larger than sight is kept
    g_perceived.SightRadius = 4; g_perceived.ExtendedSightRadius = 30;
    { CTCPerceiveThing tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing; tc.OnCreate(); CHECK(tc.SightRadius == 4 && tc.ExtendedSightRadius == 30); }
    // a thing def held only by the lookup is destroyed
    g_thingDef.RefCount = 0; { CTCPerceiveThing tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing; tc.OnCreate(); } CHECK(g_destroyed == 1);
    if (fails == 0) printf("00744ed0_TEST PASS\n"); else printf("00744ed0_TEST FAIL\n");
    return fails;
}