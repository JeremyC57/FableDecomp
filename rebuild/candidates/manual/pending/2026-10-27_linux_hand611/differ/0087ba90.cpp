// NCameraModes::CScriptedControl::OnInit() -- remember the current camera, hand the script module its owner and camera manager and start it, then point the camera from the go-to point along the look-at direction with the mode's roll and FOV, cut, and record the observed thing's mesh height
// Grade: DIFFER(294v294) -- the module vtable load: retail stores PCameraManager first and loads the vtable into the freed eax; ours loads it into edx before the store, and every later eax/ecx/edx choice rotates by one. Agent611: names re-proven (OnInit is virtual UAE; CBase::GetObservedThingCurrentMeshHeight virtual const called qualified; SetCameraRoll is NCameraModes:: free fastcall on CCamera&; AccurateNormalise; GetCamera protected CCamera&). Tried: named module pointer, manager local, inline module Init helper, optimize("s").
// Leaf: ?OnInit@CScriptedControl@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BA90, length 294 bytes (`ret`); vtable 0x0125C58C slot 21 (OnDie 0x0087BBF0 slot 22).
// Identity (proven): Ego_r.pdb ?OnInit@CScriptedControl@NCameraModes@@UAEXXZ (virtual) + the RTTI slot.
// Callees: NCameraModes::CBase::GetCamera 0x0084D070 (Ego_r.pdb protected IAEAAVCCamera@@XZ), the script module's slot 8
//   (+0x20, OnInitLeaf: the CScriptModuleFullTransition / OpenChest overrides of slot 8, Ego_r.pdb UAEXXZ),
//   C3DVector::AccurateNormalise 0x00A14440, CCamera::PointAt(const C3DVector&) 0x00A0C2B0,
//   NCameraModes::SetCameraRoll(CCamera&, float) 0x0084CD10 (Ego_r.pdb ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z),
//   CCamera::SetFOV(float, float) 0x00A0BED0, NCameraModes::CBase::GetObservedThingCurrentMeshHeight 0x0084D8B0 called
//   qualified (it is CBase's vtable 0x0124DD74 slot 15; CScriptedControl overrides slot 15 with 0x0069E060).
//   Global: the 1e-4f epsilon 0x0129BA3C (squared at run time; 50 different spellings are bound to it -- descriptive).
// Layout: as the landed OnDie twin 0x0087BBF0 (InitCamera +0xb0, PScriptModule +0xa4 whose POwner +0x38 / PCameraManager +0x3c
//   are set); CGameCameraManager as engine/CGameCameraManager.h (Camera +4, OldCamera +0x3c, SingleCut +0xb0) viewed through a
//   local polymorphic-free model because the header keeps the cameras as opaque byte arrays.
// Shape: the camera snapshot is CCamera's implicit assignment (`rep movsd` x 0xE); roll and FOV read into locals before the
// camera work; the view vector (look-at - go-to) stays on the FPU while the camera position takes the go-to point (implicit
// operator= `lea` copies) and a direction local takes the view; when its squared length exceeds eps*eps the direction is
// normalised and the camera points at go-to + direction with the roll; then the FOV, the inline camera cut and the height.
#include <math.h>
#include "engine/CCountedPointer.h"
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector& AccurateNormalise();         // 0x00A14440
};
class CRightHandedSet { public:
    C3DVector Up;                           // +0x00
    C3DVector Forward;                      // +0x0c
};
class CCamera { public:                     // 0x38 bytes
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


class CGameCameraManager {
public:
    void*         __vftable;                // +0x00
    CCamera       Camera;                   // +0x04
    CCamera       OldCamera;                // +0x3C
    unsigned char _pad74[0x3c];
    bool          SingleCut;                // +0xB0
    void CutCamera() { OldCamera = Camera; SingleCut = true; }
};

class CPhysicsState { public:                      // retail 0x30 bytes, position first
    C3DVector     Position;
    unsigned char _rest[0x24];
};

namespace NCameraModes {
class CBase;
void __fastcall SetCameraRoll(CCamera& camera, float roll);   // 0x0084CD10
class CScriptModuleBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void OnInitLeaf();              // slot 8 (+0x20)
    unsigned char       _pad04[0x34];
    CBase*              POwner;             // +0x38
    CGameCameraManager* PCameraManager;     // +0x3c
};
class CBase {
public:
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
    virtual float GetObservedThingCurrentMeshHeight() const;   // 0x0084D8B0 (vtable 0x0124DD74 slot 15)
protected:
    CCamera& GetCamera();                                 // 0x0084D070
};
class CScriptedControl : public CBase {
public:
    CCountedPointer<CScriptModuleBase> PScriptModule;     // +0xa4
    float              ObservedThingMeshHeightOnInit;     // +0xac
    CCamera            InitCamera;                        // +0xb0
    virtual void OnInit();
    virtual void OnDie();
};

extern float g_fEpsilon;                    // 0x0129BA3C (1e-4f)

void CScriptedControl::OnInit()
{
    CCamera* camera = &GetCamera();
    InitCamera = *camera;
    CScriptModuleBase& module = *PScriptModule;
    module.PCameraManager = CameraManager;
    module.POwner = this;
    module.OnInitLeaf();
    float roll = RollAngle;
    float fov = CameraFOV;
    C3DVector view = LookAt.Position - GoTo.Position;
    camera->WorldPosition = GoTo.Position;
    C3DVector direction = view;
    if (view.GetLengthSquared() > g_fEpsilon * g_fEpsilon) {
        direction.AccurateNormalise();
        camera->PointAt(direction + GoTo.Position);
        SetCameraRoll(*camera, roll);
    }
    camera->SetFOV(fov, 0.0f);
    CameraManager->CutCamera();
    ObservedThingMeshHeightOnInit = CBase::GetObservedThingCurrentMeshHeight();
}
}
