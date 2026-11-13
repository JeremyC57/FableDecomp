// Behaviour test for CEngineClothRenderer::CalculateVertexNormals (0x00BB1F50): a host model of the normal accumulation -- each render
// triangle adds (p2 - p0) x (p1 - p0) to its three corners (through the render vertices' position indices), each render quad adds
// (p0 - p1) x (p2 - p1) to corners 0, 1, 2 and (p2 - p1) x (p3 - p1) to corners 1, 3, 2, W untouched, then the buffer is normalised.
// Mutation checks: four wrong models (swapped cross operands, the quad's second half on the wrong corner, the render-vertex remap
// skipped, W accumulated) must each fail the same checks.
#include <cstdio>
#include <cmath>

struct V4 { float X, Y, Z, W; };
struct V3 { float X, Y, Z; };
static V3 Sub(const V4& a, const V4& b) { V3 r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V3 Cross(const V3& a, const V3& b) { V3 r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }

enum { M_NONE, M_SWAP_CROSS, M_QUAD_CORNER, M_NO_REMAP, M_TOUCH_W, M_COUNT };
static int g_mutation = M_NONE;

static void Add(V4& n, const V3& c) { n.X += c.X; n.Y += c.Y; n.Z += c.Z; if (g_mutation == M_TOUCH_W) n.W += 1.0f; }
static unsigned long Remap(const unsigned short* remap, unsigned short i) { return g_mutation == M_NO_REMAP ? i : remap[i]; }

// The accumulation part (everything before GFNormalise3DVectors)
static void Accumulate(const V4* positions, V4* normals, unsigned long count, const unsigned short* remap,
                       const unsigned short (*tris)[3], unsigned long ntris, const unsigned short (*quads)[4], unsigned long nquads)
{
    for (unsigned long i = 0; i < count; ++i) { normals[i].X = normals[i].Y = normals[i].Z = normals[i].W = 0.0f; }   // GFClear4DVectors
    for (unsigned long t = 0; t < ntris; ++t) {
        unsigned long i0 = Remap(remap, tris[t][0]), i1 = Remap(remap, tris[t][1]), i2 = Remap(remap, tris[t][2]);
        V3 n = g_mutation == M_SWAP_CROSS ? Cross(Sub(positions[i1], positions[i0]), Sub(positions[i2], positions[i0]))
                                          : Cross(Sub(positions[i2], positions[i0]), Sub(positions[i1], positions[i0]));
        Add(normals[i0], n); Add(normals[i1], n); Add(normals[i2], n);
    }
    for (unsigned long q = 0; q < nquads; ++q) {
        unsigned long i0 = Remap(remap, quads[q][0]), i1 = Remap(remap, quads[q][1]), i2 = Remap(remap, quads[q][2]), i3 = Remap(remap, quads[q][3]);
        V3 n1 = Cross(Sub(positions[i0], positions[i1]), Sub(positions[i2], positions[i1]));
        Add(normals[i0], n1); Add(normals[i1], n1); Add(normals[i2], n1);
        V3 n2 = Cross(Sub(positions[i2], positions[i1]), Sub(positions[i3], positions[i1]));
        Add(normals[i1], n2); Add(normals[g_mutation == M_QUAD_CORNER ? i0 : i3], n2); Add(normals[i2], n2);
    }
}

static void Normalise(V4* normals, unsigned long count)                 // GFNormalise3DVectors
{
    for (unsigned long i = 0; i < count; ++i) {
        float l = std::sqrt(normals[i].X * normals[i].X + normals[i].Y * normals[i].Y + normals[i].Z * normals[i].Z);
        if (l > 0.0f) { normals[i].X /= l; normals[i].Y /= l; normals[i].Z /= l; }
    }
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static bool Is(const V4& v, float x, float y, float z, float w) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z) && Near(v.W, w); }

static bool Checks()
{
    bool ok = true;
    // 1) raw sums, non-planar quad + one triangle, through a non-identity render-vertex remap (render vertex r -> position remap[r])
    //    positions 0..3: the quad (0,0,0) (1,0,0) (0,1,0) (1,1,1); positions 4..6: the triangle (0,0,0) (1,0,0) (0,1,0)
    V4 p[7] = { { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 1, 1, 1, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 } };
    unsigned short remap[7] = { 6, 5, 4, 3, 2, 1, 0 };                  // render vertex r uses position 6 - r
    unsigned short tris[1][3] = { { 2, 1, 0 } };                        // positions 4, 5, 6
    unsigned short quads[1][4] = { { 6, 5, 4, 3 } };                    // positions 0, 1, 2, 3
    V4 n[7];
    Accumulate(p, n, 7, remap, tris, 1, quads, 1);
    // triangle: (p6 - p4) x (p5 - p4) = (0,1,0) x (1,0,0) = (0,0,-1) on positions 4, 5, 6
    // quad: n1 = (p0 - p1) x (p2 - p1) = (0,0,-1) on 0, 1, 2; n2 = (p2 - p1) x (p3 - p1) = (1,1,-1) on 1, 3, 2
    if (!Is(n[0], 0, 0, -1, 0) || !Is(n[1], 1, 1, -2, 0) || !Is(n[2], 1, 1, -2, 0) || !Is(n[3], 1, 1, -1, 0)) ok = false;
    if (!Is(n[4], 0, 0, -1, 0) || !Is(n[5], 0, 0, -1, 0) || !Is(n[6], 0, 0, -1, 0)) ok = false;
    Normalise(n, 7);
    const float r6 = 1.0f / std::sqrt(6.0f), r3 = 1.0f / std::sqrt(3.0f);
    if (!Is(n[1], r6, r6, -2 * r6, 0) || !Is(n[3], r3, r3, -r3, 0) || !Is(n[4], 0, 0, -1, 0)) ok = false;
    // 2) a flat unit quad in strip order and a flat triangle, identity remap: every normal is (0, 0, -1) after normalising
    V4 f[7] = { { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 1, 1, 0, 1 }, { 2, 0, 0, 1 }, { 3, 0, 0, 1 }, { 2, 1, 0, 1 } };
    unsigned short id[7] = { 0, 1, 2, 3, 4, 5, 6 };
    unsigned short ftris[1][3] = { { 4, 5, 6 } };
    unsigned short fquads[1][4] = { { 0, 1, 2, 3 } };
    Accumulate(f, n, 7, id, ftris, 1, fquads, 1);
    Normalise(n, 7);
    for (int i = 0; i < 7; ++i) if (!Is(n[i], 0, 0, -1, 0)) ok = false;
    // 3) no triangles, no quads: the cleared buffer stays zero (normalising a zero vector leaves it)
    Accumulate(f, n, 7, id, ftris, 0, fquads, 0);
    Normalise(n, 7);
    for (int i = 0; i < 7; ++i) if (!Is(n[i], 0, 0, 0, 0)) ok = false;
    return ok;
}

int main()
{
    g_mutation = M_NONE;
    if (!Checks()) { std::printf("00bb1f50_TEST FAIL\n"); return 1; }
    for (int m = M_NONE + 1; m < M_COUNT; ++m) {
        g_mutation = m;
        if (Checks()) { std::printf("00bb1f50_TEST FAIL\n"); return 1; }   // a wrong model must not pass
    }
    std::printf("00bb1f50_TEST PASS\n");
    return 0;
}
