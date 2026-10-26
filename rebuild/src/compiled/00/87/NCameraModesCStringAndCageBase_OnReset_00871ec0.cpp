// NCameraModes::CStringAndCageBase::OnReset() -- snap the string-and-cage camera: put it behind the observed thing (the thing's position less its flattened, normalised facing times the desired string length, raised by the desired height offset) and look at the thing's camera focal point
// Leaf: OnReset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871EC0, 270 bytes (+2 int3 padding), `ret`.
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition() const 0x004C73D0
//   (const C3DVector&), the thing physics' virtual slot 70 (+0x118, GetFacingVector, C3DVector by hidden pointer),
//   C3DVector::AccurateNormalise @ 0x00A14440, CTCGraphicAppearance::GetCameraFocalPos(float) const 0x004C12D0 (thing +0x64).
//   Float pool 0x0122DEDC = 0.0f.
// Layout: as the landed UpdateZVars 0x00872050 (retail CBase = PDB - 0x24): CBase LookAt +0x10, Position +0x54,
//   CameraManager +0x40; DesiredHeightOffset +0xA4, DesiredStringLength +0xA8. CThing physics +0x60, appearance +0x64.
// Shape: C3DVector with a user copy ctor and inline `return C3DVector(...)` operators; the raise is
//   C3DVector(0, 0, 1) * offset (1 * offset folds, 0 * offset is computed once and reused for X and Y). The focal point
//   is a named local in an inner block, so VC7.1 gives it the dead raise temporary's 12 bytes (0x3C frame); at function
//   scope it takes its own slot (0x48), and assigning LookAt straight from the call copies through eax.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& AccurateNormalise();                                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
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
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
};
class CThingGameObject : public CThing { };
struct CGameCameraManager;
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char       _pad04[0x0c];
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
    void OnReset();
};
void CStringAndCageBase::OnReset()
{
    C3DVector thingPos = GetObservedThing()->GetPosition();
    C3DVector facing = GetObservedThing()->GetPhysics()->GetFacingVector();
    facing.Z = 0.0f;
    facing.AccurateNormalise();
    Position = thingPos - facing * DesiredStringLength + C3DVector(0.0f, 0.0f, 1.0f) * DesiredHeightOffset;
    const CThingGameObject* thing = GetObservedThing();
    {
        C3DVector focal = thing->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
        LookAt = focal;
    }
}
}