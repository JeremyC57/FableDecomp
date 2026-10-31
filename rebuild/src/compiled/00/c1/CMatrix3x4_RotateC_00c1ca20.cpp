// CMatrix3x4::RotateC(float angle) -- rotate the matrix in place about its Z axis: every row's (X, Y) pair becomes (c*X + s*Y, c*Y - s*X)
// Leaf: ?RotateC@CMatrix3x4@@QAEXM@Z
// Grade: MATCH (wave-2 sec6): the first row's s*Y product held in a named local (float t = s * Right.Y; Right.X = t + c
//   * Right.X) gives retail's s*X, s*Y, c*X order at the zero-displacement row; rows 2-4 unchanged
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1CA20, 138 bytes, thiscall `ret 4`.
// Identity (descriptive): the third of the libpk orientation helpers beside the landed RotateB 0x00C1C900 (Y/Z) and
//   RotateA 0x00C1C990 (X/Z), same 0x30-byte four-row matrix and same x87 shape; only X and Y move, so with Fable's
//   Z-up frame it is the yaw rotation. Its only caller 0x00C1CD8B (the unlisted builder at 0x00C1CD20..0x00C1CD91 that
//   sets up a rotation, calls RotateB 0x00C1C900, then this) passes its third argument. The method takes the next
//   letter of the landed descriptive names. Catalog "sub_00c1ca20" is a gapscan placeholder.
// Callees: none (cos/sin are the /O2 x87 intrinsics: `fld [esp+4] ; fcos ; fld [esp+4] ; fsin`).
// Shape: s*X is formed first and kept on the FPU stack because X is overwritten before Y is computed; the sum is
//   (s*Y) + (c*X) in that operand order. On the last row the s*X product is spilled into the angle's own argument slot
//   (`fstp [esp+4]` ... `fsub [esp+4]`), as in RotateB.
#include <math.h>

class C3DVector { public: float X, Y, Z; };            // 12 bytes, 1-aligned in retail

class CMatrix3x4 { public:                             // 0x30 bytes: four rows
    C3DVector Right;                            // +0x00
    C3DVector Up;                               // +0x0C
    C3DVector Forward;                          // +0x18
    C3DVector Position;                         // +0x24
    void RotateC(float angle);
};

void CMatrix3x4::RotateC(float angle)
{
    float c = cos(angle), s = sin(angle);
    float sx;
    sx = s * Right.X;    float t = s * Right.Y; Right.X = t + c * Right.X;    Right.Y    = c * Right.Y    - sx;
    sx = s * Up.X;       Up.X       = s * Up.Y       + c * Up.X;       Up.Y       = c * Up.Y       - sx;
    sx = s * Forward.X;  Forward.X  = s * Forward.Y  + c * Forward.X;  Forward.Y  = c * Forward.Y  - sx;
    sx = s * Position.X; Position.X = s * Position.Y + c * Position.X; Position.Y = c * Position.Y - sx;
}