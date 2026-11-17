// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal (0x00AC7CB0): the face's vertices a, b, c (by the
// face's three vertex indices into the primitive's vertex array); normal = (c - a) ^ (b - a), normalised (fast inverse square root,
// approximate) only when its squared length exceeds DELTA_NOTIONAL_ZERO squared; a degenerate face returns the zero vector unchanged.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static V sub(const V& a, const V& b) { V r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V cross(const V& a, const V& b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
static const float DELTA_NOTIONAL_ZERO = 0.0001f;

struct Face { long VertexIndices[3]; };
struct Prim {
    V Vertices[6];
    V GetFaceNormal(const Face& face) const
    {
        const V& vertex_a = Vertices[face.VertexIndices[0]];
        const V& vertex_b = Vertices[face.VertexIndices[1]];
        const V& vertex_c = Vertices[face.VertexIndices[2]];
        V normal = cross(sub(vertex_c, vertex_a), sub(vertex_b, vertex_a));
        float squared = normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z;
        if (squared > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            float factor = 1.0f / std::sqrt(squared);
            normal.X *= factor; normal.Y *= factor; normal.Z *= factor;
        }
        return normal;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static int fail(int n) { std::printf("00ac7cb0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Prim p = { { { 0, 0, 0 }, { 2, 0, 0 }, { 0, 3, 0 }, { 0.00001f, 0, 0 }, { 0.0001f, 0, 0 }, { 0, 1, 0 } } };
    Face f = { { 0, 1, 2 } };
    V n = p.GetFaceNormal(f);                       // (0,3,0) ^ (2,0,0) = (0,0,-6) -> (0,0,-1)
    if (!near(n.X, 0) || !near(n.Y, 0) || !near(n.Z, -1)) return fail(1);
    Face g = { { 0, 2, 1 } };                       // reversed winding
    n = p.GetFaceNormal(g);
    if (!near(n.Z, 1)) return fail(2);
    Face h = { { 1, 2, 0 } };                       // a = (2,0,0): (c-a)=(-2,0,0), (b-a)=(-2,3,0) -> (0,0,-6)
    n = p.GetFaceNormal(h);
    if (!near(n.Z, -1)) return fail(3);
    Face d = { { 0, 1, 1 } };                       // degenerate: zero vector, not normalised
    n = p.GetFaceNormal(d);
    if (n.X != 0 || n.Y != 0 || n.Z != 0) return fail(4);
    Face tiny = { { 0, 3, 2 } };                    // |normal| = 3e-5 < 1e-4: returned unnormalised
    n = p.GetFaceNormal(tiny);
    if (!(std::fabs(n.Z) < 1e-4f) || n.Z == 0) return fail(5);
    Face bound = { { 0, 4, 5 } };                   // |normal| = 1e-4 exactly: not > DELTA^2, unnormalised
    n = p.GetFaceNormal(bound);
    if (n.Z != -0.0001f) return fail(6);
    std::printf("00ac7cb0_TEST PASS\n");
    return 0;
}
