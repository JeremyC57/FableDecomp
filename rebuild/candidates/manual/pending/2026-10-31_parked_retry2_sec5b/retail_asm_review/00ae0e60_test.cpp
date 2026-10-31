// Behaviour test for CParticleProgram::TransformVertices (0x00AE0E60): clear the output, then for every blend group gather
// its source vertices, transform them by the group's bone matrix (w assumed 1) and accumulate weight * transformed into the
// output vertex of each blend.
#include <cstdio>

struct V4 { float X, Y, Z, W; };
struct Blend { unsigned long VertexIndex; float BlendValue; };
struct Group { long BoneIndex; const Blend* blends; unsigned long count; };
struct M4 { float M[16]; };

static void Transform(const M4& m, const V4& in, V4& out)   // transposed, w assumed 1
{
    out.X = m.M[0] * in.X + m.M[4] * in.Y + m.M[8] * in.Z + m.M[12];
    out.Y = m.M[1] * in.X + m.M[5] * in.Y + m.M[9] * in.Z + m.M[13];
    out.Z = m.M[2] * in.X + m.M[6] * in.Y + m.M[10] * in.Z + m.M[14];
    out.W = m.M[3] * in.X + m.M[7] * in.Y + m.M[11] * in.Z + m.M[15];
}
static void TransformVertices(V4* out, const V4* in, unsigned long count, const M4* matrices, const Group* groups, unsigned long n)
{
    for (unsigned long i = 0; i < count; ++i) { out[i].X = out[i].Y = out[i].Z = out[i].W = 0.0f; }
    for (unsigned long g = 0; g < n; ++g) {
        if (!groups[g].count) continue;
        const M4& m = matrices[groups[g].BoneIndex];
        for (unsigned long k = 0; k < groups[g].count; ++k) {
            V4 t; Transform(m, in[groups[g].blends[k].VertexIndex], t);
            float w = groups[g].blends[k].BlendValue;
            V4& o = out[groups[g].blends[k].VertexIndex];
            o.X += t.X * w; o.Y += t.Y * w; o.Z += t.Z * w; o.W += t.W * w;
        }
    }
}

int main()
{
    M4 mats[2];
    for (int i = 0; i < 16; ++i) { mats[0].M[i] = (i % 5 == 0) ? 1.0f : 0.0f; mats[1].M[i] = mats[0].M[i]; }
    mats[1].M[12] = 10.0f;                                    // translate x by 10
    V4 in[3] = { { 1, 2, 3, 1 }, { 4, 5, 6, 1 }, { 7, 8, 9, 1 } };
    Blend b0[2] = { { 0, 0.5f }, { 2, 1.0f } };
    Blend b1[1] = { { 0, 0.5f } };
    Group groups[3] = { { 0, b0, 2 }, { 1, b1, 1 }, { 1, 0, 0 } };
    V4 out[3];
    TransformVertices(out, in, 3, mats, groups, 3);
    int ok = 1;
    ok &= out[0].X == 0.5f * 1 + 0.5f * 11 && out[0].Y == 2.0f && out[0].W == 1.0f;
    ok &= out[1].X == 0.0f && out[1].W == 0.0f;              // untouched vertex stays cleared
    ok &= out[2].X == 7.0f && out[2].Z == 9.0f;
    if (!ok) { std::printf("00ae0e60_TEST FAIL\n"); return 1; }
    std::printf("00ae0e60_TEST PASS\n");
    return 0;
}
