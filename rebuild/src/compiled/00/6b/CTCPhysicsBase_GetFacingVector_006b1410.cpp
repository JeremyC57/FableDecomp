// CTCPhysicsBase::GetFacingVector() const -- returns the constant vector (0.0f, 1.0f, 0.0f)
// Leaf: ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1410, 27 bytes, thiscall, hidden return pointer, `ret 4`.
// Identity (proven): slot 70 of vtable 0x0125D2C4 (RTTI .?AVCTCPhysicsBase@@; also CTCPhysicsLight 0x01265DFC slot 70); no argument (`ret 4`), returns (0, 1, 0); Ego_r.pdb CTCPhysicsBase has exactly two C3DVector-returning virtuals with these signatures besides GetImpulse: ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ and the float overload, which VC7.1 places in reverse declaration order (the float one at slot 69, 0x006B1430).
// Shape (family M018, representative 0x00415CD0): `return C3DVector(x, y, z);` -- the constructor stores the three
//   immediates through the hidden return pointer (no NRVO question: nothing is named); `this` is unused.
// Header debt: engine/CTCPhysicsBase.h is a generated flat layout without the virtual interface, so the class is declared here
//   with the one virtual this TU defines.
#include "engine/C3DVector.h"
class CTCPhysicsBase {
public:
    virtual C3DVector GetFacingVector() const;
};
C3DVector CTCPhysicsBase::GetFacingVector() const
{
    return C3DVector(0.0f, 1.0f, 0.0f);
}