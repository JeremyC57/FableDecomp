// GFGet2DDistance(const C2DVector&, const C2DVector&) -- distance between two float 2D vectors: sqrt(dx*dx + dy*dy)
// Leaf: ?GFGet2DDistance@@YIMABVC2DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2460, length 27 bytes. PDB: ?GFGet2DDistance@@YIMABVC2DVector@@0@Z
//         (float __fastcall GFGet2DDistance(const C2DVector&, const C2DVector&); a in ecx, b in edx).
// Callees: none (`#pragma intrinsic(sqrt)` gives the inline `fsqrt`).
// Identity: the catalog label "Calc_Distance2D" is descriptive; the PDB has the C2DVector overload of GFGet2DDistance
//   next to the C2DCoordI one (0x009E2430) in the same order as retail.
// Shape: both differences stay on the x87 stack; `dx*dx + dy*dy` written in one expression squares the top (dy)
//   first (`fld st(0) ; fmul st(1)` then `fld st(2) ; fmul st(3)`), and the two dead differences are popped after
//   the `fsqrt` (`fstp st(2) ; fstp st(0)`).
#include <math.h>
#pragma intrinsic(sqrt)

class C2DVector                                      // PDB: 8 bytes {X, Y}
{
public:
    float X;
    float Y;
};

float __fastcall GFGet2DDistance(const C2DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}