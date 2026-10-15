#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CAnimationEntry { int id; };
struct CBaseObject { int id; };
struct CCharString {
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
};
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CAnimationEntry g_anim;
static const char* g_lookedUpName = 0;
static CThingCreatureBase* g_lookedUpOwner = 0;
CAnimationEntry* FASTCALL GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name)
{
    g_lookedUpOwner = owner; g_lookedUpName = name.m_text; return &g_anim;
}
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct SBaseArgs { CThingCreatureBase* owner; float a; float b; CAnimationEntry* anim; long first; long last; long c; long d; };
static SBaseArgs g_base;
struct CCreatureActionBase {
    virtual ~CCreatureActionBase() { }
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d)
    {
        g_base.owner = owner; g_base.a = a; g_base.b = b; g_base.anim = animation;
        g_base.first = sound.FirstSound; g_base.last = sound.LastSound; g_base.c = c; g_base.d = d;
    }
};
struct CCreatureAction_AlertAllNearbyToThing : CCreatureActionBase {
    long m_thingToAlertTo;
    long m_alertRadius;
    CCreatureAction_AlertAllNearbyToThing(CThingCreatureBase* owner, long thingToAlertTo, long alertRadius);
};
CCreatureAction_AlertAllNearbyToThing::CCreatureAction_AlertAllNearbyToThing(CThingCreatureBase* owner, long thingToAlertTo, long alertRadius)
    : CCreatureActionBase(owner, 0.0f, 0.0f, GetRandomLoadedAnimation(owner, "ALARM"), CSoundPair(), 0, 1)
    , m_thingToAlertTo(thingToAlertTo)
    , m_alertRadius(alertRadius)
{
}
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CThing thing; thing.id = 9;
    (void)thing;
    g_base.first = 5; g_base.last = 5;
    CCreatureAction_AlertAllNearbyToThing action(&owner, 1234, 56);
    bool ok = g_base.owner == &owner && g_base.a == 0.0f && g_base.b == 0.0f && g_base.anim == &g_anim
        && g_base.first == 0 && g_base.last == 0 && g_base.c == 0 && g_base.d == 1
        && g_lookedUpOwner == &owner && std::strcmp(g_lookedUpName, "ALARM") == 0 && action.m_thingToAlertTo == 1234 && action.m_alertRadius == 56;
    if (!ok) { std::printf("00842370_TEST FAIL\n"); return 1; }
    std::printf("00842370_TEST PASS\n");
    return 0;
}
