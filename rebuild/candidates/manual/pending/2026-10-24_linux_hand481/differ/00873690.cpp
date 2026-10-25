// NCameraModes::CFirstPerson::Update() -- step the first-person camera: smooth the right-stick vector towards the left stick, advance pitch (Theta, clamped to -85..+50 degrees in turns) and yaw (Phi), swing the principal axis by the yaw about the vertical, set the go-to position on the cylinder around the raised base and look along the pitched direction
// Grade: DIFFER(603v603) -- frame layout + x87 order: retail parks the offset height at the top of a 0x48 frame (ours 0x40, height low), and sums the yawed direction / scales it by the radius in a different x87 order
// Leaf: ?Update@CFirstPerson@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00873690, length 603 bytes (ret). Vtable slot 1 of NCameraModes::CFirstPerson
// (0x012776AC); Ego_r.pdb ?Update@CFirstPerson@NCameraModes@@UAEXXZ. The catalog label "CVertexBufferWin32_UpdateTransform" is
// propagated and wrong.
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (the inlined GetObservedThing: CameraManager->PObservedThing, result
//          unused), NCameraModes::CBase::GetCamTools() const 0x0084D080 (result unused), C3DVector::AccurateNormalise()
//          0x00A14440 (x2), C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60.
// Pools: 0.5f 0x0122F59C, 2/3 0x012544AC, 0.01f 0x01230A5C, 0.025f 0x0126F42C, -0.2361111f 0x01277638 (also the immediate
//        store), 0.1388889f 0x01277634 (also the immediate), (double)TWO_PI 0x0124F2B8, 0.0f 0x0122DEDC.
// Layout (retail CBase 0xA4 bytes, as the landed CFirstPerson::OnInit 0x008735D0): CBase LookAtPosition +0x10, CameraManager
//        +0x40, GoToPosition +0x54, LeftStickVector +0x98; CFirstPerson OffsetHeight +0xA4, CylinderRadius +0xA8, Base +0xAC,
//        PrincipleAxis +0xB8, Theta +0xC4, Phi +0xC8, RSVector +0xCC; CGameCameraManager PObservedThing +0x100.
// Shape: RSVector = (RSVector * 0.5f + LeftStickVector) * (2/3) through C2DVector operators (int copies of the temp);
// Phi's new value goes to a local before the member (`float phi = Phi + ...; Phi = phi;`); the clamps are immediate stores;
// the side vector is (Axis.Y, -Axis.X, 0); the go-to is `C3DVector(Base.X, Base.Y, Base.Z + height) + dir * radius` (the
// scaled argument first); the pitch axis is (dir.Y, -dir.X, 0).
// Tried (diagnostic compiles): member-first Phi, a named base local, sin term first, float s/c locals, a named offset, the
// height read before GetObservedThing.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sin, cos)
static const float TWO_PI = 6.2831855f;

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& r) const { return C2DVector(X + r.X, Y + r.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& r) const { return C3DVector(X + r.X, Y + r.Y, Z + r.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    void AccurateNormalise();                                 // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);          // 0x00A13C60
};
class CThing;
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                                 // 0x00A01B50
};
class CGameCameraManager {
public:
    unsigned char _pad000[0x100];
    CBaseIntelligentPointer PObservedThing;                   // +0x100
};
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x0c];
    C3DVector LookAtPosition;                                 // +0x10
    unsigned char _pad1c[0x24];
    CGameCameraManager* CameraManager;                        // +0x40
    unsigned char _pad44[0x10];
    C3DVector GoToPosition;                                   // +0x54
    unsigned char _pad60[0x38];
    C2DVector LeftStickVector;                                // +0x98
    unsigned char _pada0[0x4];
    const CThing* GetObservedThing() const { return CameraManager->PObservedThing.GetPItem(); }
    const CTools& GetCamTools() const;                        // 0x0084D080
};
class CFirstPerson : public CBase {
public:
    float OffsetHeight;                     // +0xA4
    float CylinderRadius;                   // +0xA8
    C3DVector Base;                         // +0xAC
    C3DVector PrincipleAxis;                // +0xB8
    float Theta;                            // +0xC4
    float Phi;                              // +0xC8
    C2DVector RSVector;                     // +0xCC
    virtual void Update();
};
}

void NCameraModes::CFirstPerson::Update()
{
    GetObservedThing();
    float height = OffsetHeight;
    GetCamTools();
    RSVector = (RSVector * 0.5f + LeftStickVector) * 0.6666667f;
    Theta += RSVector.Y * 0.01f;
    float phi = Phi + RSVector.X * 0.025f;
    Phi = phi;
    if (Theta < -0.2361111f)
        Theta = -0.2361111f;
    else if (Theta > 0.1388889f)
        Theta = 0.1388889f;
    C3DVector side(PrincipleAxis.Y, -PrincipleAxis.X, 0.0f);
    double angle = phi * (double)TWO_PI;
    C3DVector dir = PrincipleAxis * (float)cos(angle) + side * (float)sin(angle);
    dir.AccurateNormalise();
    C3DVector goTo = C3DVector(Base.X, Base.Y, Base.Z + height) + dir * CylinderRadius;
    GoToPosition = goTo;
    C3DVector axis(dir.Y, -dir.X, 0.0f);
    dir.Rotate(axis, Theta);
    dir.AccurateNormalise();
    LookAtPosition = goTo + dir;
}
