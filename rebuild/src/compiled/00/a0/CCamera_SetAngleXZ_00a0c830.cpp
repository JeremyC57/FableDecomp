// CCamera::SetAngleXZ(float angle) -- (unfinished in retail) rotates a local world-up vector by angle about the camera's forward axis and discards it
// Leaf: ?SetAngleXZ@CCamera@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C830, 51 bytes, thiscall, `ret 4`.
// Identity: the CCamera TU (0x00A0BE30 SetView .. 0x00A0C6F0 SetAngleYZ) follows Ego_r.pdb's CCamera member order
//   (GetAngleXY/YZ/XZ, SetAngleXY/YZ/XZ, LimitPos); this is the SetAngleXZ slot, which is also only 0x30 bytes in the
//   donor build (0x00B4D540..0x00B4D570). No direct call site in the image (xref: 0 calls / 0 references).
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60 (landed; rotates in place).
// Layout: Ego_r.pdb CCamera: WorldPosition +0, RHSet CRightHandedSet +0xC (Up +0xC, Forward +0x18), HeightLocked +0x24,
//   ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2C, VerticalFOV +0x30, Zoom +0x34 (the landed MoveForward
//   0x00A0C250 model).
// Shape: `C3DVector v(0, 0, 1); v.Rotate(RHSet.Forward, angle);` -- the 12-byte local is built after the two pushes
//   (axis = this + 0x18, the angle forwarded from its slot) and dropped; nothing is stored back into the camera.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   CCamera::SetAngleXZ.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
};

class CRightHandedSet {                                 // 24 bytes
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
};

class CCamera {
public:
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0C
    bool            HeightLocked;                       // +0x24
    bool            ViewVecZLocked;                     // +0x25
    unsigned long   FOVFlags;                           // +0x28
    float           HorizontalFOV;                      // +0x2C
    float           VerticalFOV;                        // +0x30
    float           Zoom;                               // +0x34
    void SetAngleXZ(float angle);
};

void CCamera::SetAngleXZ(float angle)
{
    C3DVector v(0.0f, 0.0f, 1.0f);
    v.Rotate(RHSet.Forward, angle);
}