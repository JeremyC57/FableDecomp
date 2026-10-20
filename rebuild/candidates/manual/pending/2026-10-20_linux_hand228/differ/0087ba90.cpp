// Grade: DIFFER(294v294) -- register allocation only: retail loads the module vtable into eax AFTER the PCameraManager store (eax freed), ours hoists it into edx before the stores, which rotates eax/ecx/edx through the rest of the body (roll/FOV locals, the WorldPosition copy, the PointAt/SetCameraRoll/SetFOV pushes); same instructions otherwise (tried: direct member stores, a module local, an inline SetOwner)
// NCameraModes::CScriptedControl::OnInit() -- remember the current camera, hand the script module its owner and camera manager and start it, then point the camera from the go-to point along the look-at direction with the mode's roll and FOV, cut, and record the observed thing's mesh height
// Leaf: OnInit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BA90, length 294 bytes (`ret`).
// Callees: NCameraModes::CBase::GetCamera @ 0x0084D070, the script module's virtual slot 8 (+0x20; descriptive OnStart),
// C3DVector::Normalise @ 0x00A14440, CCamera::PointAt(const C3DVector&) @ 0x00A0C2B0, SetCameraRoll(CCamera*, float) @ 0x0084CD10
// (`__fastcall`), CCamera::SetFOV(float, float) @ 0x00A0BED0, NCameraModes::CBase::GetObservedThingCurrentMeshHeight
// @ 0x0084D8B0. Global: the 1e-4f epsilon 0x0129BA3C (an extern float variable, squared at run time).
// Layout: as the OnDie twin 0x0087BBF0 (InitCamera +0xb0, PScriptModule +0xa4 whose POwner +0x38 / PCameraManager +0x3c are set).
// Shape: the camera snapshot is CCamera's implicit assignment (`rep movsd` x 0xE); roll and FOV read into locals before the
// camera work; the view vector (look-at - go-to) stays on the FPU while the camera position takes the go-to point (implicit
// operator= `lea` copies) and a direction local takes the view; when its squared length exceeds eps*eps the direction is
// normalised and the camera points at go-to + direction with the roll; then the FOV, the inline camera cut and the height.
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
    float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
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

extern float g_fEpsilon;                    // 0x0129BA3C (1e-4f)

void CScriptedControl::OnInit()
{
    CCamera* camera = GetCamera();
    InitCamera = *camera;
    CScriptModuleBase* module = PScriptModule;
    module->PCameraManager = CameraManager;
    module->POwner = this;
    module->OnStart();
    float roll = RollAngle;
    float fov = CameraFOV;
    C3DVector view = LookAt.Position - GoTo.Position;
    camera->WorldPosition = GoTo.Position;
    C3DVector direction = view;
    if (view.GetLengthSquared() > g_fEpsilon * g_fEpsilon) {
        direction.Normalise();
        camera->PointAt(direction + GoTo.Position);
        SetCameraRoll(camera, roll);
    }
    camera->SetFOV(fov, 0.0f);
    CameraManager->CutCamera();
    ObservedThingMeshHeightOnInit = GetObservedThingCurrentMeshHeight();
}
}
