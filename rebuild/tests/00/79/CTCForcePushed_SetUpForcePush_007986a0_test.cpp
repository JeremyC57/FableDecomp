// Behaviour test for 007986a0: src/007986a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher) -- arm the push: remember where it started, how far and along which per-frame step it goes and who pushed; an AI creature being pushed has its animation suspended and its current action finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007986A0, length 202 bytes (+ int3 padding). __fastcall, `ret 0Ch`.
//         Ego_r.pdb: `?SetUpForcePush@CTCForcePushed@@QAEXABVC3DVector@@MAAVCThing@@@Z`.
// Callees: CThing::GetPosition @ 0x004C73D0; CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/
//          0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CThingAnimatable::SetSuspendAnimation(bool,
//          long) @ 0x00833A10; CThingCreatureBase::FinishCurrentAction @ 0x00662650.
// Shape: the two vector members are implicit C3DVector copies (destination through a `lea` register); the per-frame
//        distance is the step vector's length (x87: z, y, x loaded, then x*x + y*y + z*z and fsqrt); the pusher is the
//        converting temporary assigned through `SetPItem(GetPItem())`; the owner is read once into edi for the AI tail.
//        The length goes through a DOUBLE local narrowed into DistanceMovingPerFrame: retail moves the root under the
//        components (`fstp st(3)`), pops them, then stores; a direct float assignment stores before the pops.
// Layout: Ego_r.pdb CTCForcePushed (matches retail): PThingCausingForcePush +0x0C, PositionForcePushStarted +0x1C,
//         DistanceBeingPushed +0x28, DirectionBeingPushed +0x2C, DistanceMovingPerFrame +0x38, Initialised +0x3C,
//         StartedMoving +0x3D; retail CThing ThingClass +0x10 (1 = AI creature).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class CThing;
class CBaseObject;

class C3DVector {
public:
    float X, Y, Z;
    float GetLength() const
    {
        float z = Z, y = Y, x = X;
        float s = x * x;
        s += y * y;
        s += z * z;
        return (float)sqrt(s);
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};
class CThingCreatureBase : public CThingAnimatable {
public:
    void FinishCurrentAction();                     // 0x00662650
};

class CTCForcePushed {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CIntelligentPointer<CThing> PThingCausingForcePush;     // +0x0C
    CIntelligentPointer<CThing> PParticle;          // +0x14
    C3DVector PositionForcePushStarted;             // +0x1C
    float DistanceBeingPushed;                      // +0x28
    C3DVector DirectionBeingPushed;                 // +0x2C
    float DistanceMovingPerFrame;                   // +0x38
    bool Initialised;                               // +0x3C
    bool StartedMoving;                             // +0x3D
    void SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher);
};

void CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher)
{
    PositionForcePushStarted = ParentThing->GetPosition();
    DistanceBeingPushed = distance;
    DirectionBeingPushed = direction;
    double length = direction.GetLength();
    DistanceMovingPerFrame = (float)length;
    PThingCausingForcePush = CIntelligentPointer<CThing>(&pusher);
    CThing* owner = ParentThing;
    if (owner->ThingClass == 1) {
        ((CThingAnimatable*)owner)->SetSuspendAnimation(true, 0);
        ((CThingCreatureBase*)owner)->FinishCurrentAction();
    }
    StartedMoving = false;
    Initialised = true;
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
void CTCForcePushed::v00() {}
static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { Log('c'); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { Log('d'); }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; Log('s'); }
static C3DVector g_pos = { 10.0f, 20.0f, 30.0f };
const C3DVector& CThing::GetPosition() const { Log('p'); return g_pos; }
static bool g_suspend; static long g_reason; static int g_suspendCalls = 0, g_finishCalls = 0; static CThing* g_finishThis;
void CThingAnimatable::SetSuspendAnimation(bool suspend, long reason) { g_suspend = suspend; g_reason = reason; ++g_suspendCalls; Log('S'); }
void CThingCreatureBase::FinishCurrentAction() { ++g_finishCalls; g_finishThis = this; Log('F'); }
int main()
{
    static CThingCreatureBase owner; static CThing pusher;
    static unsigned char store[sizeof(CTCForcePushed)];
    CTCForcePushed& p = *(CTCForcePushed*)store;
    p.ParentThing = &owner; p.PThingCausingForcePush.m_pItem = 0; p.Initialised = false; p.StartedMoving = true;
    owner.ThingClass = 1;
    C3DVector dir = { 3.0f, 4.0f, 12.0f };
    p.SetUpForcePush(dir, 7.5f, pusher);
    CHECK(p.PositionForcePushStarted.X == 10.0f); CHECK(p.PositionForcePushStarted.Y == 20.0f); CHECK(p.PositionForcePushStarted.Z == 30.0f);
    CHECK(p.DistanceBeingPushed == 7.5f);
    CHECK(p.DirectionBeingPushed.X == 3.0f); CHECK(p.DirectionBeingPushed.Y == 4.0f); CHECK(p.DirectionBeingPushed.Z == 12.0f);
    CHECK(p.DistanceMovingPerFrame == 13.0f);
    CHECK(p.PThingCausingForcePush.m_pItem == &pusher);
    CHECK(g_suspendCalls == 1); CHECK(g_suspend == true); CHECK(g_reason == 0); CHECK(g_finishCalls == 1); CHECK(g_finishThis == &owner);
    CHECK(p.StartedMoving == false); CHECK(p.Initialised == true);
    CHECK(strcmp(g_log, "pcssdSF") == 0);
    // a non-AI owner: no animation suspension
    owner.ThingClass = 0; g_logn = 0; g_log[0] = 0;
    C3DVector dir2 = { 0.0f, 0.0f, 2.0f };
    p.SetUpForcePush(dir2, 1.0f, pusher);
    CHECK(p.DistanceMovingPerFrame == 2.0f); CHECK(g_suspendCalls == 1); CHECK(g_finishCalls == 1);
    CHECK(strcmp(g_log, "pcssd") == 0);
    if (fails == 0) printf("007986a0_TEST PASS\n"); else printf("007986a0_TEST FAIL\n");
    return fails;
}