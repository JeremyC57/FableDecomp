// Grade: IMPROVED(321v321) retry1 -- retail_asm FloorToLong + retail statement order; residue: h0/copy register roles
// CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const -- 2D gradient noise: floor the coordinates (round(x - 0.5), corrected when x is an exact integer), hash the four lattice corners through the 256-entry permutation table, blend the corner gradients' dot products with the 3t^2 - 2t^3 weight
// Retail-asm: 0x00F36561 `fsub [ebp-4]` + 0x00F36564 `fistp dword ptr [ebp+8]` -- the engine inline-assembly FloorToLong (landed 0x006A68F0), expanded twice; VC7.1 C++ (long)x calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36540, length 321 bytes, `ret 4`, frame-pointer function.
// Data: 3.0f 0x0122DED4 (the 3 - 2t constant), the permutation table at this+0 (256 longs) and the gradients at +0x400
// (12-byte records).
#include <math.h>
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
struct C2DVector { float X, Y; };
struct CPerlinNoise {
    long  Permutation[256];                             // +0x000
    float Gradients[256][3];                            // +0x400
    float GetGaussian2DNoise(const C2DVector& pos) const;
};
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const
{
    long ix = FloorToLong(pos.X);
    long iy = FloorToLong(pos.Y);
    float fx = pos.X - (float)ix;
    float fy = pos.Y - (float)iy;
    long h0 = Permutation[ix & 0xff] + iy;
    float sx = (3.0f - (fx + fx)) * fx * fx;
    float g00 = Gradients[Permutation[h0 & 0xff] & 0xff][0];
    float g01 = Gradients[Permutation[(h0 + 1) & 0xff] & 0xff][0];
    long h1 = Permutation[(ix + 1) & 0xff] + iy;
    float g10 = Gradients[Permutation[h1 & 0xff] & 0xff][0];
    float g11 = Gradients[Permutation[(h1 + 1) & 0xff] & 0xff][0];
    float sy = (3.0f - (fy + fy)) * fy * fy;
    float a = g00 + (g10 - g00) * sx;
    float b = g01 + (g11 - g01) * sx;
    return a + (b - a) * sy;
}
