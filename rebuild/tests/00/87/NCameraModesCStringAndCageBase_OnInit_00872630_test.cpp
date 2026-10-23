#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CStringAndCageBase::OnInit: placement behind the thing with no previous mode, inheritance from a previous string-and-cage mode (expansion timer), desired values without smoothing, measured lengths with smoothing, and the next auto-go-behind frame


#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CThingPhysics {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {}
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {}
    virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {}
    virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {}
    virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {}
    virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {}
    virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {}
    virtual C3DVector GetFacingVector() const;               // slot 70 (+0x118)
};
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float t) const;              // 0x004C12D0
};
class CThing {
public:
    unsigned char         _pad00[0x60];
    CThingPhysics*        m_pPhysics;                        // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;              // +0x64
    const C3DVector& GetPosition() const;                    // 0x004C73D0
};
class CThingGameObject : public CThing { };
extern long g_framesPerSecond;                               // 0x01375550
long __fastcall GetWorldFrame();                             // 0x0049D870
namespace NCameraModes {
class CBase;
}
struct CGameCameraManager {
    unsigned char        _pad000[0x98];
    NCameraModes::CBase* CurrentMode;                        // +0x98
    unsigned char        _pad09c[0xb0 - 0x9c];
    bool                 NoSmoothing;                        // +0xb0
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {}
    virtual bool IsStringAndCage() const;                    // slot 14 (+0x38)
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {}
    virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {} virtual void v25() {}
    virtual void ResetBase();                                // slot 26 (+0x68)
    virtual void v27() {} virtual void v28() {}
    virtual float GetAutoGoBehindTime() const;               // slot 29 (+0x74)
    unsigned char       _pad04[0x10 - 0x04];
    C3DVector           LookAt;                              // +0x10
    unsigned char       _pad1c[0x40 - 0x1c];
    CGameCameraManager* CameraManager;                       // +0x40
    unsigned char       _pad44[0x54 - 0x44];
    C3DVector           Position;                            // +0x54
    unsigned char       _pad60[0xa4 - 0x60];
    const CThingGameObject* GetObservedThing() const;        // 0x0084F1B0
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                               // +0xa4
    float DesiredStringLength;                               // +0xa8
    float DesiredCageRadius;                                 // +0xac
    float CurrentHeightOffset;                               // +0xb0
    float CurrentStringLength;                               // +0xb4
    float CurrentCageRadius;                                 // +0xb8
    long  NextAutoGoBehindWF;                                // +0xbc
    long  TimerForExpansion;                                 // +0xc0
    void OnInit();
};
void CStringAndCageBase::OnInit()
{
    ResetBase();
    float thingZ = GetObservedThing()->GetPosition().Z;
    CBase* current = CameraManager->CurrentMode;
    if (current) {
        if (current->IsStringAndCage()) {
            CStringAndCageBase* previous = static_cast<CStringAndCageBase*>(current);
            if (DesiredStringLength > previous->DesiredStringLength) {
                TimerForExpansion = (long)(g_framesPerSecond * 2.0f);
                CurrentCageRadius = CurrentStringLength;
            }
            CurrentHeightOffset = previous->CurrentHeightOffset;
            CurrentStringLength = previous->CurrentStringLength;
            CurrentCageRadius = previous->CurrentCageRadius;
        } else {
            C3DVector goTo = Position;
            if (CameraManager->NoSmoothing) {
                CurrentStringLength = DesiredStringLength;
                CurrentHeightOffset = DesiredHeightOffset;
                CurrentCageRadius = DesiredCageRadius;
            } else {
                C3DVector pos = GetObservedThing()->GetPosition();
                C3DVector delta = goTo - pos;
                float length = sqrtf(delta.X * delta.X + delta.Y * delta.Y);
                CurrentStringLength = length;
                CurrentCageRadius = length;
                CurrentHeightOffset = goTo.Z - thingZ;
            }
        }
    } else {
        CurrentStringLength = DesiredStringLength;
        CurrentHeightOffset = DesiredHeightOffset;
        CurrentCageRadius = DesiredCageRadius;
        const CThingGameObject* thing = GetObservedThing();
        CThingPhysics* physics = thing->m_pPhysics;
        C3DVector up(0.0f, 0.0f, CurrentHeightOffset);
        C3DVector goTo = GetObservedThing()->GetPosition() - physics->GetFacingVector() * (CurrentCageRadius + CurrentStringLength) * 0.5f + up;
        const CThingGameObject* observed = GetObservedThing();
        C3DVector focal = observed->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
        Position = goTo;
        LookAt = focal;
    }
    long fps = g_framesPerSecond;
    NextAutoGoBehindWF = (long)(fps * GetAutoGoBehindTime() + GetWorldFrame());
}
}

