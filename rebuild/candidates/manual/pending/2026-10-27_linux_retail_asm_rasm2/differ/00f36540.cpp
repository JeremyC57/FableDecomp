// CPerlinNoise::GetGaussian2DNoise(const C2DVector& pos) const -- 2D gradient noise: floor the coordinates (round(x - 0.5), corrected when x is an exact integer), hash the four lattice corners through the 256-entry permutation table, blend the corner gradients' dot products with the 3t^2 - 2t^3 weight
// Retail-asm: 0x00F3655E..0x00F3657E `fld [ebp - 0xc] ; fsub [ebp - 4] ; fistp [ebp + 8] ; fild [ebp + 8] ; fadd [ebp - 8] ; fstp [ebp - 0x10] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 0x10] ; jne 0xf3657b ; inc [ebp + 8] ; mov eax, [edx + 4] ; mov [ebp - 8], eax` and 0x00F3658F..0x00F365AF `fld [ebp - 8] ; fsub [ebp - 0xc] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp - 0x10] ; fstp [ebp - 0x14] ; mov eax, [ebp - 8] ; cmp eax, [ebp - 0x14] ; jne 0xf365ac ; inc [ebp - 4] ; fild [ebp + 8] ; mov esi, [ebp + 8]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36540; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Both coordinates are floor blocks.
#pragma optimize("y", off)
#include <math.h>
struct C2DVector { float X, Y; };
struct CPerlinNoise {
    long  Permutation[256];                             // +0x000
    float Gradients[256][3];                            // +0x400
    static // the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
    // place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
    __forceinline long FloorToInt(float value)
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
