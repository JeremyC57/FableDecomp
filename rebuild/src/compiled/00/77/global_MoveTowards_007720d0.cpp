// MoveTowards(float current, float target, float step) -- step a float toward a target: snap to the target once it is closer than one step, otherwise move one step down or up toward it
// Leaf: ?MoveTowards@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007720D0, 63 bytes (+ int3 padding), `__stdcall` `ret 0Ch`, result in st(0).
// Callers: CTCShop::OnInitialActivate's neighbourhood 0x00775350 (three calls at 0x007754C4 / 0x00775508 / 0x00775546:
//   `current = MoveTowards(current, target, step)` on shop price values; the result is stored straight back).
// Constant: 0.0f @ 0x0122DEDC (the float literal pool).
// Label correction: catalogued as `Math_LerpFloat`; the body is a clamped step, not a lerp. Name is descriptive (no PDB
//   symbol found for this body).
// Shape: `diff = current - target; if (fabs(diff) < step) return target; if (diff > 0) return current - step;
//   return current + step;` -- the less-than test is `test ah,5 ; jp`, the greater-than one `test ah,41h ; jne`, and the
//   difference stays on the x87 stack across both compares.
#include <math.h>

float __stdcall MoveTowards(float current, float target, float step)
{
    float diff = current - target;
    if (fabs(diff) < step)
        return target;
    if (diff > 0.0f)
        return current - step;
    return current + step;
}