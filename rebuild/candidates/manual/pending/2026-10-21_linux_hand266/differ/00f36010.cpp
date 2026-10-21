// Grade: DIFFER(412v442) -- retail floors both coordinates with the inline-asm `fld ; fsub 0.5 ; fistp` helper (frame-pointer function, 0.5/1.0 stored to the frame and the argument slot before each fistp): the documented asm_bake lane gap; one cast-form attempt
// CPerlinNoise::GetGaussian2DNoise(const C2DVector& p) -- 2D gradient (Perlin) noise at p: floor both coordinates, hash the four lattice corners through the permutation table, blend the four gradient dot products with the 3t^2-2t^3 fade
// Leaf: GetGaussian2DNoise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36010, 0x1BA bytes (padded to 448), `ret 4`, float in st0.
// Layout: local model: permutation table long[256] at +0 (masked & 0xff), gradient table of 12-byte entries at +0x400.
struct C2DVector { float X, Y; };
struct SGradient { float X, Y, Z; };
inline long FloorToLong(float value)
{
    long i = (long)(value - 0.5f);
    if (value == (float)i + 1.0f)
        ++i;
    return i;
}
class CPerlinNoise {
public:
    long      Permutation[256];                     // +0x000
    SGradient Gradients[256];                       // +0x400
    float GetGaussian2DNoise(const C2DVector& p);
};
float CPerlinNoise::GetGaussian2DNoise(const C2DVector& p)
{
    long ix = FloorToLong(p.X);
    long iy = FloorToLong(p.Y);
    float fx = p.X - (float)ix;
    float fy = p.Y - (float)iy;
    float fx1 = fx - 1.0f;
    float fy1 = fy - 1.0f;
    float sy = (3.0f - (fy + fy)) * fy * fy;
    long a = Permutation[ix & 0xff] + iy;
    long b = Permutation[(ix + 1) & 0xff] + iy;
    const SGradient& g00 = Gradients[Permutation[a & 0xff] & 0xff];
    const SGradient& g01 = Gradients[Permutation[(a + 1) & 0xff] & 0xff];
    const SGradient& g10 = Gradients[Permutation[b & 0xff] & 0xff];
    const SGradient& g11 = Gradients[Permutation[(b + 1) & 0xff] & 0xff];
    float n00 = g00.X * fx + g00.Y * fy;
    float n10 = g10.X * fx1 + g10.Y * fy;
    float n01 = g01.X * fx + g01.Y * fy1;
    float n11 = g11.X * fx1 + g11.Y * fy1;
    float sx = (3.0f - (fx + fx)) * fx * fx;
    float lo = n00 + (n10 - n00) * sx;
    float hi = n01 + (n11 - n01) * sx;
    return lo + (hi - lo) * sy;
}
