// CGameCameraManager::PeekCurrentPos() const -- the camera's world position
// Leaf: ?PeekCurrentPos@CGameCameraManager@@UBE?BVC3DVector@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E1F0, 24 bytes, thiscall, `ret 4` (hidden C3DVector result).
// Identity (probable): slot 11 of vtable 0x0125C754 (RTTI .?AVCGameCameraManager@@); Ego_r.pdb has the virtual const
//   ?PeekCurrentPos@CGameCameraManager@@UBE?BVC3DVector@@XZ (a const C3DVector by value) and CGameCameraManager::Camera
//   (CCamera, +4) begins with CCamera::WorldPosition -- the 12 bytes copied here; the slot-to-name tie is the manifest's.
// Shape (family M199, representative 0x0069E1F0): `return Camera.WorldPosition;` -- C3DVector's member-wise copy constructor into
//   the hidden result (with the implicit copy VC7.1 allocates esi and pre-adds the offset: 30v24).
#include "engine/C3DVector.h"
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