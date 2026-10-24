// GFGet2DDistance(const C2DCoordI&, const C2DCoordI&) -- distance between two integer 2D coordinates: sqrt(dx*dx + dy*dy) as float
// Leaf: ?GFGet2DDistance@@YIMABVC2DCoordI@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2430, length 33 bytes. PDB: ?GFGet2DDistance@@YIMABVC2DCoordI@@0@Z
//         (float __fastcall GFGet2DDistance(const C2DCoordI&, const C2DCoordI&); a in ecx, b in edx).
// Callees: none (the CRT sqrt is the intrinsic `fsqrt`, `#pragma intrinsic(sqrt)`).
// Shape: integer differences, both squares summed with `imul` (dy squared first: the right operand of `+`), the sum
//   spilled to the `push ecx` frame word for `fild`, then `fsqrt` and the float return on the x87 stack.
#include <math.h>
#pragma intrinsic(sqrt)

class C2DCoordI                                      // PDB: 8 bytes {X, Y}
{
public:
    long X;
    long Y;
};

float __fastcall GFGet2DDistance(const C2DCoordI& a, const C2DCoordI& b)
{
    long dx = a.X - b.X;
    long dy = a.Y - b.Y;
    return (float)sqrt((double)(dx * dx + dy * dy));
}