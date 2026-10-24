// NParticleEngine::GFInterpolate(long a, long b, float t) -- integer linear interpolation a * (1 - t) + b * t, rounded to the nearest long
// Grade: DIFFER(61v55) -- retail_asm candidate: the rounding is the inline-asm `fld ; fistp dword ptr [esp+4]` GFFloatToLong helper, which VC7.1 cannot emit from C++; this is the C++ reference body.
// Leaf: ?GFInterpolate@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B05C20, length 55 bytes, __fastcall (a in ecx, b in edx, both spilled
//   for `fimul` / `fild`), `ret 4`.
// Signature from FableWin.pdb: ?GFInterpolate@NParticleEngine@@YIJJJM@Z (the long overload of the family). No direct
//   caller in retail (the CPSC InterpolateTo bodies inline it).
// Constants: 1.0f @ 0x0122DED8.
// Label: catalog "GFInterpolate" (_global) -- the namespace is NParticleEngine.
#include <math.h>

inline long GFFloatToLong(float f) { return (long)floor(f + 0.5f); }   // C++ stand-in for the inline-asm fld/fistp

namespace NParticleEngine {

long __fastcall GFInterpolate(long a, long b, float t)
{
    return GFFloatToLong((1.0f - t) * a + b * t);
}

} // namespace NParticleEngine
