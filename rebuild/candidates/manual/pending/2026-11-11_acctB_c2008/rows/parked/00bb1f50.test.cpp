// Behaviour test for CEngineClothRenderer::CalculateVertexNormals (0x00BB1F50): a host model of the normal accumulation -- each render
// triangle adds (p2 - p0) x (p1 - p0) to its three corners (through the render vertices' position indices), each render quad adds
// (p0 - p1) x (p2 - p1) to corners 0, 1, 2 and (p2 - p1) x (p3 - p1) to corners 1, 3, 2, W untouched, then the buffer is normalised.
#include <cstdio>
#include <cmath>

struct V4 { float X, Y, Z, W; };
struct V3 { float X, Y, Z; };
static V3 Sub(const V4& a, const V4& b) { V3 r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V3 Cross(const V3& a, const V3& b) { V3 r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
static void Add(V4& n, const V3& c) { n.X += c.X; n.Y += c.Y; n.Z += c.Z; }

static void CalculateVertexNormals(const V4* positions, V4* normals, unsigned long count, const unsigned short* remap,
                                   const unsigned short (*tris)[3], unsigned long ntris, const unsigned short (*quads)[4], unsigned long nquads)
{
    for (unsigned long i = 0; i < count; ++i) { normals[i].X = normals[i].Y = normals[i].Z = normals[i].W = 0.0f; }
    for (unsigned long t = 0; t < ntris; ++t) {
        unsigned long i0 = remap[tris[t][0]], i1 = remap[tris[t][1]], i2 = remap[tris[t][2]];
        V3 n = Cross(Sub(positions[i2], positions[i0]), Sub(positions[i1], positions[i0]));
        Add(normals[i0], n); Add(normals[i1], n); Add(normals[i2], n);
    }
    for (unsigned long q = 0; q < nquads; ++q) {
        unsigned long i0 = remap[quads[q][0]], i1 = remap[quads[q][1]], i2 = remap[quads[q][2]], i3 = remap[quads[q][3]];
        V3 n1 = Cross(Sub(positions[i0], positions[i1]), Sub(positions[i2], positions[i1]));
        Add(normals[i0], n1); Add(normals[i1], n1); Add(normals[i2], n1);
        V3 n2 = Cross(Sub(positions[i2], positions[i1]), Sub(positions[i3], positions[i1]));
        Add(normals[i1], n2); Add(normals[i3], n2); Add(normals[i2], n2);
    }
    for (unsigned long i = 0; i < count; ++i) {                     // GFNormalise3DVectors
        float l = std::sqrt(normals[i].X * normals[i].X + normals[i].Y * normals[i].Y + normals[i].Z * normals[i].Z);
        if (l > 0.0f) { normals[i].X /= l; normals[i].Y /= l; normals[i].Z /= l; }
    }
}

int main()
{
    // a flat unit quad in the z = 0 plane in strip order (triangles 0 1 2 and 1 3 2) and one triangle (4, 5, 6) in the same plane
    V4 p[7] = { { 0, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 1, 1, 0, 1 }, { 2, 0, 0, 1 }, { 3, 0, 0, 1 }, { 2, 1, 0, 1 } };
    unsigned short remap[7] = { 0, 1, 2, 3, 4, 5, 6 };
    unsigned short tris[1][3] = { { 4, 5, 6 } };
    unsigned short quads[1][4] = { { 0, 1, 2, 3 } };
    V4 n[7];
    CalculateVertexNormals(p, n, 7, remap, tris, 1, quads, 1);
    bool ok = true;
    for (int i = 0; i < 7; ++i)
        if (std::fabs(n[i].X) > 1e-6f || std::fabs(n[i].Y) > 1e-6f || std::fabs(std::fabs(n[i].Z) - 1.0f) > 1e-6f || n[i].W != 0.0f) ok = false;
    // with these windings every face normal points down (-Z): both quad halves agree, so no corner cancels
    for (int i = 0; i < 7; ++i) if (!(n[i].Z < 0.0f)) ok = false;
    if (!ok) { std::printf("00bb1f50_TEST FAIL\n"); return 1; }
    std::printf("00bb1f50_TEST PASS\n");
    return 0;
}
