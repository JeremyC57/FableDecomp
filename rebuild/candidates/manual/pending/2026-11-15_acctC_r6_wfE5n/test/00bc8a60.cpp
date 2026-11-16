// Behaviour test for 0x00BC8A60 CEnginePrimitiveManagerLines::RenderLine.
// A copy of the row's logic over plain models (camera transforms = a translation by -cam / +cam, the render manager records the
// blend factors, the shader constant, the pixel shader picked, the four vertices, the texture count and the index list), checked
// against independently written expectations: both ends behind the near plane draw nothing; the end behind the plane is clipped
// onto it (start and end cases); the projection divides X and Y by Z; a zero-length screen direction draws nothing; the corners
// are start/end -/+ the perpendicular (dir.Y, -dir.X) of the normalised direction scaled to half the width; blend modes 3 / 4 give
// ONE/ONE and ONE/INV_SRC_COLOR with colour scale = alpha, any other mode SRC_ALPHA/INV_SRC_ALPHA with colour scale 255; the
// constant is (s, s, s, alpha / 255) with s = colour scale / 255; BankIndex != 0 picks the diffuse (textured) pixel shader and
// one texture, a failed frame lookup draws nothing; UVs (0,0) (1,0) (0,1) (1,1); indices 0 1 3 0 3 2.
#include <cstdio>
#include <cmath>

struct V3 { float x, y, z; };
struct Line { V3 start, end; unsigned char r, g, b, a; float width; unsigned long bank_index; long blend_mode; };
struct Vertex { V3 pos; unsigned long colour; float u, v; };
struct Out {
    bool drawn; long src, dst; float constant[4]; int shader;   // shader: 1 diffuse (textured), 2 diffuse only
    Vertex vtx[4]; unsigned long texture_count; unsigned short indices[6];
};
enum { ONE = 1, INV_SRC_COLOR = 2, SRC_ALPHA = 3, INV_SRC_ALPHA = 4 };

static V3 Sub(V3 a, V3 b) { V3 r = { a.x - b.x, a.y - b.y, a.z - b.z }; return r; }
static V3 Add(V3 a, V3 b) { V3 r = { a.x + b.x, a.y + b.y, a.z + b.z }; return r; }
static V3 Mul(V3 a, float s) { V3 r = { a.x * s, a.y * s, a.z * s }; return r; }

// The row, on the models: camera at `cam` looking down +Z (world -> view = p - cam), near plane `nearp`.
static void RenderLine(const Line& line, V3 cam, float nearp, bool frame_ok, Out& o)
{
    o.drawn = false; o.shader = 0; o.texture_count = 0;
    V3 start = Sub(line.start, cam), end = Sub(line.end, cam);
    if (start.z < nearp) {
        if (end.z < nearp) return;
        start = Add(start, Mul(Sub(end, start), (nearp - start.z) / (end.z - start.z)));
    } else if (end.z < nearp) {
        end = Add(end, Mul(Sub(start, end), (nearp - end.z) / (start.z - end.z)));
    }
    float is = 1.0f / start.z, ie = 1.0f / end.z;
    float dx = end.x * ie - start.x * is, dy = end.y * ie - start.y * is;
    if (!(dx * dx + dy * dy > 1e-6f * 1e-6f)) return;
    float len = std::sqrt(dx * dx + dy * dy); dx /= len; dy /= len;
    dx *= line.width * 0.5f; dy *= line.width * 0.5f;
    V3 offset = { dy, -dx, 0.0f };
    V3 c[4] = { Sub(start, offset), Add(start, offset), Sub(end, offset), Add(end, offset) };
    unsigned long alpha = line.a, colour_scale;
    if (line.blend_mode == 3) { o.src = ONE; o.dst = ONE; colour_scale = alpha; }
    else if (line.blend_mode == 4) { o.src = ONE; o.dst = INV_SRC_COLOR; colour_scale = alpha; }
    else { o.src = SRC_ALPHA; o.dst = INV_SRC_ALPHA; colour_scale = 255; }
    float s = colour_scale / 255.0f;
    o.constant[0] = o.constant[1] = o.constant[2] = s; o.constant[3] = line.a / 255.0f;
    o.shader = line.bank_index != 0 ? 1 : 2;
    static const float uv[4][2] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    for (int i = 0; i < 4; ++i) {
        o.vtx[i].pos = Add(c[i], cam);
        o.vtx[i].colour = (unsigned long)line.a << 24 | (unsigned long)line.r << 16 | (unsigned long)line.g << 8 | line.b;
        o.vtx[i].u = uv[i][0]; o.vtx[i].v = uv[i][1];
    }
    if (line.bank_index != 0) {
        if (!frame_ok) return;
        o.texture_count = 1;
    }
    static const unsigned short idx[6] = { 0, 1, 3, 0, 3, 2 };
    for (int i = 0; i < 6; ++i) o.indices[i] = idx[i];
    o.drawn = true;
}

