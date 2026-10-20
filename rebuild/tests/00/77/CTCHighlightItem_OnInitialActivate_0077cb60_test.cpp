// Behaviour test for 0077cb60: body copied from src/0077cb60.cpp (CTCHighlightItem::OnInitialActivate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector { float X, Y, Z; };
static int g_destroyed = 0;
struct CHighlightItemDef {
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
template <class T> struct CDefPointer {
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
static C3DVector g_pos = { 4.0f, 5.0f, 6.0f };
struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefPointerMember<CThingDef> PDef;
    const C3DVector& GetPosition() const { return g_pos; }
    void Kill(bool immediately) {}
    void UpdateGraphicAppearance(long reason) {}
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags) { return 0; }
static bool g_cameraIdle = true; static int g_idleCalls = 0;
struct CGameSubsystem {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual bool IsCameraIdle() { ++g_idleCalls; return g_cameraIdle; }
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
    void OnInitialActivate();
};
void CTCHighlightItem::OnInitialActivate()
{
    CThingDef* thingDef = ParentThing->PDef.operator->();
    CDefPointer<const CHighlightItemDef> pdef;
    thingDef->GetSubDef(&pdef);
    const CHighlightItemDef* def = pdef.Object;
    if (def) {
        StartPos = ParentThing->GetPosition();
        if (def->WaitForCameraIdle && !GetGameSubsystem()->IsCameraIdle()) {
            StartRunning = true;
            LifeTimer = 0;
        }
        if (--((CHighlightItemDef*)def)->RefCount == 0)
            ((CHighlightItemDef*)def)->Destroy();
    }
}
static void Reset(CTCHighlightItem& tc) { tc.StartRunning = false; tc.LifeTimer = 77; tc.StartPos.X = tc.StartPos.Y = tc.StartPos.Z = 0.0f; g_idleCalls = 0; }
int main()
{
    static CThing thing; static CThingDef thingDef; static CHighlightItemDef def;
    memset(&thing, 0, sizeof(thing)); thing.PDef.Object = &thingDef; def.RefCount = 1; def.WaitForCameraIdle = true; g_def = &def;
    CTCHighlightItem tc; memset(&tc, 0, sizeof(tc)); tc.ParentThing = &thing;
    // 1: waits for the camera and it is busy: start running from frame 0
    Reset(tc); g_cameraIdle = false; tc.OnInitialActivate();
    CHECK(g_getSubDef == 1); CHECK(tc.StartPos.X == 4.0f && tc.StartPos.Y == 5.0f && tc.StartPos.Z == 6.0f); CHECK(tc.StartRunning && tc.LifeTimer == 0); CHECK(g_idleCalls == 1); CHECK(def.RefCount == 1 && g_destroyed == 0);
    // 2: camera idle: no start
    Reset(tc); g_cameraIdle = true; tc.OnInitialActivate(); CHECK(!tc.StartRunning && tc.LifeTimer == 77); CHECK(g_idleCalls == 1);
    // 3: not waiting for the camera: the camera is never asked
    Reset(tc); def.WaitForCameraIdle = false; g_cameraIdle = false; tc.OnInitialActivate(); CHECK(!tc.StartRunning && g_idleCalls == 0); CHECK(tc.StartPos.X == 4.0f);
    // 4: a def held only by the lookup is destroyed
    def.RefCount = 0; tc.OnInitialActivate(); CHECK(g_destroyed == 1);
    // 5: no sub-def: nothing touched
    Reset(tc); g_def = 0; tc.OnInitialActivate(); CHECK(tc.StartPos.X == 0.0f && tc.LifeTimer == 77);
    if (fails == 0) printf("0077cb60_TEST PASS\n"); else printf("0077cb60_TEST FAIL\n");
    return fails;
}