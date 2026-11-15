// Behaviour test for 0x00BBAC60 CEnginePrimitiveManagerDecal::RenderLandscapeDecal.
// A copy of the row's logic over plain models (graphic bank = array of infos with an "exists" flag and a frame; the render / shader /
// lighting managers record what the row hands them), checked against independently written expectations: the two early exits (bank
// entry missing, frame not available), the blend op / blend mode mapping and the premultiplied pixel-shader constant, the unit grid laid
// over the decal footprint (floor of the min corner, ceil of the max corner; vertex position, normal = the set's Up, UV from the decal
// centre and size), the alternating-diagonal index list (parity of x + y of the quad's corner vertex, the corner skipping the last vertex
// of each row), the light count (lit: the light array's count and SetupShaderPointLights; unlit: 0 and 100% self illumination) and the
// vertex shader picked through PointLightShaderIndex[count].
#include <cstdio>
#include <cmath>

struct Vec3 { float X, Y, Z; };
struct Vertex { float X, Y, Z, NX, NY, NZ, U, V; };
struct Colour { unsigned char B, G, R, A; };
struct Decal {
    long index, frame; Vec3 pos; float sx, sy; Colour colour; Vec3 up; long blend_mode, blend_op; bool lit;
};
struct Info { bool exists; bool frame_ok; };
struct Result {
    bool drawn; long blend_op, src, dst; float constant[4];
    Vertex vertices[64]; long vertex_count; unsigned short indices[128]; long triangle_count;
    long light_count; bool point_lights; long self_illum; long vertex_shader;
};
enum { BLENDOP_ADD = 1, BLENDOP_SUB = 2, BLENDOP_REVSUB = 3, BLEND_ONE = 2, BLEND_INVSRCCOLOR = 4, BLEND_SRCALPHA = 5, BLEND_INVSRCALPHA = 6 };

static long Floor(float v) { return (long)std::floor(v); }
static long Ceil(float v) { return (long)std::ceil(v); }

// The row's logic.
static void RenderLandscapeDecal(const Decal& decal, const Info* bank, long bank_size, long lights_found,
                                 const long point_light_shader_index[6], Result& r)
{
    r.drawn = false;
    if (decal.index <= 0 || decal.index >= bank_size || !bank[decal.index].exists)
        return;
    if (!bank[decal.index].frame_ok)
        return;
    r.blend_op = decal.blend_op == 1 ? BLENDOP_SUB : decal.blend_op == 2 ? BLENDOP_REVSUB : BLENDOP_ADD;
    bool premultiply;
    switch (decal.blend_mode) {
    case 3: r.src = BLEND_ONE; r.dst = BLEND_ONE; premultiply = true; break;
    case 4: r.src = BLEND_ONE; r.dst = BLEND_INVSRCCOLOR; premultiply = true; break;
    default: r.src = BLEND_SRCALPHA; r.dst = BLEND_INVSRCALPHA; premultiply = false; break;
    }
    float red = decal.colour.R * (1.0f / 255.0f), green = decal.colour.G * (1.0f / 255.0f);
    float blue = decal.colour.B * (1.0f / 255.0f), alpha = decal.colour.A * (1.0f / 255.0f);
    if (premultiply) { red *= alpha; green *= alpha; blue *= alpha; }
    r.constant[0] = red; r.constant[1] = green; r.constant[2] = blue; r.constant[3] = alpha;

    long min_x = Floor(decal.pos.X - decal.sx * 0.5f), min_y = Floor(decal.pos.Y - decal.sy * 0.5f);
    long max_x = Ceil(decal.pos.X + decal.sx * 0.5f), max_y = Ceil(decal.pos.Y + decal.sy * 0.5f);
    long width = max_x - min_x, height = max_y - min_y;
    r.vertex_count = (height + 1) * (width + 1);
    long vertex = 0;
    for (long y = min_y; y <= max_y; ++y)
        for (long x = min_x; x <= max_x; ++x) {
            Vertex& v = r.vertices[vertex++];
            v.X = (float)x; v.Y = (float)y; v.Z = decal.pos.Z;
            v.NX = decal.up.X; v.NY = decal.up.Y; v.NZ = decal.up.Z;
            v.U = (v.X - decal.pos.X) / decal.sx + 0.5f;
            v.V = (v.Y - decal.pos.Y) / decal.sy + 0.5f;
        }
    r.triangle_count = height * width * 2;
    long index = 0, corner = 0;
    for (long triangle = 0; triangle < r.triangle_count; triangle += 2) {
        if ((long)(r.vertices[corner].Y + r.vertices[corner].X) & 1) {
            r.indices[index++] = (unsigned short)corner;
            r.indices[index++] = (unsigned short)(width + corner + 1);
            r.indices[index++] = (unsigned short)(corner + 1);
            r.indices[index++] = (unsigned short)(corner + 1);
            r.indices[index++] = (unsigned short)(width + corner + 1);
            r.indices[index++] = (unsigned short)(width + corner + 2);
        } else {
            r.indices[index++] = (unsigned short)corner;
            r.indices[index++] = (unsigned short)(width + corner + 2);
            r.indices[index++] = (unsigned short)(corner + 1);
            r.indices[index++] = (unsigned short)corner;
            r.indices[index++] = (unsigned short)(width + corner + 1);
            r.indices[index++] = (unsigned short)(width + corner + 2);
        }
        ++corner;
        if ((corner + 1) % (width + 1) == 0)
            ++corner;
    }
    if (decal.lit) { r.light_count = lights_found; r.point_lights = true; r.self_illum = -1; }
    else { r.light_count = 0; r.point_lights = false; r.self_illum = 100; }
    r.vertex_shader = point_light_shader_index[r.light_count];
    r.drawn = true;
}

