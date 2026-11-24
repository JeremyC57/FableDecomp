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

#include <cmath>
#include <cstdio>

static int failures = 0;

static void Check(bool condition, int line)
{
    if (!condition)
    {
        ++failures;
        std::printf("FAIL %d\n", line);
    }
}

static double Oracle(const NFractal::CNoise& noise, const C2DVector& point)
{
    const float x = point.X + 4096;
    const float y = point.Y + 4096;
    const int ix = (int)x;
    const int iy = (int)y;
    const double fx = x - ix;
    const double fy = y - iy;
    const double sx = fx * fx * (3 - 2 * fx);
    const double sy = fy * fy * (3 - 2 * fy);
    double result = 0.0;
    for (int dx = 0; dx != 2; ++dx)
    {
        for (int dy = 0; dy != 2; ++dy)
        {
            const long hash = noise.P[noise.P[(ix + dx) & 255] + ((iy + dy) & 255)];
            const C2DVector& gradient = noise.G2[hash];
            const double dot = (fx - dx) * gradient.X + (fy - dy) * gradient.Y;
            const double wx = dx ? sx : 1.0 - sx;
            const double wy = dy ? sy : 1.0 - sy;
            result += wx * wy * dot;
        }
    }
    return result;
}

int main()
{
    NFractal::CNoise noise;
    const C2DVector gradients[] = {
        C2DVector(1.0f, 0.0f), C2DVector(0.0f, -1.0f),
        C2DVector(-0.6f, 0.8f), C2DVector(0.8f, 0.6f),
        C2DVector(-1.0f, 0.0f), C2DVector(0.0f, 1.0f)
    };
    for (int i = 0; i < 512; ++i)
    {
        const int index = i & 255;
        noise.P[i] = (73 * index + 19) & 255;
        noise.G2[i] = gradients[index % 6];
    }
    const float coordinates[] = {
        -4096.0f, -4095.75f, -3840.25f, -3840.0f, -3839.75f,
        -100.25f, -2.0f, -0.75f, -0.5f, 0.0f, 0.25f, 0.5f,
        1.0f, 2.75f, 100.25f, 255.75f, 256.25f
    };
    const unsigned count = sizeof(coordinates) / sizeof(coordinates[0]);
    const double tolerance = 0.00001;
    for (unsigned x = 0; x < count; ++x)
    {
        for (unsigned y = 0; y < count; ++y)
        {
            const C2DVector point(coordinates[x], coordinates[y]);
            const C2DVector nextX(point.X + 256, point.Y);
            const C2DVector nextY(point.X, point.Y + 256);
            const float observed = noise.PerlinNoise(point);
            Check(std::fabs(observed - Oracle(noise, point)) < tolerance, __LINE__);
            Check(std::fabs(observed - noise.PerlinNoise(nextX)) < tolerance, __LINE__);
            Check(std::fabs(observed - noise.PerlinNoise(nextY)) < tolerance, __LINE__);
        }
    }
    for (int x = -4; x <= 4; ++x)
    {
        for (int y = -4; y <= 4; ++y)
            Check(noise.PerlinNoise(C2DVector((float)x, (float)y)) == 0.0f, __LINE__);
    }
    std::puts(failures ? "00f35c60_TEST FAIL" : "00f35c60_TEST PASS");
    return failures ? 1 : 0;
}