// CGameCameraManager::PeekCurrentPos() const -- the camera's world position
// Leaf: ?PeekCurrentPos@CGameCameraManager@@UBE?BVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E1F0, 24 bytes, thiscall, `ret 4` (hidden C3DVector result).
// Identity (probable): slot 11 of vtable 0x0125C754 (RTTI .?AVCGameCameraManager@@); Ego_r.pdb has the virtual const
//   ?PeekCurrentPos@CGameCameraManager@@UBE?BVC3DVector@@XZ (a const C3DVector by value) and CGameCameraManager::Camera
//   (CCamera, +4) begins with CCamera::WorldPosition -- the 12 bytes copied here; the slot-to-name tie is the manifest's.
// Shape (family M199, representative 0x0069E1F0): `return Camera.WorldPosition;` -- C3DVector's member-wise copy constructor into
//   the hidden result (with the implicit copy VC7.1 allocates esi and pre-adds the offset: 30v24).
class C3DVector {                                          // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // the engine's member-wise copy (the implicit one allocates differently)
};
class CRightHandedSet { public: C3DVector Forward, Up; };
class CCamera {                                             // Ego_r.pdb
public:
    C3DVector WorldPosition;                              // +0x00
    CRightHandedSet RHSet;                                // +0x0C
};
class CGameCameraManager {
public:
    virtual const C3DVector PeekCurrentPos() const;
    CCamera Camera;                                       // +0x04
};
const C3DVector CGameCameraManager::PeekCurrentPos() const
{
    return Camera.WorldPosition;
}