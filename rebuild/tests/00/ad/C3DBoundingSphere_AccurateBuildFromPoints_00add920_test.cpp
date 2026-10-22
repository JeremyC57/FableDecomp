// Behaviour test for C3DBoundingSphere::AccurateBuildFromPoints (0x00ADD920): model and body verbatim from final.cpp
// (the same-TU FastBuildFromPoints stand-in included); checks the refined sphere still contains every point, is no worse than
// the fast sphere, converges on a symmetric set, and leaves the sphere alone for count == 0.
#include <stdio.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#define _snprintf snprintf
#endif
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float Radius;       // +0x0c
    void FastBuildFromPoints(const C3DVector* points, long count);                 // 0x00ADD690
    void AccurateBuildFromPoints(const C3DVector* points, long count, float accuracy);
};

__declspec(noinline) void C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count)
{
    C3DVector sum(0.0f, 0.0f, 0.0f);
    for (long i = 0; i < count; ++i)
        sum += points[i];
    float inv = count ? 1.0f / (float)count : 0.0f;
    Center = sum * inv;
    float radiusSquared = 0.0f;
    for (long j = 0; j < count; ++j) {
        float d = (points[j] - Center).GetLengthSquared();
        if (d > radiusSquared)
            radiusSquared = d;
    }
    Radius = (float)sqrt(radiusSquared);
}
void C3DBoundingSphere::AccurateBuildFromPoints(const C3DVector* points, long count, float accuracy)
{
    FastBuildFromPoints(points, count);
    if (count == 0)
        return;
    float bestRadiusSquared = 1e30f;
    C3DVector bestCenter;
    bestCenter.X = Center.X;
    bestCenter.Y = Center.Y;
    bestCenter.Z = Center.Z;
    float step = 0.25f;
    if (step > accuracy) do {
        float d;
        float farthest = 0.0f;
        unsigned long farthestIndex = 0;
        unsigned long i = 0;
        if (count >= 4) {
            do {
                d = (points[i] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i; }
                d = (points[i + 1] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 1; }
                d = (points[i + 2] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 2; }
                d = (points[i + 3] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 3; }
                i += 4;
            } while (i < (unsigned long)(count - 3));
        }
        for (; i < (unsigned long)count; ++i) {
            d = (points[i] - Center).GetLengthSquared();
            if (d > farthest) { farthest = d; farthestIndex = i; }
        }
        if (farthest < bestRadiusSquared) {
            bestRadiusSquared = farthest;
            bestCenter = Center;
            Center += (points[farthestIndex] - Center) * step;
        } else {
            Center += (points[farthestIndex] - Center) * step;
            step *= 0.5;
        }
    } while (step > accuracy);
    Center = bestCenter;
    Radius = (float)sqrt(bestRadiusSquared);
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static float Abs(float v) { return v < 0.0f ? -v : v; }
static float Dist(const C3DVector& a, const C3DVector& b) { return (float)sqrt((a - b).GetLengthSquared()); }
int main()
{
    C3DVector cube[8];
    int n = 0;
    for (int x = 0; x < 2; ++x) for (int y = 0; y < 2; ++y) for (int z = 0; z < 2; ++z)
        cube[n++] = C3DVector(x ? 3.0f : 1.0f, y ? 7.0f : 1.0f, z ? 5.0f : 1.0f);
    C3DBoundingSphere fast; fast.FastBuildFromPoints(cube, 8);
    C3DBoundingSphere s; s.AccurateBuildFromPoints(cube, 8, 0.0001f);
    for (int i = 0; i < 8; ++i)
        CHECK(Dist(cube[i], s.Center) <= s.Radius + 1e-3f);
    CHECK(s.Radius <= fast.Radius + 1e-4f);
    CHECK(Abs(s.Center.X - 2.0f) < 0.05f && Abs(s.Center.Y - 4.0f) < 0.05f && Abs(s.Center.Z - 3.0f) < 0.05f);
    // 5 points: exercises the unrolled block once and the remainder loop
    C3DVector five[5] = { C3DVector(0, 0, 0), C3DVector(10, 0, 0), C3DVector(0, 10, 0), C3DVector(0, 0, 10), C3DVector(10, 10, 10) };
    C3DBoundingSphere t; t.AccurateBuildFromPoints(five, 5, 0.001f);
    for (int j = 0; j < 5; ++j)
        CHECK(Dist(five[j], t.Center) <= t.Radius + 1e-3f);
    // a single point: zero radius
    C3DBoundingSphere one; one.AccurateBuildFromPoints(five + 1, 1, 0.001f);
    CHECK(Abs(one.Center.X - 10.0f) < 1e-4f && one.Radius < 1e-4f);
    // count == 0: the fast build runs, the refinement is skipped and the sphere is left as the fast build set it
    C3DBoundingSphere z; z.Center = C3DVector(9, 9, 9); z.Radius = 9.0f;
    z.AccurateBuildFromPoints(five, 0, 0.001f);
    CHECK(z.Center.X == 0.0f && z.Radius == 0.0f);
    char line[64];
    _snprintf(line, sizeof(line), "00add920_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}