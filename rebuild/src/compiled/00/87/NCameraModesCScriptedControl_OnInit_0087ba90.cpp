// NCameraModes::CScriptedControl::OnInit() -- remember the current camera, hand the script module its owner and camera manager and start it, then point the camera from the go-to point along the look-at direction with the mode's roll and FOV, cut, and record the observed thing's mesh height
// Leaf: ?OnInit@CScriptedControl@NCameraModes@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BA90, length 294 bytes (`ret`).
// Callees: NCameraModes::CBase::GetCamera @ 0x0084D070, CScriptModuleBase::OnInitLeaf (virtual slot 8, +0x20) through the
// inline CScriptModuleBase::OnInit(CBase&, const CGameCameraManager&) (Ego_r.pdb), C3DVector::AccurateNormalise @ 0x00A14440,
// CCamera::PointAt(const C3DVector&) @ 0x00A0C2B0, NCameraModes::SetCameraRoll(CCamera&, float) @ 0x0084CD10 (`__fastcall`), CCamera::SetFOV(float, float) @ 0x00A0BED0, NCameraModes::CBase::GetObservedThingCurrentMeshHeight
// @ 0x0084D8B0 (virtual in Ego_r, called non-virtually). Global: DELTA_NOTIONAL_ZERO 0x0129BA3C (1e-4f, squared at run time).
// Layout: as the OnDie twin 0x0087BBF0 (InitCamera +0xb0, PScriptModule +0xa4 whose POwner +0x38 / PCameraManager +0x3c are set).
// Shape: the module set-up is the PDB inline OnInit (POwner, then PCameraManager, then the OnInitLeaf call: the vtable
// load lands between the two stores); the camera snapshot is CCamera's implicit assignment (`rep movsd` x 0xE); roll and FOV read into locals before the
// camera work; the view vector (look-at - go-to) stays on the FPU while the camera position takes the go-to point (implicit
// operator= `lea` copies) and a direction local takes the view; when its squared length exceeds eps*eps the direction is
// normalised and the camera points at go-to + direction with the roll; then the FOV, the inline camera cut and the height.
#include <math.h>
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
    C3DVector& AccurateNormalise();         // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CRightHandedSet { public:
    C3DVector Up;                           // +0x00
    C3DVector Forward;                      // +0x0c
};
class CCamera { public:                            // 0x38 bytes
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

#include "engine/CThing.h"
class CThingGameObject : public CThing { };
inline bool IS_THING_ALIVE(const CThing* thing) { return thing && !thing->IsDead(); }

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

extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)
namespace NCameraModes {
void __fastcall SetCameraRoll(CCamera& camera, float roll);   // 0x0084CD10 (Ego_r ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z)
class CBase;
class CScriptModuleBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsFinished() const;        // slot 7 (+0x1c) Ego_r.pdb ?IsFinished@CScriptModuleBase@NCameraModes@@UBE_NXZ
protected:
    virtual void OnInitLeaf();              // slot 8 (+0x20) Ego_r.pdb ?OnInitLeaf@CScriptModuleBase@NCameraModes@@MAEXXZ
public:
    // Ego_r.pdb ?OnInit@CScriptModuleBase@NCameraModes@@QAEXAAVCBase@2@ABVCGameCameraManager@@@Z (inline in retail)
    void OnInit(CBase& owner, const CGameCameraManager& manager)
    {
        POwner = &owner;
        PCameraManager = const_cast<CGameCameraManager*>(&manager);
        OnInitLeaf();
    }
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
protected:
    CCamera& GetCamera();                                 // 0x0084D070 (Ego_r ?GetCamera@CBase@NCameraModes@@IAEAAVCCamera@@XZ)
public:
    const CThingGameObject* GetObservedThing() const;     // 0x0084F1B0
    virtual float GetObservedThingCurrentMeshHeight() const;   // 0x0084D8B0 (Ego_r UBE; called non-virtually here)
};
class CScriptedControl : public CBase {
public:
    CScriptModuleBase* PScriptModule;                     // +0xa4 CCountedPointer<CScriptModuleBase>::Data
    void*              PScriptModuleInfo;                 // +0xa8
    float              ObservedThingMeshHeightOnInit;     // +0xac
    CCamera            InitCamera;                        // +0xb0
    virtual void OnInit();
    void OnDie();
};

void CScriptedControl::OnInit()
{
    CCamera& camera = GetCamera();
    InitCamera = camera;
    PScriptModule->OnInit(*this, *CameraManager);
    float roll = RollAngle;
    float fov = CameraFOV;
    C3DVector view = LookAt.Position - GoTo.Position;
    camera.WorldPosition = GoTo.Position;
    C3DVector direction = view;
    if (view.GetLengthSquared() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        direction.AccurateNormalise();
        camera.PointAt(direction + GoTo.Position);
        SetCameraRoll(camera, roll);
    }
    camera.SetFOV(fov, 0.0f);
    CameraManager->CutCamera();
    ObservedThingMeshHeightOnInit = CBase::GetObservedThingCurrentMeshHeight();
}
}