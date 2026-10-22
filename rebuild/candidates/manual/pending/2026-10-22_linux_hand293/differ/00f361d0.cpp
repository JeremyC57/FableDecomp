// Grade: DIFFER(740v873) -- lane gap: inline __asm floor (retail stores each coordinate and the 0.5f/1.0f constants to ebp-frame slots and floors with fld/fsub/fistp/fild/fadd/cmp/inc inside an __asm block, forcing the ebp frame); the compiled C++ cannot produce that body
// CPerlinNoise::Get3DNoise(const C3DVector& pos) const -- classic Perlin gradient noise: floor each coordinate to its lattice cell, hash the eight cell corners through the 256-entry permutation table (each step masked to 0..255), dot each corner's gradient with the offset to that corner and blend the eight values trilinearly with the s-curve t*t*(3-2t)
// Leaf: Get3DNoise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F361D0, 873 bytes (+ int3 padding), `ret 4`, ebp frame.
// Callees: none. 1.0f 0x0122DED8, 3.0f 0x0122DED4.
// Layout: CPerlinNoise: permutation table long[256] at +0x000, gradient table C3DVector[256] at +0x400.
// Note: retail floors each coordinate with an inline __asm block (`fld x; fsub 0.5; fistp n; fild n; fadd 1.0; cmp x,
//   that; jne; inc n` with the 0.5f/1.0f constants and the coordinate stored to frame slots first) -- that is what forces
//   the ebp frame and the memory constants. Not reconstructible without __asm: lane gap: inline __asm floor. This is the
//   behavioural C++ (floor via a compare-and-adjust cast).
struct C3DVector {
    float X, Y, Z;
};
static inline long FloorToLong(float x)
{
    long n = (long)x;
    if ((float)n > x)
        --n;
    return n;
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
