// NCameraModes::CFirstPerson::OnInit() -- start the first-person camera at the observed thing: base at its position, the
// principal axis its flattened, normalised facing, angles zeroed, stick vector copied, fixed radius/height, and a
// camera cut (old camera = current, SingleCut)
// Leaf: OnInit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008735D0, length 183 bytes (padded to 192), `ret`.
// Callees: NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0 (CameraManager->PObservedThing.GetPItem();
// labelled _Cons_val), CThing::GetPosition() const @ 0x004C73D0 (labelled CVertexBufferWin32::DoSizeof), the thing
// physics' virtual slot 70 (+0x118, a C3DVector by hidden pointer: the facing vector), C3DVector::Normalise
// @ 0x00A14440.
// Retail layout (Ego_r.pdb names; retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8: CPhysicsState 0x30 and a 12-byte
// vector): CBase CameraManager +0x40, LeftStickVector +0x98; CFirstPerson OffsetHeight +0xA4, CylinderRadius +0xA8,
// Base +0xAC, PrincipleAxis +0xB8, Theta +0xC4, Phi +0xC8, RSVector +0xCC; CGameCameraManager Camera +0x04,
// OldCamera +0x3C, SingleCut +0xB0; CThing physics +0x60. Graphics/camera layer: CFirstPerson is the mode behind the
// "first person" camera (vtable via rtti-vtables.tsv NCameraModes::CFirstPerson).
// Shape: C3DVector has a user copy ctor and the implicit operator= (the `lea` pointer-register copies); the axis is
// copied, its Z zeroed and normalised in place; the physics is reached through an inline GetPhysics() (straight into
// ecx); the stick vector is assigned after the two constants (VC then interleaves its stores with them); the camera
// cut is an inline manager member (CCamera's implicit assignment, `rep movsd` x 0xE, then SingleCut), which lets
// `this` live in esi.

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void Normalise();                       // 0x00A14440
};
struct C2DVector {
    float X, Y;
};
struct CCamera {
    unsigned char _body[0x38];
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
    virtual C3DVector GetFacingVector() const;      // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad[0x60];
    CThingPhysics* m_pPhysics;              // +0x60
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CThingGameObject : CThing {};

class CGameCameraManager {
public:
    void* __vftable;                        // +0x00
    CCamera Camera;                         // +0x04
    CCamera OldCamera;                      // +0x3C
    unsigned char _pad74[0x3c];
    bool SingleCut;                         // +0xB0
    void CutCamera() { OldCamera = Camera; SingleCut = true; }
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x3c];
    CGameCameraManager* CameraManager;      // +0x40
    unsigned char _pad44[0x54];
    C2DVector LeftStickVector;              // +0x98
    unsigned char _pada0[0x4];
    const CThingGameObject* GetObservedThing() const;   // 0x0084F1B0
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
    virtual void OnInit();
};
}

void NCameraModes::CFirstPerson::OnInit()
{
    Base = GetObservedThing()->GetPosition();
    PrincipleAxis = GetObservedThing()->GetPhysics()->GetFacingVector();
    PrincipleAxis.Z = 0.0f;
    PrincipleAxis.Normalise();
    Theta = 0.0f;
    Phi = 0.0f;
    CylinderRadius = 0.015f;
    OffsetHeight = 1.45f;
    RSVector = LeftStickVector;
    CameraManager->CutCamera();
}