static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
static int g_resets = 0;
static const NCameraModes::CBase* g_sacMode = 0;
static C3DVector g_pos(10.0f, 20.0f, 5.0f), g_facing(1.0f, 0.0f, 0.0f), g_focal(1.0f, 2.0f, 3.0f);
static float g_focalT = 0.0f;
static CThingGameObject g_thing;
static CThingPhysics g_physics;
static CTCGraphicAppearance g_appearance;
long g_framesPerSecond = 30;
long __fastcall GetWorldFrame() { return 1000; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
C3DVector CThingPhysics::GetFacingVector() const { return g_facing; }
C3DVector CTCGraphicAppearance::GetCameraFocalPos(float t) const { g_focalT = t; return g_focal; }
namespace NCameraModes {
bool CBase::IsStringAndCage() const { return this == g_sacMode; }
void CBase::ResetBase() { ++g_resets; }
float CBase::GetAutoGoBehindTime() const { return 2.5f; }
const CThingGameObject* CBase::GetObservedThing() const { return &g_thing; }
}
using NCameraModes::CStringAndCageBase;
static void Setup(CStringAndCageBase& m, CGameCameraManager& mgr)
{
    m.CameraManager = &mgr;
    m.DesiredHeightOffset = 2.0f; m.DesiredStringLength = 8.0f; m.DesiredCageRadius = 4.0f;
    m.CurrentHeightOffset = m.CurrentStringLength = m.CurrentCageRadius = -1.0f;
    m.NextAutoGoBehindWF = -1; m.TimerForExpansion = -1;
    m.Position = C3DVector(13.0f, 24.0f, 9.0f); m.LookAt = C3DVector(0.0f, 0.0f, 0.0f);
}
int main()
{
    g_thing.m_pPhysics = &g_physics; g_thing.m_pGraphicAppearance = &g_appearance;
    CGameCameraManager mgr; mgr.CurrentMode = 0; mgr.NoSmoothing = false;
    // no previous mode: desired values, camera behind the thing, look at its focal point
    CStringAndCageBase m; Setup(m, mgr);
    m.OnInit();
    Check(g_resets == 1 && m.CurrentHeightOffset == 2.0f && m.CurrentStringLength == 8.0f && m.CurrentCageRadius == 4.0f, __LINE__);
    Check(Near(m.Position.X, 4.0f) && Near(m.Position.Y, 20.0f) && Near(m.Position.Z, 7.0f), __LINE__);
    Check(m.LookAt.X == 1.0f && m.LookAt.Y == 2.0f && m.LookAt.Z == 3.0f && g_focalT == 1.0f, __LINE__);
    Check(m.NextAutoGoBehindWF == 1075 && m.TimerForExpansion == -1, __LINE__);
    // previous string-and-cage mode with a shorter string: expansion timer, its current values inherited
    CStringAndCageBase prev; Setup(prev, mgr);
    prev.DesiredStringLength = 5.0f; prev.CurrentHeightOffset = 1.0f; prev.CurrentStringLength = 6.0f; prev.CurrentCageRadius = 3.0f;
    g_sacMode = &prev; mgr.CurrentMode = &prev;
    Setup(m, mgr); m.OnInit();
    Check(m.TimerForExpansion == 60 && m.CurrentHeightOffset == 1.0f && m.CurrentStringLength == 6.0f && m.CurrentCageRadius == 3.0f, __LINE__);
    Check(m.Position.X == 13.0f && m.NextAutoGoBehindWF == 1075, __LINE__);
    prev.DesiredStringLength = 9.0f; Setup(m, mgr); m.OnInit();
    Check(m.TimerForExpansion == -1 && m.CurrentStringLength == 6.0f, __LINE__);
    // a previous mode of another kind, no smoothing: desired values
    g_sacMode = 0; mgr.NoSmoothing = true;
    Setup(m, mgr); m.OnInit();
    Check(m.CurrentHeightOffset == 2.0f && m.CurrentStringLength == 8.0f && m.CurrentCageRadius == 4.0f && m.Position.X == 13.0f, __LINE__);
    // with smoothing: horizontal distance from the go-to point and its height above the thing
    mgr.NoSmoothing = false;
    Setup(m, mgr); m.OnInit();
    Check(Near(m.CurrentStringLength, 5.0f) && Near(m.CurrentCageRadius, 5.0f) && Near(m.CurrentHeightOffset, 4.0f), __LINE__);
    Check(g_resets == 5 && m.NextAutoGoBehindWF == 1075, __LINE__);
    if (fails == 0) std::printf("00872630_TEST PASS\n");
    return fails;
}