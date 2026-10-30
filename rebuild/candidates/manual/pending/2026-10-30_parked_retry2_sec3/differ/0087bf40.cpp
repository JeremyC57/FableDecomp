// Grade: DIFFER(293v295) -- C3DVector& xAxis alias fixes the XAxis.Y read through edi ([edi+4]); left: retail computes both XAxis differences on the FPU before storing (fld/fsub, fld/fsub, fxch, fstp x2), ours stores X before computing Y (2 bytes)
// NCameraModes::CRotateAround::OnInit() -- set up the rotate-around camera: the XY string length and height offset from the go-to point to the look-at point / observed thing, the normalised XY axes, Theta 0, and the FOV, Z offset and mode def inherited from the current camera mode
// Leaf: OnInit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BF40, length 295 bytes (`ret`).
// Callees: NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0, CThing::GetPosition() const @ 0x004C73D0,
// C3DVector::Normalise @ 0x00A14440 (x2), the current mode's virtual slot 18 (+0x48, its FOV; descriptive name) and the mode
// def's virtual slot 1 (destroy, when the inline CDefPointer assignment drops the last reference).
// Layout: retail NCameraModes::CBase (0xA4 bytes, Ego_r.pdb names, CPhysicsState 0x30 and 12-byte vectors as in the landed
// CFirstPerson::OnInit 0x008735D0): DefaultFOV +4, LookAt +0x10 (position first), CameraManager +0x40, PCameraModeDef +0x44,
// GoTo +0x54, ObservedPositionZOffset +0x8c; CRotateAround: StringLengthXY +0xa4, HeightOffset +0xa8, XAxis +0xac,
// YAxis +0xb8, Theta +0xc4 (PDB - 0x24); CGameCameraManager::CurrentMode +0x98.
// Shape: the look-at XY is a C2DVector local (integer copies into the frame); the length squares the Y term first (one
// expression); the axes are C3DVector temporaries (Z = 0.0f through the frame) copied by the implicit operator= over a
// user-copy-constructible C3DVector (the `lea edi` / `mov edx,edi` pointer copies); the def pointer assignment is the
// inline CDefPointer operator= (short `dec [ecx+4] ; jne` release, then add-ref).
#include <math.h>
#pragma intrinsic(sqrt)

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                       // 0x00A14440
};
struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CThingGameObject : CThing { };

struct CCameraModeDef {
    virtual void v00();
    virtual void Destroy();                 // slot 1 (+4)
    long RefCount;                          // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CDefPointer {                        // CDefPointer<const CCameraModeDef>
    CCameraModeDef* m_p;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->RefCount;
        }
        return *this;
    }
};

struct CPhysicsState {                      // retail 0x30 bytes, position first
    C3DVector     Position;
    unsigned char _rest[0x24];
};

namespace NCameraModes { class CBase; }
class CGameCameraManager {
public:
    unsigned char       _pad00[0x98];
    NCameraModes::CBase* CurrentMode;       // +0x98 CCountedPointer<NCameraModes::CBase>::Data
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual float GetFOV();                 // slot 18 (+0x48) descriptive
    float               DefaultFOV;                       // +0x04
    float               DefaultRollAngle;                 // +0x08
    float               DefaultObservedPositionZOffset;   // +0x0c
    CPhysicsState       LookAt;                           // +0x10
    CGameCameraManager* CameraManager;                    // +0x40
    CDefPointer         PCameraModeDef;                   // +0x44
    unsigned char       SubComponents[0xc];               // +0x48 vector<CCountedPointer<CSubComponentBase> >
    CPhysicsState       GoTo;                             // +0x54
    float               CameraFOV;                        // +0x84
    float               RollAngle;                        // +0x88
    float               ObservedPositionZOffset;          // +0x8c
    unsigned char       _pad90[0x14];                     // +0x90 stick vectors, ViewOccluded
    const CThingGameObject* GetObservedThing() const;     // 0x0084F1B0
};
class CRotateAround : public CBase {
public:
    float     StringLengthXY;               // +0xa4
    float     HeightOffset;                 // +0xa8
    C3DVector XAxis;                        // +0xac
    C3DVector YAxis;                        // +0xb8
    float     Theta;                        // +0xc4
    void OnInit();
};

void CRotateAround::OnInit()
{
    C3DVector axis;
    axis.X = LookAt.Position.X;
    axis.Y = LookAt.Position.Y;
    float dx = axis.X - GoTo.Position.X;
    float dy = axis.Y - GoTo.Position.Y;
    StringLengthXY = (float)sqrt(dx * dx + dy * dy);
    HeightOffset = GoTo.Position.Z - GetObservedThing()->GetPosition().Z;
    axis.X = GoTo.Position.X - axis.X;
    axis.Y = GoTo.Position.Y - axis.Y;
    axis.Z = 0.0f;
    C3DVector& xAxis = XAxis;
    xAxis = axis;
    xAxis.Normalise();
    axis.X = xAxis.Y;
    axis.Y = -xAxis.X;
    axis.Z = 0.0f;
    YAxis = axis;
    YAxis.Normalise();
    Theta = 0.0f;
    CBase* previous = CameraManager->CurrentMode;
    DefaultFOV = previous->GetFOV();
    ObservedPositionZOffset = previous->ObservedPositionZOffset;
    PCameraModeDef = previous->PCameraModeDef;
}
}