static int fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { ++fails; std::printf("fail: %s\n", what); } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    V3 cam = { 0, 0, 0 };
    Out o;
    Line l = { { -1, 0, 2 }, { 1, 0, 2 }, 10, 20, 30, 128, 2.0f, 0, 0 };
    RenderLine(l, cam, 1.0f, true, o);
    Check(o.drawn, "simple line drawn");
    // direction (1,0) -> perpendicular (0,-1) * 1: corners start -/+ (0,-1,0)
    Check(Near(o.vtx[0].pos.x, -1) && Near(o.vtx[0].pos.y, 1) && Near(o.vtx[1].pos.y, -1), "start corners");
    Check(Near(o.vtx[2].pos.x, 1) && Near(o.vtx[2].pos.y, 1) && Near(o.vtx[3].pos.y, -1) && Near(o.vtx[3].pos.z, 2), "end corners");
    Check(o.src == SRC_ALPHA && o.dst == INV_SRC_ALPHA && Near(o.constant[0], 1.0f) && Near(o.constant[3], 128 / 255.0f), "default blend");
    Check(o.shader == 2 && o.texture_count == 0, "untextured");
    Check(o.vtx[1].u == 1 && o.vtx[1].v == 0 && o.vtx[2].u == 0 && o.vtx[2].v == 1 && o.vtx[3].u == 1 && o.vtx[3].v == 1, "uvs");
    Check(o.indices[2] == 3 && o.indices[5] == 2 && o.vtx[0].colour == 0x800a141eUL, "indices / colour");
    l.blend_mode = 3; RenderLine(l, cam, 1.0f, true, o);
    Check(o.src == ONE && o.dst == ONE && Near(o.constant[0], 128 / 255.0f), "additive");
    l.blend_mode = 4; RenderLine(l, cam, 1.0f, true, o);
    Check(o.src == ONE && o.dst == INV_SRC_COLOR && Near(o.constant[1], 128 / 255.0f), "add smooth");
    l.bank_index = 7; RenderLine(l, cam, 1.0f, true, o);
    Check(o.drawn && o.shader == 1 && o.texture_count == 1, "textured");
    RenderLine(l, cam, 1.0f, false, o);
    Check(!o.drawn, "missing frame draws nothing");
    Line b = { { 0, 0, -2 }, { 1, 0, -1 }, 0, 0, 0, 255, 1.0f, 0, 0 };
    RenderLine(b, cam, 1.0f, true, o);
    Check(!o.drawn, "both ends behind the near plane");
    Line c = { { 0, 1, 0 }, { 4, 0, 4 }, 0, 0, 0, 255, 1.0f, 0, 0 };   // start clipped to (1, 0.75, 1)
    RenderLine(c, cam, 1.0f, true, o);
    Check(o.drawn && Near(o.vtx[0].pos.z, 1) && Near(o.vtx[0].pos.y, 0.75f) && Near(o.vtx[0].pos.x, 1.5f), "start clipped");
    Line d = { { 4, 0, 4 }, { 0, 1, 0 }, 0, 0, 0, 255, 1.0f, 0, 0 };   // end clipped to (1, 0.75, 1)
    RenderLine(d, cam, 1.0f, true, o);
    Check(o.drawn && Near(o.vtx[2].pos.z, 1) && Near(o.vtx[2].pos.y, 0.75f) && Near(o.vtx[2].pos.x, 0.5f), "end clipped");
    Line e = { { 1, 1, 1 }, { 2, 2, 2 }, 0, 0, 0, 255, 1.0f, 0, 0 };   // same screen point: zero direction
    RenderLine(e, cam, 0.5f, true, o);
    Check(!o.drawn, "zero screen direction draws nothing");
    Line f = { { 0, 0, 2 }, { 0, 4, 4 }, 0, 0, 0, 255, 4.0f, 0, 0 };   // projected (0,0) -> (0,1): perpendicular (1,0) * 2
    RenderLine(f, cam, 1.0f, true, o);
    Check(Near(o.vtx[0].pos.x, -2) && Near(o.vtx[1].pos.x, 2) && Near(o.vtx[3].pos.x, 2), "projected direction");
    if (fails) { std::printf("00bc8a60_TEST FAIL %d\n", fails); return 1; }
    std::printf("00bc8a60_TEST PASS\n");
    return 0;
}
