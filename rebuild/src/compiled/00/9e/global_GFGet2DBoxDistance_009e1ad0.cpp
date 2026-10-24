// GFGet2DBoxDistance(const C2DVector&, const C2DVector&) -- "box" (Chebyshev) distance between two 2D points: the larger of |dx| and |dy|
// Leaf: ?GFGet2DBoxDistance@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1AD0, length 35 bytes.
// PDB (FableWin): ?GFGet2DBoxDistance@@YIMABVC2DVector@@0@Z -- __fastcall, the points in ecx / edx, float result.
// The catalog label "Math_MaxAbsDelta" is descriptive. C2DVector = {X, Y} (PDB layout; no engine header exists).
// Callers: CTCScriptedControl::Update (0x00713A41, 0x00714059).
// Shape: two named float locals and a BY-VALUE max (`a > b ? a : b`): dx is computed first and stays on the FPU, dy is
// spilled to the `push ecx` frame slot for the `fcom [esp]` (`test ah,41h ; je` = `a > b`) and reloaded when it wins.
// A const-reference max homes BOTH temporaries (dy first, right-to-left arguments) and selects their address.
#include <math.h>

class C2DVector
{
public:
    float X;
    float Y;
};

template <class T>
inline T GFMax(T a, T b)
{
    return a > b ? a : b;
}

float __fastcall GFGet2DBoxDistance(const C2DVector& a, const C2DVector& b)
{
    float dx = (float)fabs(a.X - b.X);
    float dy = (float)fabs(a.Y - b.Y);
    return GFMax(dx, dy);
}