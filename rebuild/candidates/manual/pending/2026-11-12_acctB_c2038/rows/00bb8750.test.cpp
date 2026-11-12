// Behaviour test for 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup.
// A copy of the row's logic over plain models (render managers record their calls; CArray -> fixed arrays; the floor / ceil helpers
// are the portable twins' semantics for ordinary values), checked against independently computed expectations: the shader / state /
// transform set-up, the per-decal cell extent (floor / ceil, integer edges), the vertex grid (position, normal, uv), the colour constant
// (with and without premultiplied alpha), the two-triangles-per-cell index list with the checkerboard diagonal, PrepareForRender(0x15)
// and the draw counts, and the skip of an empty decal.
#include <cstdio>
#include <cmath>

struct Vec3 { float X, Y, Z; };
struct Colour4 { float R, G, B, A; };
struct Vertex { float X, Y, Z, NormalX, NormalY, NormalZ, U1, V1; };
struct Decal { Vec3 WorldPos; float SizeX, SizeY; unsigned char B, G, R, A; Vec3 Up; };

enum { MAXV = 256, MAXI = 1024, MAXLOG = 64 };
struct Recorder {
    int log[MAXLOG]; int n;
    long vertex_shader; float world[12]; Colour4 constant;
    long prepare_flags; long draw_verts; long draw_tris; long draw_stride;
    Vertex verts[MAXV]; unsigned short idx[MAXI];
    unsigned long alpha_blend, zenable, zwrite, alpha_test, alpha_ref;
    void Add(int c) { if (n < MAXLOG) log[n++] = c; }
};
enum { C_VS = 1, C_PS, C_STATES, C_WORLD, C_CONST, C_PREP, C_DRAW };

static long Floor(float v) { return (long)std::floor(v); }   // GFFloatToLongFloor for ordinary values
static long Ceil(float v) { return (long)std::ceil(v); }     // GFFloatToLongCeil for ordinary values

// The row's logic.
static void RenderLandscapeDecalGroup(Recorder& r, const unsigned long* point_light_index, const Vec3& camera,
                                      const Decal* decals, int count, bool premultiplied_alpha, long point_light_count)
{
    r.vertex_shader = (long)point_light_index[point_light_count]; r.Add(C_VS);
    r.Add(C_PS);
    r.alpha_blend = 1; r.zenable = 1; r.zwrite = 0; r.alpha_test = 1; r.alpha_ref = 1; r.Add(C_STATES);
    float world[12] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
    world[9] = camera.X; world[10] = camera.Y; world[11] = camera.Z;
    for (int k = 0; k < 12; ++k) r.world[k] = world[k];
    r.Add(C_WORLD);
    for (int d = 0; d < count; ++d) {
        const Decal& decal = decals[d];
        long min_x = Floor(decal.WorldPos.X - decal.SizeX * 0.5f);
        long min_y = Floor(decal.WorldPos.Y - decal.SizeY * 0.5f);
        long max_x = Ceil(decal.WorldPos.X + decal.SizeX * 0.5f);
        long max_y = Ceil(decal.WorldPos.Y + decal.SizeY * 0.5f);
        long width = max_x - min_x;
        long height = max_y - min_y;
        long num_tris = width * height * 2;
        if (num_tris > 0) {
            Colour4 colour = { decal.R * (1.0f / 255.0f), decal.G * (1.0f / 255.0f), decal.B * (1.0f / 255.0f), decal.A * (1.0f / 255.0f) };
            if (premultiplied_alpha) { colour.R *= colour.A; colour.G *= colour.A; colour.B *= colour.A; }
            r.constant = colour; r.Add(C_CONST);
            float u_scale = 1.0f / decal.SizeX;
            long row = width + 1;
            long num_verts = row * (height + 1);
            float v_scale = 1.0f / decal.SizeY;
            float u_offset = 0.5f - decal.WorldPos.X * u_scale;
            float v_offset = 0.5f - decal.WorldPos.Y * v_scale;
            Vertex* vertices = r.verts;
            long v = 0;
            for (long y = min_y; y <= max_y; ++y) {
                for (long x = min_x; x <= max_x; ++x) {
                    vertices[v].X = (float)x; vertices[v].Y = (float)y; vertices[v].Z = decal.WorldPos.Z;
                    vertices[v].NormalX = decal.Up.X; vertices[v].NormalY = decal.Up.Y; vertices[v].NormalZ = decal.Up.Z;
                    vertices[v].U1 = vertices[v].X * u_scale + u_offset;
                    vertices[v].V1 = vertices[v].Y * v_scale + v_offset;
                    ++v;
                }
            }
            unsigned short* indices = r.idx;
            long i = 0;
            long vi = 0;
            for (long t = 0; t < num_tris; t += 2) {
                long flip = 1 - ((long)(vertices[vi].X + vertices[vi].Y) & 1);
                indices[i++] = (unsigned short)vi;
                indices[i++] = (unsigned short)(row + flip + vi);
                indices[i++] = (unsigned short)(vi + 1);
                indices[i++] = (unsigned short)(vi - flip + 1);
                indices[i++] = (unsigned short)(row + vi);
                indices[i++] = (unsigned short)(width + vi + 2);
                vi += 1 + ((vi + 2) % row == 0);
            }
            r.prepare_flags = 0x15; r.Add(C_PREP);
            r.draw_verts = num_verts; r.draw_tris = num_tris; r.draw_stride = (long)sizeof(Vertex); r.Add(C_DRAW);
        }
    }
}

static int fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { ++fails; std::printf("check failed: %s\n", what); } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

