// CNoise::PerlinNoise(const C2DVector& position) const -- Ken Perlin's classic 2D gradient noise (noise2): lattice cell from the position offset by 4096, permutation lookups, gradient dot products at the four corners, s-curve weighted bilinear blend
// Grade: DIFFER(312v316) -- instruction scheduling only: retail loads b11 = P[j + by1] at the last corner and addresses its gradient's [1] as [esi+eax*8+804h] before the lea; ours hoists the b11 load into the s-curve and uses lea + [eax+4]
// Leaf: ?PerlinNoise@CNoise@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F35C60, 316 bytes (+ int3 padding), `ret 4`.
// Callees: __ftol2 0x00BFEA70 (the two (int) truncations).
// Constants: 4096.0f 0x012B8FC0 (N), 1.0f 0x0122DED8, 3.0f 0x0122DED4.
// Layout: permutation table P[512] (ints) at +0, gradient table G2[][2] (floats) at +0x800.
// Shape: the reference noise2 with float macros (s_curve as t*t*(3 - 2t), lerp a + t*(b - a)); sy is evaluated at
//   the final lerp (the macro expansion), the second axis's fraction goes through fild/fsubr (register pressure).
struct C2DVector { float X, Y; };

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
#define at2(rx, ry) ( rx * q[0] + ry * q[1] )

class CNoise {
public:
    int   P[NOISE_B + NOISE_B];                    // +0x000
    float G2[NOISE_B + NOISE_B + 2][2];            // +0x800
    float PerlinNoise(const C2DVector& position) const;
};

float CNoise::PerlinNoise(const C2DVector& position) const
{
    int bx0, bx1, by0, by1, b00, b10, b01, b11;
    float rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v;
    const float* q;
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

    q = G2[b00]; u = at2(rx0, ry0);
    q = G2[b10]; v = at2(rx1, ry0);
    a = lerp(sx, u, v);

    q = G2[b01]; u = at2(rx0, ry1);
    q = G2[b11]; v = at2(rx1, ry1);
    b = lerp(sx, u, v);

    return lerp(sy, a, b);
}
