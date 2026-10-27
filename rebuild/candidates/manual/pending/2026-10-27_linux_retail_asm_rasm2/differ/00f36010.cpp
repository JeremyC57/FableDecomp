// CPerlinNoise::GetGaussian2DNoise(const C2DVector& p) -- 2D gradient (Perlin) noise at p: floor both coordinates, hash the four lattice corners through the permutation table, blend the four gradient dot products with the 3t^2-2t^3 fade
// Leaf: GetGaussian2DNoise
// Retail-asm: 0x00F3602E..0x00F3604E `fld [ebp - 0x10] ; fsub [ebp - 0x14] ; fistp [ebp - 4] ; fild [ebp - 4] ; fadd [ebp + 8] ; fstp [ebp - 0xc] ; mov eax, [ebp - 0x10] ; cmp eax, [ebp - 0xc] ; jne 0xf3604b ; inc [ebp - 4] ; mov eax, [edx + 4] ; mov [ebp - 0x10], eax` and 0x00F3605F..0x00F3607F `fld [ebp - 0x10] ; fsub [ebp - 0x14] ; fistp [ebp - 8] ; fild [ebp - 8] ; fadd [ebp + 8] ; fstp [ebp - 0xc] ; mov eax, [ebp - 0x10] ; cmp eax, [ebp - 0xc] ; jne 0xf3607c ; inc [ebp - 8] ; fild [ebp - 4] ; mov esi, [ebp - 4]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36010; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Both coordinates are floor blocks (1.0f homed in the dead argument slot).
#pragma optimize("y", off)
struct C2DVector { float X, Y; };
struct SGradient { float X, Y, Z; };
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
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
