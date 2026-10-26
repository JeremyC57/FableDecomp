// NCameraModes::CStringAndCageBase::OnInit() -- start the string-and-cage camera: inherit the current string/cage lengths and height from a previous string-and-cage mode (with an expansion timer when the new string is longer), else measure them from the go-to point or, with no previous mode, take the desired values and place the camera behind the thing; then schedule the next auto-go-behind frame
// Leaf: OnInit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872630, 647 bytes, `ret`. Callees: virtual slots 26 (+0x68, base reset), 14 (+0x38,
//   IsStringAndCage on the camera manager's current mode) and 29 (+0x74, GetAutoGoBehindTime); NCameraModes::CBase::
//   GetObservedThing 0x0084F1B0, CThing::GetPosition 0x004C73D0, the physics' GetFacingVector (slot 70, +0x118),
//   CTCGraphicAppearance::GetCameraFocalPos 0x004C12D0, GetWorldFrame 0x0049D870 (manifest "GFGetPreMainMemoryUsed"), __ftol2.
//   Globals: frames-per-second long 0x01375550; float pool 0.5f 0x0122F59C.
// Layout: as the landed UpdateZVars 0x00872050 / OnReset 0x00871EC0 (retail = Ego_r.pdb - 0x24): Desired{HeightOffset,
//   StringLength,CageRadius} +0xA4.., Current{...} +0xB0.., NextAutoGoBehindWF +0xBC, TimerForExpansion +0xC0;
//   CGameCameraManager CurrentMode +0x98 (raw pointer read), NoSmoothing +0xB0.
// Shape: the thing's Z is a named float before the mode test; the measured branch copies the position and forms a
//   C3DVector delta (sqrt of X*X + Y*Y); the placement branch names the physics pointer and a C3DVector(0,0,height)
//   BEFORE the one-expression `pos - facing * (cage + string) * 0.5f + up` (a named distance local stores after the lea),
//   and the focal's thing is a named pointer at function scope (an inner block shares the temps' slots, frame 0x38;
//   a direct GetObservedThing()->... lets VC hoist the focal pushes into the subtraction). The fps is read into a local
//   before the virtual call (fimul from the frame).
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CThingPhysics {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
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
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                             // 0x0049D870
namespace NCameraModes {
class CBase;
}
class CGameCameraManager { public:
    unsigned char        _pad000[0x98];
    NCameraModes::CBase* CurrentMode;                        // +0x98
    unsigned char        _pad09c[0xb0 - 0x9c];
    bool                 NoSmoothing;                        // +0xb0
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual bool IsStringAndCage() const;                    // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void ResetBase();                                // slot 26 (+0x68)
    virtual void v27(); virtual void v28();
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
    NextAutoGoBehindWF = (long)(fps * GetAutoGoBehindTime() + CWorld::GetFrame());
}
}