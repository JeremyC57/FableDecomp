// NCameraModes::CCameraPointModeBase::GetLookDirection() const -- the default look direction of a camera-point mode: the unit X axis
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E2B0, 21 bytes, `ret 4` (C3DVector returned through the hidden pointer).
// Leaf: ?GetLookDirection@CCameraPointModeBase@NCameraModes@@UBE?AVC3DVector@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Name: vtable 0x0124DDDC slot 29; FableWin.pdb ?GetLookDirection@CCameraPointModeBase@NCameraModes@@UBE?AVC3DVector@@XZ
//       (CSnapToPoint overrides the same slot 29 with 0x0056E3B0, returning its LookDirection member).
// Shape: a C3DVector with a user constructor is built straight into the caller's return slot (1.0f stored as an immediate,
//        the two zeros through ecx); eax already holds the hidden pointer.
#include "engine/C3DVector.h"
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
};
struct CCameraPointModeBase : CBase {
    virtual C3DVector GetLookDirection() const;
};
C3DVector CCameraPointModeBase::GetLookDirection() const
{
    return C3DVector(1.0f, 0.0f, 0.0f);
}
}