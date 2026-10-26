// Behaviour test for 007446b0: the model below is a copy of src/007446b0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_adds = 0, g_removes = 0;
class CThing { public:};
class CBaseObject { public:};
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    virtual ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() const { ++g_gets; return m_pItem; }
    void SetPItem(CBaseObject* item) { ++g_sets; m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
};
static CThing* g_addedFor = 0; static CThing* g_removedFor = 0;
class CPlayerGui { public:
    void AddPerceivingThing(CThing* thing) { ++g_adds; g_addedFor = thing; }
    void RemovePerceivingThing(CThing* thing) { ++g_removes; g_removedFor = thing; }
};
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
class CDefPointeeBase;
class C3DVector { public: float X, Y, Z; };
class CTCPerceiveThing { public:
    void*     __vftable;
    CThing*   ParentThing;
    bool      ToBeRemoved;
    bool      IsInGlobalUpdateTCs;
    CIntelligentPointer PerceivedThing;
    float     DistanceFromPerceivedThingSquared;
    float     AwarenessTimer;
    long      FramesSinceDirectlyPerceivedThing;
    C3DVector LastPerceivedPos;
    bool      Perceiving;
    bool      LineOfSight;
    bool      ExtendedLineOfSight;
    bool      IgnoreFOVLineOfSight;
    bool      DirectAwareness;
    bool      Smelt;
    bool      Heard;
    bool      SearchingForThingToPerceive;
    CDefPointeeBase* PDef;
    long      UpdateOffset;
    float     SoundRadius;
    float     SightRadius;
    float     ExtendedSightRadius;
    float     SmellRadius;
    float     GiveUpChaseRadius;
    bool      PerceptionCombat;
    long      FrameHitByPerceivedThing;
    void UpdateDrawPerceivers()
    {
        if (PerceivedThing.GetPItem())
            g_pPlayerGui->AddPerceivingThing(ParentThing);
        else
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
};

void CTCPerceiveThing::SetPerceivedThing(const CThing* thing)
{
    {
        CIntelligentPointer newThing(thing);
        PerceivedThing = newThing;
    }
    UpdateDrawPerceivers();
}

static bool Run(bool withThing)
{
    static CThing parent, perceived;
    CTCPerceiveThing p; p.__vftable = 0; p.ParentThing = &parent;
    g_ctors = g_dtors = g_sets = g_gets = g_adds = g_removes = 0; g_addedFor = g_removedFor = 0;
    p.SetPerceivedThing(withThing ? &perceived : 0);
    if (p.PerceivedThing.m_pItem != (withThing ? (CBaseObject*)&perceived : 0)) return false;
    if (g_ctors != 1 || g_dtors != 1 || g_sets != 2 || g_gets != 2) return false;   // the temporary, copied, then the draw test
    if (withThing) { if (g_adds != 1 || g_removes != 0 || g_addedFor != &parent) return false; }
    else { if (g_adds != 0 || g_removes != 1 || g_removedFor != &parent) return false; }
    return true;
}
int main()
{
    bool ok = Run(true) && Run(false);
    if (!ok) { std::printf("007446b0_TEST FAIL\n"); return 1; }
    std::printf("007446b0_TEST PASS\n");
    return 0;
}