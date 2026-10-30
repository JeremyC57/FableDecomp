// Grade: DIFFER(444v442) -- the floor blocks are the landed FloorToLong retail_asm helper; body in retail order with g00 by reference and 2D copies of the other three gradients; residue: frame packing (0x24 v 0x38) and the hash/gradient interleave
// CPerlinNoise::GetGaussian2DNoise(const C2DVector& p) -- 2D gradient (Perlin) noise at p: floor both coordinates, hash the four lattice corners through the permutation table, blend the four gradient dot products with the 3t^2-2t^3 fade
// Retail-asm: 0x00F3602E fld dword ptr [ebp-10h] ; fsub dword ptr [ebp-14h] ; fistp dword ptr [ebp-4] ; inc dword ptr [ebp-4] -- the landed FloorToLong helper (0x006A68F0), expanded twice
// Leaf: GetGaussian2DNoise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36010, 0x1BA bytes (padded to 448), `ret 4`, float in st0.
// Layout: local model: permutation table long[256] at +0 (masked & 0xff), gradient table of 12-byte entries at +0x400.
struct C2DVector { float X, Y; };
struct SGradient { float X, Y, Z; const C2DVector& XY() const { return *(const C2DVector*)this; } };
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
    float sx = (3.0f - (fx + fx)) * fx * fx;
    long a = Permutation[ix & 0xff] + iy;
    const SGradient& g00 = Gradients[Permutation[a & 0xff] & 0xff];
    long b = Permutation[(ix + 1) & 0xff] + iy;
    C2DVector g10 = Gradients[Permutation[b & 0xff] & 0xff].XY();
    C2DVector g01 = Gradients[Permutation[(a + 1) & 0xff] & 0xff].XY();
    C2DVector g11 = Gradients[Permutation[(b + 1) & 0xff] & 0xff].XY();
    float n00 = g00.X * fx + g00.Y * fy;
    float n01 = g01.X * fx + g01.Y * fy1;
    float lo = n00 + (g10.X * fx1 + g10.Y * fy - n00) * sx;
    float hi = n01 + (g11.X * fx1 + g11.Y * fy1 - n01) * sx;
    return lo + (hi - lo) * ((3.0f - (fy + fy)) * fy * fy);
}
