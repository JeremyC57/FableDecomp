// NFractal::CNoise::PerlinNoise(const C2DVector& position) const -- Ken Perlin's classic 2D gradient noise (noise2): lattice cell from the position offset by 4096, permutation lookups, gradient dot products at the four corners, s-curve weighted bilinear blend
// Grade: DIFFER(312v316) -- instruction scheduling: retail loads b11 = P[j + by1] at the last corner and folds `fmul [esi+eax*8+804h]` before the lea; ours hoists the b11 load into the s-curve.
// Leaf: ?PerlinNoise@CNoise@NFractal@@QBEMABVC2DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F35C60, 316 bytes (+ int3 padding), `ret 4`. FableWin.pdb ?PerlinNoise@CNoise@NFractal@@QBEMABVC2DVector@@@Z.
// Callees: __ftol2 0x00BFEA70 (the two (int) truncations).
// Constants: 4096.0f 0x012B8FC0 (N), 1.0f 0x0122DED8, 3.0f 0x0122DED4.
// Layout: as the landed InitPerlinNoise 0x00F35E00: permutation table P[512] at +0, gradient table G2[512] (C2DVector) at +0x800, Seed +0x1800.
#include "engine/C2DVector.h"

#define NOISE_B  0x100
#define NOISE_BM 0xff
#define NOISE_N  0x1000
#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(v, b0, b1, r0, r1) \
    t = v + NOISE_N;             \
    b0 = ((int)t) & NOISE_BM;    \
    b1 = (b0 + 1) & NOISE_BM;    \
    r0 = t - (int)t;             \
    r1 = r0 - 1.0f;


namespace NFractal {
inline float at2(const C2DVector* q, float rx, float ry) { return rx * q->X + ry * q->Y; }
class CNoise {
public:
    long          P[NOISE_B + NOISE_B];            // +0x0000
    C2DVector     G2[NOISE_B + NOISE_B];           // +0x0800
    unsigned long Seed;                            // +0x1800
    float PerlinNoise(const C2DVector& position) const;
};

float CNoise::PerlinNoise(const C2DVector& position) const
{
    int bx0, bx1, by0, by1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v;
    const C2DVector* q;
    int i, j;

    setup(position.X, bx0, bx1, rx0, rx1);
    setup(position.Y, by0, by1, ry0, ry1);

    i = P[bx0];
    j = P[bx1];

    b00 = P[i + by0];
    b10 = P[j + by0];
    b01 = P[i + by1];
    b11 = P[j + by1];

    sx = s_curve(rx0);
    sy = s_curve(ry0);

    q = &G2[b00]; u = at2(q, rx0, ry0);
    q = &G2[b10]; v = at2(q, rx1, ry0);
    a = lerp(sx, u, v);

    q = &G2[b01]; u = at2(q, rx0, ry1);
    q = &G2[b11]; v = at2(q, rx1, ry1);
    b = lerp(sx, u, v);

    return lerp(sy, a, b);
}
}   // namespace NFractal
