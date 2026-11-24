#define FABLE_RETAIL_NFRACTAL_CNOISE_DONOR_LERP_PASS26
#define FABLE_RETAIL_NFRACTAL_CNOISE_GRADIENT_GROUPING_PASS26
// Leaf: ?PerlinNoise@CNoise@NFractal@@QBEMABVC2DVector@@@Z
// Flags: (none)
#include "engine/NFractal_CNoise.h"

float NFractal::CNoise::PerlinNoise(const C2DVector& point) const
{
    long bx0, bx1, by0, by1, b00, b10, b01, b11, i, j;
    float rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v;

    t = point.X + 4096;
    bx0 = ((int)t) & 255;
    bx1 = (bx0 + 1) & 255;
    rx0 = t - (int)t;
    rx1 = rx0 - 1.0f;

    t = point.Y + 4096;
    by0 = ((int)t) & 255;
    by1 = (by0 + 1) & 255;
    ry0 = t - (int)t;
    ry1 = ry0 - 1.0f;

    i = P[bx0];
    j = P[bx1];

    b00 = P[i + by0];
    b10 = P[j + by0];
    b01 = P[i + by1];
    b11 = P[j + by1];

    sx = SCurve(rx0);
    sy = SCurve(ry0);

    u = At2(G2[b00], rx0, ry0);
    v = At2(G2[b10], rx1, ry0);
    a = Lerp(sx, u, v);

    u = At2(G2[b01], rx0, ry1);
    v = At2(G2[b11], rx1, ry1);
    b = Lerp(sx, u, v);

    return Lerp(sy, a, b);
}