static int fails;
static void Check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    Info bank[4] = { { false, false }, { true, true }, { true, false }, { false, true } };
    const long shader_index[6] = { 0, 1, 1, 2, 2, 2 };
    Decal d = { 1, 0, { 10.5f, 20.5f, 3.0f }, 2.0f, 1.0f, { 255, 0, 0, 255 }, { 0.0f, 0.0f, 1.0f }, 0, 0, false };
    Result r;

    // Early exits: missing entry, frame not available.
    Decal e = d; e.index = 3; RenderLandscapeDecal(e, bank, 4, 0, shader_index, r); Check(!r.drawn, "missing entry");
    e.index = 2; RenderLandscapeDecal(e, bank, 4, 0, shader_index, r); Check(!r.drawn, "no frame");

    // Footprint x 9.5..11.5 -> 9..12 (width 3), y 20..21 (height 1): 8 vertices, 6 triangles.
    RenderLandscapeDecal(d, bank, 4, 0, shader_index, r);
    Check(r.drawn, "drawn");
    Check(r.vertex_count == 8 && r.triangle_count == 6, "grid size");
    Check(r.vertices[0].X == 9.0f && r.vertices[0].Y == 20.0f && r.vertices[7].X == 12.0f && r.vertices[7].Y == 21.0f, "grid corners");
    Check(r.vertices[5].Z == 3.0f && r.vertices[5].NZ == 1.0f && r.vertices[5].NX == 0.0f, "z and normal");
    Check(Near(r.vertices[0].U, -0.25f) && Near(r.vertices[0].V, 0.0f) && Near(r.vertices[7].U, 1.25f) && Near(r.vertices[7].V, 1.0f), "uv");
    // Quads: corner 0 (9+20 odd), 1 (10+20 even), 2 (11+20 odd); width 3 -> row stride 4.
    static const unsigned short want[18] = { 0, 4, 1, 1, 4, 5,   1, 6, 2, 1, 5, 6,   2, 6, 3, 3, 6, 7 };
    bool same = true;
    for (int i = 0; i < 18; ++i) same = same && r.indices[i] == want[i];
    Check(same, "indices 3x1");
    Check(r.blend_op == BLENDOP_ADD && r.src == BLEND_SRCALPHA && r.dst == BLEND_INVSRCALPHA, "default blend");
    Check(Near(r.constant[0], 0.0f) && Near(r.constant[2], 1.0f) && Near(r.constant[3], 1.0f), "plain constant");
    Check(r.light_count == 0 && !r.point_lights && r.self_illum == 100 && r.vertex_shader == 0, "unlit");

    // 2x2 footprint: corner skipping across rows (corners 0, 1, 3, 4).
    Decal g = d; g.pos.X = 1.0f; g.pos.Y = 1.0f; g.sx = 2.0f; g.sy = 2.0f; g.blend_op = 1; g.blend_mode = 3; g.lit = true;
    g.colour.R = 255; g.colour.G = 51; g.colour.B = 0; g.colour.A = 51;
    RenderLandscapeDecal(g, bank, 4, 3, shader_index, r);
    Check(r.vertex_count == 9 && r.triangle_count == 8, "grid 2x2");
    static const unsigned short want2[24] = { 0, 4, 1, 0, 3, 4,   1, 4, 2, 2, 4, 5,   3, 6, 4, 4, 6, 7,   4, 8, 5, 4, 7, 8 };
    same = true;
    for (int i = 0; i < 24; ++i) same = same && r.indices[i] == want2[i];
    Check(same, "indices 2x2");
    Check(r.blend_op == BLENDOP_SUB && r.src == BLEND_ONE && r.dst == BLEND_ONE, "additive blend");
    Check(Near(r.constant[0], 0.2f) && Near(r.constant[1], 0.04f) && Near(r.constant[2], 0.0f) && Near(r.constant[3], 0.2f), "premultiplied");
    Check(r.light_count == 3 && r.point_lights && r.vertex_shader == 2, "lit");

    g.blend_op = 2; g.blend_mode = 4; RenderLandscapeDecal(g, bank, 4, 1, shader_index, r);
    Check(r.blend_op == BLENDOP_REVSUB && r.dst == BLEND_INVSRCCOLOR && r.vertex_shader == 1, "revsub / addsmooth");

    if (!fails) std::printf("00bbac60_TEST PASS\n");
    return fails;
}