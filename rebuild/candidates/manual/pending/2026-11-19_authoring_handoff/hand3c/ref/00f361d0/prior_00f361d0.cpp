// Grade: DIFFER(744v873) 243 diff lines (parked 740v873, 338 lines; retail_asm candidate): the three lattice floors go through the engine's inline-asm floor helper GFFloatToLongFloor (expanded in place, ebp frame via optimize("y", off)); the prologue and all three floor blocks now line up with retail; left: the noise body -- retail's frame is 0x44 (ours 0x24) because it copies some of the gradient vectors into frame C3DVectors (first corner read through the pointer, later corners copied member-wise to [ebp-38h]/[ebp-2Ch]) and schedules the hash chain differently (tried: by-value gradient in CornerValue, Perlin's reused q local -- both worse)
// CPerlinNoise::Get3DNoise(const C3DVector& pos) const -- classic Perlin gradient noise: floor each coordinate to its lattice cell, hash the eight cell corners through the 256-entry permutation table (each step masked to 0..255), dot each corner's gradient with the offset to that corner and blend the eight values trilinearly with the s-curve t*t*(3-2t)
// Leaf: Get3DNoise
// Retail-asm: 0x00F361EF..0x00F36209 `fld [ebp-18h] ; fsub [ebp-1Ch] ; fistp dword ptr [ebp-10h] ; fild [ebp-10h] ; fadd [ebp+8] ; fstp [ebp-14h] ; mov eax,[ebp-18h] ; cmp eax,[ebp-14h] ; jne ; inc dword ptr [ebp-10h]` -- the engine's inline-assembly floor helper GFFloatToLongFloor (standalone copy 0x0048D940) expanded three times; VC7.1 emits no bare dword fistp from C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F361D0, 873 bytes (+ int3 padding), `ret 4`, ebp frame.
// Callees: none. 1.0f 0x0122DED8, 3.0f 0x0122DED4.
// Layout: CPerlinNoise: permutation table long[256] at +0x000, gradient table C3DVector[256] at +0x400.
#pragma optimize("y", off)
struct C3DVector {
    float X, Y, Z;
};
// the engine's inline-assembly FPU floor helper (standalone copy 0x0048D940, ?GFFloatToLongFloor@@YIJM@Z): expanded in place
__forceinline long GFFloatToLongFloor(float value)
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
    long bx0 = GFFloatToLongFloor(pos.X);
    long by0 = GFFloatToLongFloor(pos.Y);
    long bz0 = GFFloatToLongFloor(pos.Z);
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
