// CGameCameraManager::UpdateRightStickLookAround() -- ease the right-stick look-around angles toward their targets ((angles * 3 + target) / 4) and, when the current camera mode uses right-stick look-around, yaw forward and the side vector about up, pitch forward and up about the side vector, renormalise and set the view
// Grade: DIFFER(346v346) -- scheduling only: in the side vector's Y term the first Rotate call's `lea ecx,[esp+14h]`/`push ecx` come one x87 instruction late (retail `fld ; lea ; fmul ; push`, ours `lea ; fld ; push ; fmul`) | this session: PDB respell (private, C3DVector::Cross, AccurateNormalise, IsUseRightStickLookAround), PeekViewDirection/PeekUpDirection accessors (identical), CRightHandedSet local copy (worse), inline CRightHandedSet::Rotate/Normalise (worse)
// Leaf: ?UpdateRightStickLookAround@CGameCameraManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697CC0, 346 bytes, `ret`. Ego_r.pdb ?UpdateRightStickLookAround@CGameCameraManager@@AAEXXZ
//   (private; the bound spelling ?UpdateRightStickLookAround@CGameCameraManager@@QAEXXZ is wrong: NAME_FIXES.tsv).
// Callees: NCameraModes::CBase slot 4 IsUseRightStickLookAround() const (vtable 0x0124DD74 slot 4 = 0x0056E160; overridden in
//   CZTargetNew / CCameraPointModeBase / CSimplePointBase, all FableWin ?IsUseRightStickLookAround@...@@UBE_NXZ),
//   C3DVector::Rotate(const C3DVector&, float) 0x00A13C60, C3DVector::AccurateNormalise() 0x00A14440 (sqrt + divide body,
//   Ego_r out-of-line name set; the majority binding), CCamera::SetView(const C3DVector&, const C3DVector&) 0x00A0BE30
//   (Ego_r ?SetView@CCamera@@QAEXABVC3DVector@@0@Z); 3.0f 0x0122DED4, 0.25f 0x0125C1E4.
// Layout: Camera +0x04 (CCamera: WorldPosition, RHSet {Up +0x10, Forward +0x1C}), CurrentMode +0x98 (CCountedPointer data
//   word), RightStickLookAroundAngles +0xC4, RightStickTargetLookAroundAngles +0xCC (engine/CGameCameraManager.h offsets;
//   the header flattens the vptr and Camera, so a local model).
class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    const C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector Cross(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    C3DVector& AccurateNormalise();                     // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
};
class CRightHandedSet {
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    const C3DVector& GetUp() const { return Up; }
    const C3DVector& GetForward() const { return Forward; }
};
class CCamera {
public:
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0C
    unsigned char   _pad24[0x38 - 0x24];
    const C3DVector& PeekViewDirection() const { return RHSet.GetForward(); }
    const C3DVector& PeekUpDirection() const { return RHSet.GetUp(); }
    void SetView(const C3DVector& forward, const C3DVector& up);   // 0x00A0BE30
};
namespace NCameraModes {
class CBase {
public:
    virtual ~CBase();
    virtual void _slot1();
    virtual void _slot2();
    virtual void _slot3();
    virtual bool IsUseRightStickLookAround() const;     // slot 4 (+0x10) 0x0056E160
};
}
class CGameCameraManager {
public:
    virtual ~CGameCameraManager();
    CCamera              Camera;                        // +0x04
    unsigned char        _pad3c[0x98 - 0x3c];
    NCameraModes::CBase* CurrentMode;                   // +0x98 (CCountedPointer<NCameraModes::CBase> data word)
    void*                CurrentMode_Info;              // +0x9c
    unsigned char        _pada0[0xc4 - 0xa0];
    C2DVector            RightStickLookAroundAngles;        // +0xC4
    C2DVector            RightStickTargetLookAroundAngles;  // +0xCC
private:
    void UpdateRightStickLookAround();
};

void CGameCameraManager::UpdateRightStickLookAround()
{
    RightStickLookAroundAngles = (RightStickLookAroundAngles * 3.0f + RightStickTargetLookAroundAngles) * 0.25f;
    if (!CurrentMode->IsUseRightStickLookAround())
        return;
    C3DVector forward = Camera.RHSet.Forward;
    C3DVector up = Camera.RHSet.Up;
    C3DVector side = forward.Cross(up);
    float yaw = RightStickLookAroundAngles.X;
    forward.Rotate(up, yaw);
    side.Rotate(up, yaw);
    float pitch = RightStickLookAroundAngles.Y;
    forward.Rotate(side, pitch);
    up.Rotate(side, pitch);
    forward.AccurateNormalise();
    up.AccurateNormalise();
    Camera.SetView(forward, up);
}
