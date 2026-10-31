// Behaviour test for CPSCRenderMesh::GetComponentCost (0x00B00DB0): render sizes = round2(bits/2047*20); the larger (by
// squared magnitude) of start/end, scaled per axis by the system scale when ScaleParticles is set, gives a radius r; with a
// mesh, polys = triangles * particles, area = particles * r*r*pi, PolyCount += polys, PixelCount += area,
// RenderCost += materials * polys + area; a trail adds 2 polys per particle and width x min(length, 2)/2 x particles.
#include <cstdio>
#include <cmath>

static float Round2(float v) { float x = v * 100.0f; long r = (long)floor(x + 0.5f); return r / 100.0f; }
static float Size(unsigned b) { return Round2((float)b * (1.0f / 2047.0f) * 20.0f); }

struct Cost { float PolyCount, PixelCount, LightCount, RenderCost; };

static void MeshCost(Cost& c, const unsigned s[3], const unsigned e[3], bool scale, const float sc[3],
                     unsigned tris, unsigned mats, float n)
{
    float a[3], b[3];
    for (int i = 0; i < 3; ++i) { a[i] = Size(s[i]); b[i] = Size(e[i]); }
    float sa = a[0]*a[0] + a[1]*a[1] + a[2]*a[2], sb = b[0]*b[0] + b[1]*b[1] + b[2]*b[2];
    const float* v = sa > sb ? a : b;
    float x = v[0], y = v[1], z = v[2];
    if (scale) { x *= sc[0]; y *= sc[1]; z *= sc[2]; }
    float r = sqrtf(x*x + y*y + z*z);
    float polys = (float)tris * n, area = n * (r * r * 3.14159265f);
    c.PolyCount += polys; c.PixelCount += area; c.RenderCost += (float)mats * polys + area;
}

int main()
{
    int ok = 1;
    Cost c = { 0, 0, 0, 0 };
    unsigned s[3] = { 2047, 0, 0 }, e[3] = { 0, 1023, 0 };   // start 20 x 0 x 0 wins over end 0 x ~10 x 0
    float sc[3] = { 0.5f, 1, 1 };
    MeshCost(c, s, e, false, sc, 100, 2, 3.0f);
    ok &= c.PolyCount == 300.0f && fabs(c.PixelCount - 3.0f * 400.0f * 3.14159265f) < 0.5f;
    ok &= fabs(c.RenderCost - (600.0f + c.PixelCount)) < 0.5f;
    Cost d = { 0, 0, 0, 0 };
    MeshCost(d, s, e, true, sc, 100, 2, 1.0f);                // scaled radius 10
    ok &= fabs(d.PixelCount - 100.0f * 3.14159265f) < 0.1f;
    if (!ok) { std::printf("00b00db0_TEST FAIL\n"); return 1; }
    std::printf("00b00db0_TEST PASS\n");
    return 0;
}