// Independent expectation for one decal: cells enumerated row by row.
static void CheckDecal(const Recorder& r, const Decal& decal, long min_x, long min_y, long max_x, long max_y)
{
    long w = max_x - min_x, h = max_y - min_y, row = w + 1;
    Check(r.draw_verts == row * (h + 1), "vertex count");
    Check(r.draw_tris == 2 * w * h, "triangle count");
    Check(r.draw_stride == 32, "stride");
    Check(r.prepare_flags == 0x15, "prepare flags");
    for (long y = 0; y <= h; ++y)
        for (long x = 0; x <= w; ++x) {
            const Vertex& vx = r.verts[y * row + x];
            Check(Near(vx.X, (float)(min_x + x)) && Near(vx.Y, (float)(min_y + y)) && Near(vx.Z, decal.WorldPos.Z), "position");
            Check(Near(vx.NormalX, decal.Up.X) && Near(vx.NormalY, decal.Up.Y) && Near(vx.NormalZ, decal.Up.Z), "normal");
            Check(Near(vx.U1, (vx.X - decal.WorldPos.X) / decal.SizeX + 0.5f), "u");
            Check(Near(vx.V1, (vx.Y - decal.WorldPos.Y) / decal.SizeY + 0.5f), "v");
        }
    long k = 0;
    for (long cy = 0; cy < h; ++cy)
        for (long cx = 0; cx < w; ++cx) {
            long b = cy * row + cx;
            bool odd = (((min_x + cx) + (min_y + cy)) & 1) != 0;
            // Even cells: (b, b+row+1, b+1) and (b, b+row, b+row+1); odd cells: (b, b+row, b+1) and (b+1, b+row, b+row+1).
            unsigned short e[6];
            if (!odd) { e[0] = (unsigned short)b; e[1] = (unsigned short)(b + row + 1); e[2] = (unsigned short)(b + 1);
                        e[3] = (unsigned short)b; e[4] = (unsigned short)(b + row); e[5] = (unsigned short)(b + row + 1); }
            else      { e[0] = (unsigned short)b; e[1] = (unsigned short)(b + row); e[2] = (unsigned short)(b + 1);
                        e[3] = (unsigned short)(b + 1); e[4] = (unsigned short)(b + row); e[5] = (unsigned short)(b + row + 1); }
            for (int j = 0; j < 6; ++j) Check(r.idx[k + j] == e[j], "index");
            k += 6;
        }
}

int main()
{
    unsigned long point_light_index[6] = { 0, 1, 2, 2, 2, 2 };
    Vec3 camera = { 100.0f, 200.0f, 30.0f };
    Recorder r; r.n = 0;
    Decal a = { { 10.25f, 20.75f, 3.0f }, 2.0f, 1.0f, 0x40, 0x80, 0xFF, 0x80, { 0.0f, 0.0f, 1.0f } };
    RenderLandscapeDecalGroup(r, point_light_index, camera, &a, 1, true, 3);
    Check(r.n == 7 && r.log[0] == C_VS && r.log[1] == C_PS && r.log[2] == C_STATES && r.log[3] == C_WORLD && r.log[4] == C_CONST &&
          r.log[5] == C_PREP && r.log[6] == C_DRAW, "call order");
    Check(r.vertex_shader == 2, "vertex shader from the point-light table");
    Check(r.alpha_blend == 1 && r.zenable == 1 && r.zwrite == 0 && r.alpha_test == 1 && r.alpha_ref == 1, "render states");
    Check(Near(r.world[0], 1) && Near(r.world[4], 1) && Near(r.world[8], 1) && Near(r.world[1], 0) && Near(r.world[9], 100.0f) &&
          Near(r.world[10], 200.0f) && Near(r.world[11], 30.0f), "world transform");
    float alpha = 0x80 / 255.0f;
    Check(Near(r.constant.R, alpha) && Near(r.constant.G, (0x80 / 255.0f) * alpha) && Near(r.constant.B, (0x40 / 255.0f) * alpha) &&
          Near(r.constant.A, alpha), "premultiplied colour");
    CheckDecal(r, a, 9, 20, 12, 22);

    // Integer edges: floor / ceil keep the exact cell boundaries; no premultiply; colour straight from the bytes.
    Recorder r2; r2.n = 0;
    Decal b = { { 5.0f, -3.0f, 7.5f }, 2.0f, 4.0f, 10, 20, 30, 255, { 0.6f, 0.0f, 0.8f } };
    RenderLandscapeDecalGroup(r2, point_light_index, camera, &b, 1, false, 0);
    Check(r2.vertex_shader == 0, "vertex shader 0");
    Check(Near(r2.constant.R, 30 / 255.0f) && Near(r2.constant.G, 20 / 255.0f) && Near(r2.constant.B, 10 / 255.0f) &&
          Near(r2.constant.A, 1.0f), "straight colour");
    CheckDecal(r2, b, 4, -5, 6, -1);

    // An empty decal (zero size) draws nothing; the next decal in the group still does.
    Recorder r3; r3.n = 0;
    Decal two[2] = { { { 3.0f, 3.0f, 0.0f }, 0.0f, 0.0f, 0, 0, 0, 0, { 0.0f, 0.0f, 1.0f } },
                     { { 0.5f, 0.5f, 1.0f }, 1.0f, 1.0f, 1, 2, 3, 4, { 0.0f, 1.0f, 0.0f } } };
    RenderLandscapeDecalGroup(r3, point_light_index, camera, two, 2, false, 1);
    Check(r3.n == 7 && r3.log[4] == C_CONST && r3.log[6] == C_DRAW, "empty decal skipped");
    CheckDecal(r3, two[1], 0, 0, 1, 1);

    if (fails) { std::printf("00bb8750_TEST FAIL\n"); return 1; }
    std::printf("00bb8750_TEST PASS\n");
    return 0;
}
