// NCameraModes::CScriptedControl::OnDie() -- when the script module is finished, restore the initial camera: go-to point = its position, look-at point = position + forward * distance to the observed thing (1 when gone), the FOV, and a camera cut
// Leaf: OnDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BBF0, length 298 bytes (`ret`).
// Callees: the script module's virtual slot 7 (+0x1c, bool; descriptive IsFinished), NCameraModes::CBase::GetObservedThing()
// const @ 0x0084F1B0 (twice), CThing::GetPosition() const @ 0x004C73D0, CCamera::GetHorizontalFOV() const @ 0x00A0BEB0.
// Layout: retail NCameraModes::CBase as the landed CFirstPerson::OnInit 0x008735D0 (LookAt +0x10, CameraManager +0x40, GoTo +0x54,
// CameraFOV +0x84); CScriptedControl (Ego_r.pdb - 0x24): PScriptModule +0xa4, ObservedThingMeshHeightOnInit +0xac, InitCamera
// (CCamera, 0x38 bytes) +0xb0; CGameCameraManager Camera +4, OldCamera +0x3c, SingleCut +0xb0.
// Shape: the camera position copied into a local; the distance a float local preset to 1.0f in the frame, replaced by the
// length of (position - observed position) when the observed thing is alive (inline IS_THING_ALIVE re-fetching the thing);
// GoTo / LookAt assigned through the implicit C3DVector operator= (`lea` pointer copies), the look-at through value operators
// (frame temporaries); the camera cut is the inline manager member of the landed CFirstPerson (`rep movsd` x 0xE).
#include <math.h>
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                       // 0x00A14440
};
struct CRightHandedSet {
    C3DVector Up;                           // +0x00
    C3DVector Forward;                      // +0x0c
};
struct CCamera {                            // 0x38 bytes
    C3DVector       WorldPosition;          // +0x00
    CRightHandedSet RHSet;                  // +0x0c
    bool            HeightLocked;           // +0x24
    bool            ViewVecZLocked;         // +0x25
    unsigned long   FOVFlags;               // +0x28
    float           HorizontalFOV;          // +0x2c
    float           VerticalFOV;            // +0x30
    float           Zoom;                   // +0x34
    float GetHorizontalFOV() const;         // 0x00A0BEB0
    void PointAt(const C3DVector& target);  // 0x00A0C2B0
    void SetFOV(float horizontal, float vertical);   // 0x00A0BED0
};
void __fastcall SetCameraRoll(CCamera* camera, float roll);   // 0x0084CD10

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CThingGameObject : CThing { };
inline bool IS_THING_ALIVE(const CThing* thing) { return thing && !(thing->m_flags91 & 1); }

class CGameCameraManager {
public:
    void*         __vftable;                // +0x00
    CCamera       Camera;                   // +0x04
    CCamera       OldCamera;                // +0x3C
    unsigned char _pad74[0x3c];
    bool          SingleCut;                // +0xB0
    void CutCamera() { OldCamera = Camera; SingleCut = true; }
};

struct CPhysicsState {                      // retail 0x30 bytes, position first
    C3DVector     Position;
    unsigned char _rest[0x24];
};

namespace NCameraModes {
class CBase;
class CScriptModuleBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsFinished();              // slot 7 (+0x1c) descriptive
    virtual void OnStart();                 // slot 8 (+0x20) descriptive
    unsigned char       _pad04[0x34];
    CBase*              POwner;             // +0x38
    CGameCameraManager* PCameraManager;     // +0x3c
};
class CBase {
public:
    virtual void v00();
    float               DefaultFOV;                       // +0x04
    float               DefaultRollAngle;                 // +0x08
    float               DefaultObservedPositionZOffset;   // +0x0c
    CPhysicsState       LookAt;                           // +0x10
    CGameCameraManager* CameraManager;                    // +0x40
    void*               PCameraModeDef;                   // +0x44
    unsigned char       SubComponents[0xc];               // +0x48
    CPhysicsState       GoTo;                             // +0x54
    float               CameraFOV;                        // +0x84
    float               RollAngle;                        // +0x88
    float               ObservedPositionZOffset;          // +0x8c
    unsigned char       _pad90[0x14];                     // +0x90 stick vectors, ViewOccluded
    CCamera* GetCamera();                                 // 0x0084D070
    const CThingGameObject* GetObservedThing() const;     // 0x0084F1B0
    float GetObservedThingCurrentMeshHeight();            // 0x0084D8B0
};
class CScriptedControl : public CBase {
public:
    CScriptModuleBase* PScriptModule;                     // +0xa4 CCountedPointer<CScriptModuleBase>::Data
    void*              PScriptModuleInfo;                 // +0xa8
    float              ObservedThingMeshHeightOnInit;     // +0xac
    CCamera            InitCamera;                        // +0xb0
    void OnInit();
    void OnDie();
};

void CScriptedControl::OnDie()
{
    if (PScriptModule->IsFinished()) {
        C3DVector position = InitCamera.WorldPosition;
        float distance = 1.0f;
        if (IS_THING_ALIVE(GetObservedThing()))
            distance = (position - GetObservedThing()->GetPosition()).GetLength();
        GoTo.Position = position;
        LookAt.Position = InitCamera.RHSet.Forward * distance + position;
        CameraFOV = InitCamera.GetHorizontalFOV();
        CameraManager->CutCamera();
    }
}
}
