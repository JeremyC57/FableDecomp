// Behaviour test for 0077cca0: body copied from src/0077cca0.cpp (CTCHighlightItem::DoStopRunning), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public: float X, Y, Z; };
static int g_destroyed = 0;
class CHighlightItemDef { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long          RefCount;
    unsigned char _pad_0x08[0x24];
    long          EndEffectDef;
    unsigned char _pad_0x30[0x10];
    bool          WaitForCameraIdle;
    unsigned char _pad_0x41[0x3];
    float         StartDelaySeconds;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
};
static CHighlightItemDef* g_def = 0; static int g_getSubDef = 0;
struct CThingDef {
    bool GetSubDef(CDefPointer<const CHighlightItemDef>* out) const;
};
bool CThingDef::GetSubDef(CDefPointer<const CHighlightItemDef>* out) const
{
    ++g_getSubDef;
    if (!g_def) return false;
    ++g_def->RefCount; out->Object = g_def; return true;
}
template <class T> struct CDefPointerMember {
    T* Object;
    T* operator->() const { return Object; }
};
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f }; static int g_killed = 0; static bool g_killArg = false;
class CThing { public:
    unsigned char _pad_0x00[0x70];
    CDefPointerMember<CThingDef> PDef;
    const C3DVector& GetPosition() const { return g_pos; }
    void Kill(bool immediately) { ++g_killed; g_killArg = immediately; }
    void UpdateGraphicAppearance(long reason) {}
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
static int g_created = 0; static long g_createdDef = 0; static const C3DVector* g_createdPos = 0; static long g_createdFlags = -1;
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags) { ++g_created; g_createdDef = defIndex; g_createdPos = &position; g_createdFlags = flags; return 0; }
struct CGameSubsystem {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual bool IsCameraIdle() { return true; }
};
static CGameSubsystem g_subsystem;
long g_GameFramesPerSecond = 30;
struct CTCHighlightItem {
    void*         __vftable;
    CThing*       ParentThing;
    bool          ToBeRemoved;
    bool          IsInGlobalUpdateTCs;
    unsigned char _pad_0x0a[0x2];
    bool          StartRunning;
    bool          Running;
    long          LifeTimer;
    C3DVector     StartPos;
    CBaseIntelligentPointer PParticleEmitter;
    CGameSubsystem* GetGameSubsystem() const { return &g_subsystem; }
    void DoStartRunning() {}
    void DoMovement() {}
    void DoStopRunning();
};
void CTCHighlightItem::DoStopRunning()
{
    if (Running) {
        if (PParticleEmitter.GetPItem())
            PParticleEmitter.GetPItem()->Kill(true);
        CThingDef* thingDef = ParentThing->PDef.operator->();
        CDefPointer<const CHighlightItemDef> pdef;
        thingDef->GetSubDef(&pdef);
        const CHighlightItemDef* def = pdef.Object;
        if (def->EndEffectDef)
            CreateThing(def->EndEffectDef, ParentThing->GetPosition(), 0);
        Running = false;
        if (--((CHighlightItemDef*)def)->RefCount == 0)
            ((CHighlightItemDef*)def)->Destroy();
    }
}
int main()
{
    static CThing thing, emitter; static CThingDef thingDef; static CHighlightItemDef def;
    memset(&thing, 0, sizeof(thing)); thing.PDef.Object = &thingDef; def.RefCount = 1; def.EndEffectDef = 41; g_def = &def;
    CTCHighlightItem tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing;
    // 1: not running: nothing
    tc.Running = false; tc.DoStopRunning(); CHECK(g_getSubDef == 0 && g_killed == 0);
    // 2: running with an emitter and an end effect
    tc.Running = true; tc.PParticleEmitter.m_pItem = &emitter; tc.DoStopRunning();
    CHECK(g_killed == 1 && g_killArg); CHECK(g_getSubDef == 1); CHECK(g_created == 1 && g_createdDef == 41 && g_createdPos == &g_pos && g_createdFlags == 0);
    CHECK(!tc.Running); CHECK(def.RefCount == 1 && g_destroyed == 0);
    // 3: no emitter, no end effect, def held only by the lookup
    tc.Running = true; tc.PParticleEmitter.m_pItem = 0; def.EndEffectDef = 0; def.RefCount = 0; tc.DoStopRunning();
    CHECK(g_killed == 1 && g_created == 1 && !tc.Running && g_destroyed == 1);
    if (fails == 0) printf("0077cca0_TEST PASS\n"); else printf("0077cca0_TEST FAIL\n");
    return fails;
}