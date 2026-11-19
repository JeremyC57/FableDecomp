// C3DCoordI::operator+=(const C3DVector&) -- adds a float vector to the integer coordinate component-wise, truncating each sum back to long (__ftol2)
// Leaf: ??YC3DCoordI@@QAEABV0@ABVC3DVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13BE0, length 54 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (CRT float-to-long on the x87 stack, result in eax).
// PDB layout: C3DCoordI = {long X, long Y, long Z}; C3DVector = {float X, Y, Z}. Each component
// is `fild` + `fadd [v]` + ftol, and the next component's `fild` is scheduled before the store
// of the previous result. Returns *this (ret 4).
// Re-port (2026-11-19, hand3c landing): the local `class C3DCoordI {long X, Y, Z;}` model replaced by the new shared
//   engine/C3DCoordI.h (Ego_r layout: C2DCoordI base X / Y, long Z) under FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS. Same text, same bytes.
#define FABLE_RETAIL_C3DCOORDI_OPERATOR_PLUS_EQUALS
#include "engine/C3DVector.h"
#include "engine/C3DCoordI.h"

const C3DCoordI& C3DCoordI::operator+=(const C3DVector& v)
{
    X = (long)((float)X + v.X);
    Y = (long)((float)Y + v.Y);
    Z = (long)((float)Z + v.Z);
    return *this;
}