// Grade: DIFFER(319v321) -- lane gap: retail floors each coordinate with the inline-asm FloatToInt helper (`fld ; fsub 0.5 ; fistp` into the frame, frame-pointer function, the 0.5/1.0 constants stored as locals); plain C++ gives `_ftol` and no frame pointer
// CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const -- 2D gradient noise: floor the coordinates (round(x - 0.5), corrected when x is an exact integer), hash the four lattice corners through the 256-entry permutation table, blend the corner gradients' dot products with the 3t^2 - 2t^3 weight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36540, length 321 bytes, `ret 4`, frame-pointer function.
// Data: 3.0f 0x0122DED4 (the 3 - 2t constant), the permutation table at this+0 (256 longs) and the gradients at +0x400
// (12-byte records).
#include <math.h>
struct C2DVector { float X, Y; };
struct CPerlinNoise {
    long  Permutation[256];                             // +0x000
    float Gradients[256][3];                            // +0x400
    static long FloorToInt(float x)
    {
        long i = (long)(x - 0.5f);
        if (x == (float)i + 1.0f)
            ++i;
        return i;
    }
    float GetGaussian2DNoise(const C2DVector& pos) const;
};
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const
{
    long ix = FloorToInt(pos.X);
    long iy = FloorToInt(pos.Y);
    float fx = pos.X - (float)ix;
    float fy = pos.Y - (float)iy;
    long h0 = Permutation[ix & 0xff];
    long h1 = Permutation[(ix + 1) & 0xff];
    float sx = (3.0f - (fx + fx)) * fx * fx;
    float sy = (3.0f - (fy + fy)) * fy * fy;
    float g00 = Gradients[Permutation[(h0 + iy) & 0xff] & 0xff][0];
    float g01 = Gradients[Permutation[(h0 + iy + 1) & 0xff] & 0xff][0];
    float g10 = Gradients[Permutation[(h1 + iy) & 0xff] & 0xff][0];
    float g11 = Gradients[Permutation[(h1 + iy + 1) & 0xff] & 0xff][0];
    float a = g00 + (g10 - g00) * sx;
    float b = g01 + (g11 - g01) * sx;
    return a + (b - a) * sy;
}
