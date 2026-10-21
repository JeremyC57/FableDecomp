// CGameCameraManager::UpdateRightStickLookAround() -- ease the right-stick look-around angles toward their targets ((angles * 3 + target) / 4) and, when the current mode allows it, turn the camera: yaw forward and the side vector about up, pitch forward and up about the side vector, renormalise and set the view
// Grade: DIFFER(ours v retail) -- scheduling only: in the cross product's Y term the `lea ecx`/`push ecx` of the first Rotate call interleave one x87 instruction later than retail (same instructions, same operands)
// Leaf: UpdateRightStickLookAround
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697CC0, length 346 bytes (`ret`).
// Callees: the current camera mode's virtual slot 4 (+0x10, a bool), C3DVector::Rotate(const C3DVector& axis, float angle)
//   @ 0x00A13C60, C3DVector::Normalise @ 0x00A14440 (manifest "GetScaled" propagated), CCamera::SetView(const C3DVector&,
//   const C3DVector&) @ 0x00A0BE30; constants 3.0f @ 0x0122DED4, 0.25f @ 0x0125C1E4.
// Layout: Camera +4 (CCamera: WorldPosition, then RHSet Up +0x10 / Forward +0x1C), the current mode +0x98, and the
//   right-stick angle pairs at +0xC4 / +0xCC (12 bytes below engine/CGameCameraManager.h's PDB offsets: retail containers
//   are smaller, so a local model).
// Shape: the 2D easing uses C2DVector operators; forward is copied before up (memberwise C3DVector copy ctor); the side
//   vector is an inline CrossProduct (x87 operand order follows the copy order, the X term written v.Z * Y - v.Y * Z).
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector CrossProduct(const C3DVector& v) const { return C3DVector(v.Z * Y - v.Y * Z, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    void Normalise();                                   // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCamera {
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0C
    void SetView(const C3DVector& forward, const C3DVector& up);   // 0x00A0BE30
};
struct CCameraModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool AllowsLookAround();                    // slot 4 (+0x10)
};
struct CGameCameraManager {
    void*            __vftable;                         // +0x00
    CCamera          Camera;                            // +0x04
    unsigned char    _pad28[0x70];
    CCameraModeBase* CurrentMode;                       // +0x98
    unsigned char    _pad9c[0x28];
    C2DVector        RightStickLookAroundAngles;        // +0xC4
    C2DVector        RightStickTargetLookAroundAngles;  // +0xCC
    void UpdateRightStickLookAround();
};

void CGameCameraManager::UpdateRightStickLookAround()
{
    RightStickLookAroundAngles = (RightStickLookAroundAngles * 3.0f + RightStickTargetLookAroundAngles) * 0.25f;
    if (!CurrentMode->AllowsLookAround())
        return;
    C3DVector forward = Camera.RHSet.Forward;
    C3DVector up = Camera.RHSet.Up;
    C3DVector side = forward.CrossProduct(up);
    float yaw = RightStickLookAroundAngles.X;
    forward.Rotate(up, yaw);
    side.Rotate(up, yaw);
    float pitch = RightStickLookAroundAngles.Y;
    forward.Rotate(side, pitch);
    up.Rotate(side, pitch);
    forward.Normalise();
    up.Normalise();
    Camera.SetView(forward, up);
}
