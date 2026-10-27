// CPerlinNoise::Get3DNoise(const C3DVector& pos) const -- classic Perlin gradient noise: floor each coordinate to its lattice cell, hash the eight cell corners through the 256-entry permutation table (each step masked to 0..255), dot each corner's gradient with the offset to that corner and blend the eight values trilinearly with the s-curve t*t*(3-2t)
// Leaf: Get3DNoise
// Retail-asm: 0x00F361EF..0x00F3620F `fld [ebp - 0x18] ; fsub [ebp - 0x1c] ; fistp [ebp - 0x10] ; fild [ebp - 0x10] ; fadd [ebp + 8] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x18] ; cmp eax, [ebp - 0x14] ; jne 0xf3620c ; inc [ebp - 0x10] ; mov eax, [edx + 4] ; mov [ebp - 0x18], eax` and 0x00F36220..0x00F36240 `fld [ebp - 0x18] ; fsub [ebp - 0x1c] ; fistp [ebp - 0x14] ; fild [ebp - 0x14] ; fadd [ebp + 8] ; fstp [ebp - 0xc] ; mov eax, [ebp - 0x18] ; cmp eax, [ebp - 0xc] ; jne 0xf3623d ; inc [ebp - 0x14] ; mov eax, [edx + 8] ; mov [ebp - 0xc], eax` and 0x00F36251..0x00F36271 `fld [ebp - 0xc] ; fsub [ebp - 0x1c] ; fistp [ebp - 0x18] ; fild [ebp - 0x18] ; fadd [ebp + 8] ; fstp [ebp - 8] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 8] ; jne 0xf3626e ; inc [ebp - 0x18] ; fild [ebp - 0x10] ; mov esi, [ebp - 0x14]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F361D0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Three floor blocks (1.0f homed in the dead argument slot).
#pragma optimize("y", off)
struct C3DVector {
    float X, Y, Z;
};
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

static inline float SCurve(float t) { return (3.0f - (t + t)) * t * t; }
static inline float Lerp(float t, float a, float b) { return a + t * (b - a); }
class CPerlinNoise {
public:
    long      Permutation[256];                    // +0x000
    C3DVector Gradient[256];                       // +0x400
    float CornerValue(long hash, float rx, float ry, float rz) const
    {
        const C3DVector& g = Gradient[hash & 0xff];
        return rx * g.X + ry * g.Y + rz * g.Z;
    }
    float Get3DNoise(const C3DVector& pos) const;
};

float CPerlinNoise::Get3DNoise(const C3DVector& pos) const
{
    long bx0 = FloorToLong(pos.X);
    long by0 = FloorToLong(pos.Y);
    long bz0 = FloorToLong(pos.Z);
    float rx0 = pos.X - (float)bx0;
    float ry0 = pos.Y - (float)by0;
    float rz0 = pos.Z - (float)bz0;
    float rx1 = rx0 - 1.0f;
    float ry1 = ry0 - 1.0f;
    float rz1 = rz0 - 1.0f;
    long i = Permutation[bx0 & 0xff];
    long j = Permutation[(bx0 + 1) & 0xff];
    long b00 = Permutation[(i + by0) & 0xff];
    long b10 = Permutation[(j + by0) & 0xff];
    long b01 = Permutation[(i + by0 + 1) & 0xff];
    long b11 = Permutation[(j + by0 + 1) & 0xff];
    float sx = SCurve(rx0);
    float sy = SCurve(ry0);
    float sz = SCurve(rz0);
    float a = Lerp(sx, CornerValue(b00 + bz0, rx0, ry0, rz0), CornerValue(b10 + bz0, rx1, ry0, rz0));
    float b = Lerp(sx, CornerValue(b01 + bz0, rx0, ry1, rz0), CornerValue(b11 + bz0, rx1, ry1, rz0));
    float c = Lerp(sy, a, b);
    a = Lerp(sx, CornerValue(b00 + bz0 + 1, rx0, ry0, rz1), CornerValue(b10 + bz0 + 1, rx1, ry0, rz1));
    b = Lerp(sx, CornerValue(b01 + bz0 + 1, rx0, ry1, rz1), CornerValue(b11 + bz0 + 1, rx1, ry1, rz1));
    float d = Lerp(sy, a, b);
    return Lerp(sz, c, d);
}
