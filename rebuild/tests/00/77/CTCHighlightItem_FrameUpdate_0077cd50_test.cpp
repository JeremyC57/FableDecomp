// Behaviour test for 0077cd50: src/0077cd50.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHighlightItem::FrameUpdate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHighlightItem::FrameUpdate() -- while waiting, starts running once the def's delay has elapsed; while running, moves; counts frames either way
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077CD50, 117 bytes (+11 int3); default /O2 TU, no frame.
// Catalog label "CTCHighlightItem::vfunc_9" (the CTCBase FrameUpdate slot).
// Callees: CThingDef::GetSubDef<CHighlightItemDef> @ 0x0077D080, DoStartRunning() @ 0x0077CBE0,
//          CThing::UpdateGraphicAppearance(long) @ 0x004C7590 (15), DoMovement() @ 0x0077C960.
// Globals: the frames-per-second long @ 0x01375550.
// Shape: `(float)LifeTimer > def->StartDelaySeconds * fps` (`fild timer ; fild fps ; fmul [def+44h] ; fcompp ;
// test ah,5 ; jp`); the running arm has its own `inc ; ret` tail.
// Layout: CTCHighlightItem (PDB, matches retail): StartRunning +0x0C, Running +0x0D, LifeTimer +0x10, StartPos +0x14,
// PParticleEmitter +0x20 (CIntelligentPointer<CThing>); CTCBase ParentThing +0x04; retail CThing def object +0x70.
// The def pointer is read once into edi after GetSubDef and released by hand (`dec [edi+4] ; jne ; call [vtbl+4]`).

class C3DVector { public:
    float X, Y, Z;
};

class CHighlightItemDef { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04): called when the last reference goes
    long          RefCount;                          // +0x04
    unsigned char _pad_0x08[0x24];
    long          EndEffectDef;                      // +0x2C (0 = none)
    unsigned char _pad_0x30[0x10];
    bool          WaitForCameraIdle;                 // +0x40
    unsigned char _pad_0x41[0x3];
    float         StartDelaySeconds;                 // +0x44
};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() {}
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CThingDef {
    bool GetSubDefRaw(CDefPointer<const CHighlightItemDef>* out) const;   // 0x0077D080
    __forceinline bool GetSubDef(CDefPointer<const CHighlightItemDef>* out) const { out->Object = 0; return GetSubDefRaw(out); }
};
void* FindDef_CThingDef_GetSubDefRaw_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDefRaw(CDefPointer<const CHighlightItemDef>* out) const
{
    void* def = FindDef_CThingDef_GetSubDefRaw_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

template <class T> struct CDefPointerMember {
    T* Object;
    T* operator->() const { return Object; }
};

class CThing { public:
    unsigned char _pad_0x00[0x70];
    CDefPointerMember<CThingDef> PDef;               // +0x70 CDefPointer<const CThingDef>
    const C3DVector& GetPosition() const;            // 0x004C73D0
    void Kill(bool immediately);                     // 0x004C9B80
    void UpdateGraphicAppearance(long reason);       // 0x004C7590
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};

CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CGameSubsystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsCameraIdle();                     // slot 9 (+0x24)
};

extern long g_GameFramesPerSecond;                   // 0x01375550

struct CTCHighlightItem {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    bool          ToBeRemoved;                       // +0x08
    bool          IsInGlobalUpdateTCs;               // +0x09
    unsigned char _pad_0x0a[0x2];
    bool          StartRunning;                      // +0x0C
    bool          Running;                           // +0x0D
    long          LifeTimer;                         // +0x10
    C3DVector     StartPos;                          // +0x14
    CBaseIntelligentPointer PParticleEmitter;        // +0x20 CIntelligentPointer<CThing>

    CGameSubsystem* GetGameSubsystem() const;        // 0x00686D60 (CTCBase: main game component +0x1C)
    void DoStartRunning();                           // 0x0077CBE0
    void DoMovement();                               // 0x0077C960
    void OnInitialActivate();
    void DoStopRunning();
    void FrameUpdate();
};

void CTCHighlightItem::FrameUpdate()
{
    if (StartRunning) {
        CThingDef* thingDef = ParentThing->PDef.operator->();
        CDefPointer<const CHighlightItemDef> pdef;
        thingDef->GetSubDef(&pdef);
        const CHighlightItemDef* def = pdef.Object;
        if (LifeTimer > def->StartDelaySeconds * g_GameFramesPerSecond)
            DoStartRunning();
        ParentThing->UpdateGraphicAppearance(15);
        ++LifeTimer;
        CHighlightItemDef* release = (CHighlightItemDef*)def;
        if (DecRefToZero(release->RefCount))
            release->Destroy();
    } else if (Running) {
        DoMovement();
        ++LifeTimer;
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0, g_start = 0, g_move = 0; static long g_reason = 0; static int g_update = 0;
void* FindDef_CThingDef_GetSubDefRaw_0() { return g_def; }
void CHighlightItemDef::v00() {}
void CHighlightItemDef::Destroy() { ++g_destroyed; }
void CBaseIntelligentPointer::v00() {}
long g_GameFramesPerSecond = 10;
void CThing::UpdateGraphicAppearance(long reason) { g_reason = reason; ++g_update; }
void CTCHighlightItem::DoStartRunning() { ++g_start; }
void CTCHighlightItem::DoMovement() { ++g_move; }
int main()
{
    CHighlightItemDef def; def.RefCount = 10; def.StartDelaySeconds = 1.5f; g_def = &def;
    CThingDef td;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.PDef.Object = &td;
    CTCHighlightItem h; memset(&h, 0, sizeof(h)); h.ParentThing = &thing;
    h.StartRunning = true; h.LifeTimer = 15;
    h.FrameUpdate(); CHECK(g_start == 0); CHECK(g_update == 1); CHECK(g_reason == 15); CHECK(h.LifeTimer == 16); CHECK(def.RefCount == 9);
    h.FrameUpdate(); CHECK(g_start == 1); CHECK(h.LifeTimer == 17); CHECK(def.RefCount == 8);
    def.RefCount = 1; h.FrameUpdate(); CHECK(def.RefCount == 0); CHECK(g_destroyed == 1);
    h.StartRunning = false; h.Running = true; h.LifeTimer = 3; h.FrameUpdate(); CHECK(g_move == 1); CHECK(h.LifeTimer == 4); CHECK(g_update == 3);
    h.Running = false; h.FrameUpdate(); CHECK(g_move == 1); CHECK(h.LifeTimer == 4);
    if (fails == 0) printf("0077cd50_TEST PASS\n"); else printf("0077cd50_TEST FAIL\n");
    return fails;
}