// Behaviour test for 007445e0: body copied from src/007445e0.cpp (CTCPerceiveThing ctor), the CTCBase / intelligent-pointer callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_ipCtor = 0, g_setPItem = 0; static void* g_lastItem = (void*)1;
struct CThing {
    unsigned char _pad00[0x18];
    unsigned long m_uidLow;
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() { ++g_ipCtor; m_pItem = (CBaseObject*)0x77; }
    void SetPItem(CBaseObject* item) { ++g_setPItem; g_lastItem = item; m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
};
struct CTCBase {
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};
struct C3DVector { float X, Y, Z; };
struct CPerceivedThingDef;
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CTCPerceiveThing : CTCBase {
    CIntelligentPointer PerceivedThing;
    float     DistanceFromPerceivedThingSquared;
    float     AwarenessTimer;
    long      FramesSinceDirectlyPerceivedThing;
    C3DVector LastPerceivedPos;
    bool      Perceiving, LineOfSight, ExtendedLineOfSight, IgnoreFOVLineOfSight, DirectAwareness, Smelt, Heard, SearchingForThingToPerceive;
    CDefPointer<const CPerceivedThingDef> PDef;
    long      UpdateOffset;
    float     SoundRadius, SightRadius, ExtendedSightRadius, SmellRadius, GiveUpChaseRadius;
    bool      PerceptionCombat;
    long      FrameHitByPerceivedThing;
    virtual ~CTCPerceiveThing() {}
    CTCPerceiveThing(CThing* parent);
};
CTCPerceiveThing::CTCPerceiveThing(CThing* parent) : CTCBase(parent)
    , DistanceFromPerceivedThingSquared(0), AwarenessTimer(0), FramesSinceDirectlyPerceivedThing(-1)
    , Perceiving(false), LineOfSight(false), ExtendedLineOfSight(false), IgnoreFOVLineOfSight(false), DirectAwareness(false)
    , Smelt(false), Heard(false), SearchingForThingToPerceive(false), PDef()
    , SoundRadius(0), SightRadius(0), ExtendedSightRadius(0), SmellRadius(0), GiveUpChaseRadius(0)
    , PerceptionCombat(false), FrameHitByPerceivedThing(0)
{
    UpdateOffset = ParentThing->m_uidLow & 3;
}
int main()
{
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.m_uidLow = 0x1236;   // & 3 == 2
    unsigned char raw[sizeof(CTCPerceiveThing)]; memset(raw, 0xAA, sizeof(raw));
    CTCPerceiveThing* tc = new (raw) CTCPerceiveThing(&thing);
    CHECK(sizeof(CTCPerceiveThing) == 0x58);
    CHECK(g_baseCtor == 1 && tc->ParentThing == &thing);
    CHECK(g_ipCtor == 1 && g_setPItem == 1 && g_lastItem == 0 && tc->PerceivedThing.m_pItem == 0);
    CHECK(tc->DistanceFromPerceivedThingSquared == 0.0f && tc->AwarenessTimer == 0.0f);
    CHECK(tc->FramesSinceDirectlyPerceivedThing == -1);
    CHECK(*(unsigned long*)&tc->LastPerceivedPos.X == 0xAAAAAAAA);          // not initialised
    CHECK(!tc->Perceiving && !tc->LineOfSight && !tc->ExtendedLineOfSight && !tc->IgnoreFOVLineOfSight);
    CHECK(!tc->DirectAwareness && !tc->Smelt && !tc->Heard && !tc->SearchingForThingToPerceive);
    CHECK(tc->PDef.Object == 0);
    CHECK(tc->UpdateOffset == 2);
    CHECK(tc->SoundRadius == 0.0f && tc->SightRadius == 0.0f && tc->ExtendedSightRadius == 0.0f && tc->SmellRadius == 0.0f && tc->GiveUpChaseRadius == 0.0f);
    CHECK(!tc->PerceptionCombat && tc->FrameHitByPerceivedThing == 0);
    thing.m_uidLow = 7; CTCPerceiveThing* tc2 = new (raw) CTCPerceiveThing(&thing); CHECK(tc2->UpdateOffset == 3);
    if (fails == 0) printf("007445e0_TEST PASS\n"); else printf("007445e0_TEST FAIL\n");
    return fails;
}