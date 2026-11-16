// Behaviour test for CEngineClothRenderer::RenderPolygons (0x00BB2540): a model of the row's buffer filling with the same control flow --
// primitive counts from the render_tris / render_quads flags (nothing drawn when both are empty), one or two sides (double sided), the
// render data created once with two vertex buffers and the current buffer flipping 0 -> 1 -> 0, each vertex = position + normal (negated on
// the back copy) + UV through the render vertex's two indices, and the index list: every triangle, two triangles per quad (A B C, B D C),
// offset by the vertex count on the back copy whose winding bytes are swapped (SWAP of a 2-byte winding table), 6 bytes per primitive.
#include <cstdio>

struct V4 { float X, Y, Z, W; };
struct UV { float X, Y; };
struct RV { unsigned short PositionIndex, TexCoordIndex; };
struct Tri { unsigned short A, B, C; };
struct Quad { unsigned short A, B, C, D; };
struct Vert { float X, Y, Z, NX, NY, NZ, U, V; };
struct RenderData { long CurrentVB; int Created; };

template<class T> static void SWAP(T& a, T& b) { T t = a; a = b; b = t; }

static int Render(RenderData& data, bool& has_data, const Tri* tris, unsigned long ntri, const Quad* quads, unsigned long nquad,
                  const RV* rv, unsigned long nv, const V4* pos, const V4* nrm, const UV* uv, bool double_sided, bool render_tris,
                  bool render_quads, Vert* vout, unsigned short* iout, long& used_vb, unsigned long& index_bytes_out)
{
    unsigned long tri_count = render_tris ? ntri : 0;
    unsigned long quad_count = render_quads ? nquad : 0;
    unsigned long prim_count = tri_count + quad_count * 2;
    if (prim_count == 0)
        return 0;
    unsigned long sides = (double_sided ? 1 : 0) + 1;
    unsigned long total_prims = sides * prim_count;
    if (!has_data) { data.CurrentVB = 0; data.Created++; has_data = true; }
    used_vb = data.CurrentVB;
    data.CurrentVB = (data.CurrentVB + 1) % 2;
    unsigned long out = 0;
    for (unsigned long side = 0; side < sides; ++side)
        for (unsigned long i = 0; i < nv; ++i, ++out) {
            unsigned long p = rv[i].PositionIndex, t = rv[i].TexCoordIndex;
            Vert& v = vout[out];
            v.X = pos[p].X; v.Y = pos[p].Y; v.Z = pos[p].Z;
            float nx = nrm[p].X, ny = nrm[p].Y, nz = nrm[p].Z;
            if (side == 1) { nx = -nx; ny = -ny; nz = -nz; }
            v.NX = nx; v.NY = ny; v.NZ = nz; v.U = uv[t].X; v.V = uv[t].Y;
        }
    index_bytes_out = total_prims * 3 * sizeof(unsigned short);
    unsigned short base = 0;
    unsigned char winding[2] = { 0, 1 };
    unsigned short* idx = iout;
    for (unsigned long side = 0; side < sides; ++side) {
        for (unsigned long k = 0; k < tri_count; ++k) {
            idx[winding[0]] = tris[k].A + base; idx[winding[1]] = tris[k].B + base; idx[2] = tris[k].C + base; idx += 3;
        }
        for (unsigned long q = 0; q < quad_count; ++q) {
            idx[winding[0]] = quads[q].A + base; idx[winding[1]] = quads[q].B + base; idx[2] = quads[q].C + base; idx += 3;
            idx[winding[0]] = quads[q].B + base; idx[winding[1]] = quads[q].D + base; idx[2] = quads[q].C + base; idx += 3;
        }
        SWAP(winding[0], winding[1]);
        base = (unsigned short)(base + nv);
    }
    return (int)(idx - iout);
}

int main()
{
    Tri tris[1] = { { 0, 1, 2 } };
    Quad quads[1] = { { 1, 2, 3, 4 } };
    RV rv[5] = { { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 0 } };
    V4 pos[5], nrm[5]; UV uv[4];
    for (int i = 0; i < 5; ++i) { pos[i].X = (float)i; pos[i].Y = 10.0f + i; pos[i].Z = 20.0f + i; pos[i].W = 1.0f;
                                  nrm[i].X = 0.5f; nrm[i].Y = -0.25f; nrm[i].Z = 1.0f + i; nrm[i].W = 0.0f; }
    for (int i = 0; i < 4; ++i) { uv[i].X = 0.1f * i; uv[i].Y = 0.2f * i; }
    RenderData data = { 0, 0 }; bool has = false; Vert v[10]; unsigned short ix[18]; long vb; unsigned long bytes;
    bool ok = Render(data, has, tris, 1, quads, 1, rv, 5, pos, nrm, uv, true, false, false, v, ix, vb, bytes) == 0 && !has;
    int n = Render(data, has, tris, 1, quads, 1, rv, 5, pos, nrm, uv, true, true, true, v, ix, vb, bytes);
    ok = ok && n == 18 && bytes == 36 && vb == 0 && data.CurrentVB == 1 && data.Created == 1;
    ok = ok && ix[0] == 0 && ix[1] == 1 && ix[2] == 2 && ix[3] == 1 && ix[4] == 2 && ix[5] == 3 && ix[6] == 2 && ix[7] == 4 && ix[8] == 3;
    ok = ok && ix[9] == 6 && ix[10] == 5 && ix[11] == 7 && ix[12] == 7 && ix[13] == 6 && ix[14] == 8 && ix[15] == 9 && ix[16] == 7 && ix[17] == 8;
    ok = ok && v[4].X == 4.0f && v[4].NZ == 5.0f && v[4].U == 0.0f && v[9].NZ == -5.0f && v[9].NX == -0.5f && v[6].V == uv[1].Y;
    n = Render(data, has, tris, 1, quads, 1, rv, 5, pos, nrm, uv, false, true, false, v, ix, vb, bytes);
    ok = ok && n == 3 && bytes == 6 && vb == 1 && data.CurrentVB == 0 && data.Created == 1 && ix[0] == 0 && ix[1] == 1;
    std::printf(ok ? "00bb2540_TEST PASS\n" : "00bb2540_TEST FAIL\n");
    return ok ? 0 : 1;
}