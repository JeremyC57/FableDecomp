// CMatrix3x4::RotateA(float angle) -- rotate the matrix in place about its Y axis: every row's (X, Z) pair becomes (c*X - s*Z, c*Z + s*X)
// Leaf: ?RotateA@CMatrix3x4@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C990, 142 bytes (`ret 4`). Descriptive name, kept from the landed caller
//   GetRotationAngles 0x00C1CC10 (its only caller, at 0x00C1CC48, rotating a working copy by -first). The manifest's
//   "CMatrix3x4_RotateYawSparse" is propagated: only X and Z move (Y is untouched), a rotation about Y.
//   Sibling of RotateB 0x00C1C900 (the Y/Z rotation).
// Callees: none (cos/sin are the /O2 x87 intrinsics).
// Shape: s*X is formed first and kept on the FPU stack because X is overwritten before Z; the last row spills it into
//   the angle's argument slot and needs one `fxch`.
#include <math.h>

class C3DVector { public: float X, Y, Z; };            // 12 bytes, 1-aligned in retail

class CMatrix3x4 { public:                             // 0x30 bytes: four rows
    C3DVector Right;                            // +0x00
    C3DVector Up;                               // +0x0C
    C3DVector Forward;                          // +0x18
    C3DVector Position;                         // +0x24
    void RotateA(float angle);
};

void CMatrix3x4::RotateA(float angle)
{
    float c = cos(angle), s = sin(angle);
    float sx;
    sx = s * Right.X;    Right.X    = c * Right.X    - s * Right.Z;    Right.Z    = c * Right.Z    + sx;
    sx = s * Up.X;       Up.X       = c * Up.X       - s * Up.Z;       Up.Z       = c * Up.Z       + sx;
    sx = s * Forward.X;  Forward.X  = c * Forward.X  - s * Forward.Z;  Forward.Z  = c * Forward.Z  + sx;
    sx = s * Position.X; Position.X = c * Position.X - s * Position.Z; Position.Z = c * Position.Z + sx;
}