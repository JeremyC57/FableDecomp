// Behaviour test for 00871ec0: src/00871ec0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NCameraModes::CStringAndCageBase::OnReset()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CStringAndCageBase::OnReset() -- snap the string-and-cage camera: put it behind the observed thing (the thing's position less its flattened, normalised facing times the desired string length, raised by the desired height offset) and look at the thing's camera focal point
// Leaf: OnReset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871EC0, 270 bytes (+2 int3 padding), `ret`.
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition() const 0x004C73D0
//   (const C3DVector&), the thing physics' virtual slot 70 (+0x118, GetFacingVector, C3DVector by hidden pointer),
//   C3DVector::Normalise 0x00A14440, CTCGraphicAppearance::GetCameraFocalPos(float) const 0x004C12D0 (thing +0x64).
//   Float pool 0x0122DEDC = 0.0f.
// Layout: as the landed UpdateZVars 0x00872050 (retail CBase = PDB - 0x24): CBase LookAt +0x10, Position +0x54,
//   CameraManager +0x40; DesiredHeightOffset +0xA4, DesiredStringLength +0xA8. CThing physics +0x60, appearance +0x64.
// Shape: C3DVector with a user copy ctor and inline `return C3DVector(...)` operators; the raise is
//   C3DVector(0, 0, 1) * offset (1 * offset folds, 0 * offset is computed once and reused for X and Y). The focal point
//   is a named local in an inner block, so VC7.1 gives it the dead raise temporary's 12 bytes (0x3C frame); at function
//   scope it takes its own slot (0x48), and assigning LookAt straight from the call copies through eax.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void Normalise();                                        // 0x00A14440
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
    facing.Normalise();
    Position = thingPos - facing * DesiredStringLength + C3DVector(0.0f, 0.0f, 1.0f) * DesiredHeightOffset;
    const CThingGameObject* thing = GetObservedThing();
    {
        C3DVector focal = thing->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
        LookAt = focal;
    }
}
}

// ---- stubs and driver ----
#include <cmath>
#include <cstdlib>
void CThingPhysics::v000() { }
void CThingPhysics::v001() { }
void CThingPhysics::v002() { }
void CThingPhysics::v003() { }
void CThingPhysics::v004() { }
void CThingPhysics::v005() { }
void CThingPhysics::v006() { }
void CThingPhysics::v007() { }
void CThingPhysics::v008() { }
void CThingPhysics::v009() { }
void CThingPhysics::v010() { }
void CThingPhysics::v011() { }
void CThingPhysics::v012() { }
void CThingPhysics::v013() { }
void CThingPhysics::v014() { }
void CThingPhysics::v015() { }
void CThingPhysics::v016() { }
void CThingPhysics::v017() { }
void CThingPhysics::v018() { }
void CThingPhysics::v019() { }
void CThingPhysics::v020() { }
void CThingPhysics::v021() { }
void CThingPhysics::v022() { }
void CThingPhysics::v023() { }
void CThingPhysics::v024() { }
void CThingPhysics::v025() { }
void CThingPhysics::v026() { }
void CThingPhysics::v027() { }
void CThingPhysics::v028() { }
void CThingPhysics::v029() { }
void CThingPhysics::v030() { }
void CThingPhysics::v031() { }
void CThingPhysics::v032() { }
void CThingPhysics::v033() { }
void CThingPhysics::v034() { }
void CThingPhysics::v035() { }
void CThingPhysics::v036() { }
void CThingPhysics::v037() { }
void CThingPhysics::v038() { }
void CThingPhysics::v039() { }
void CThingPhysics::v040() { }
void CThingPhysics::v041() { }
void CThingPhysics::v042() { }
void CThingPhysics::v043() { }
void CThingPhysics::v044() { }
void CThingPhysics::v045() { }
void CThingPhysics::v046() { }
void CThingPhysics::v047() { }
void CThingPhysics::v048() { }
void CThingPhysics::v049() { }
void CThingPhysics::v050() { }
void CThingPhysics::v051() { }
void CThingPhysics::v052() { }
void CThingPhysics::v053() { }
void CThingPhysics::v054() { }
void CThingPhysics::v055() { }
void CThingPhysics::v056() { }
void CThingPhysics::v057() { }
void CThingPhysics::v058() { }
void CThingPhysics::v059() { }
void CThingPhysics::v060() { }
void CThingPhysics::v061() { }
void CThingPhysics::v062() { }
void CThingPhysics::v063() { }
void CThingPhysics::v064() { }
void CThingPhysics::v065() { }
void CThingPhysics::v066() { }
void CThingPhysics::v067() { }
void CThingPhysics::v068() { }
void CThingPhysics::v069() { }
C3DVector CThingPhysics::GetFacingVector() const { return C3DVector(0, 0, 0); }
struct CTestPhysics : CThingPhysics {
    C3DVector Facing;
    C3DVector GetFacingVector() const { return Facing; }
};
void C3DVector::Normalise() { float l = (float)sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
static float g_focalT = -1;
C3DVector CTCGraphicAppearance::GetCameraFocalPos(float t) const { g_focalT = t; return C3DVector(100 + t, 200, 300); }
static C3DVector g_thingPos(10, 20, 5);
const C3DVector& CThing::GetPosition() const { return g_thingPos; }
static CThingGameObject* g_thing; static int g_observedCalls = 0;
void NCameraModes::CBase::v00() { }
const CThingGameObject* NCameraModes::CBase::GetObservedThing() const { ++g_observedCalls; return g_thing; }
int main()
{
    g_thing = (CThingGameObject*)calloc(1, 0x100);
    CTestPhysics physics; physics.Facing = C3DVector(3, 4, 7);
    g_thing->m_pPhysics = &physics;
    CTCGraphicAppearance appearance; g_thing->m_pGraphicAppearance = &appearance;
    NCameraModes::CStringAndCageBase* cam = (NCameraModes::CStringAndCageBase*)calloc(1, sizeof(NCameraModes::CStringAndCageBase));
    cam->DesiredStringLength = 5.0f; cam->DesiredHeightOffset = 2.0f;
    cam->OnReset();
    CHECK(fabs(cam->Position.X - 7.0f) < 1e-4 && fabs(cam->Position.Y - 16.0f) < 1e-4 && fabs(cam->Position.Z - 7.0f) < 1e-4);
    CHECK(cam->LookAt.X == 101.0f && cam->LookAt.Y == 200.0f && cam->LookAt.Z == 300.0f);
    CHECK(g_focalT == 1.0f && g_observedCalls == 3);
    if (fails == 0) printf("00871ec0_TEST PASS\n");
    return fails;
}
