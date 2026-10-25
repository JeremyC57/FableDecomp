// CMatrix3x4::RotateB(float angle) -- rotate the matrix in place about its X axis: every row's (Y, Z) pair becomes (c*Y + s*Z, c*Z - s*Y)
// Leaf: ?RotateB@CMatrix3x4@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C900, 143 bytes (`ret 4`). Descriptive name, kept from the landed caller
//   GetRotationAngles 0x00C1CC10 (call at 0x00C1CC6E; one more call at 0x00C1CD7F in an unlisted neighbour); the
//   manifest's "CMatrix3x4_RotateYaw" is propagated and wrong for the axis (only Y and Z move, X is untouched -- with Fable's Z-up frame this is a
//   rotation about X, i.e. pitch). Sibling of RotateA 0x00C1C990 (the X/Z rotation).
// Callees: none (cos/sin are the /O2 x87 intrinsics: `fld [esp+4] ; fcos ; fld [esp+4] ; fsin`).
// Shape: s*Y is formed first and kept on the FPU stack because Y is overwritten before Z is computed; the sum is
//   (s*Z) + (c*Y) in that operand order. On the last row the s*Y product is spilled into the angle's own argument slot
//   (`fstp [esp+4]` ... `fsub [esp+4]`).
#include <math.h>

struct C3DVector { float X, Y, Z; };            // 12 bytes, 1-aligned in retail

struct CMatrix3x4 {                             // 0x30 bytes: four rows
    C3DVector Right;                            // +0x00
    C3DVector Up;                               // +0x0C
    C3DVector Forward;                          // +0x18
    C3DVector Position;                         // +0x24
    void RotateB(float angle);
};

void CMatrix3x4::RotateB(float angle)
{
    float c = cos(angle), s = sin(angle);
    float sy;
    sy = s * Right.Y;    Right.Y    = s * Right.Z    + c * Right.Y;    Right.Z    = c * Right.Z    - sy;
    sy = s * Up.Y;       Up.Y       = s * Up.Z       + c * Up.Y;       Up.Z       = c * Up.Z       - sy;
    sy = s * Forward.Y;  Forward.Y  = s * Forward.Z  + c * Forward.Y;  Forward.Z  = c * Forward.Z  - sy;
    sy = s * Position.Y; Position.Y = s * Position.Z + c * Position.Y; Position.Z = c * Position.Z - sy;
}