// Behaviour test for 0x00BAE2D0 CEnginePrimitiveManager2DSprites::RenderSubPrimitive.
// A copy of the row's logic over plain models (the render / shader managers record what the row hands them), checked against
// independently written expectations: the fixed 2D states (Z off, Z write off, alpha blend + alpha test on, alpha ref 1), the alpha-mask
// stage (point filtering only when the mask is not filtered, clamp), the per-sub-primitive stage 0 (wrap / clamp, point filtering when
// not filtered), the blend-mode table (additive / add-smooth: ONE + ONE / INV_SRC_COLOR and the colour scaled by the alpha; any other
// mode: SRC_ALPHA / INV_SRC_ALPHA and full colour), the diffuse pixel shader pick (textured / untextured x alpha / additive), the
// clock sprite's own pixel shader and its two draws (split at IndexOffset / 3 triangles, second texture and second colour), and the
// sprite's single 2-triangle draw with the manager's index list.
#include <cstdio>
#include <cmath>

enum { BLEND_ALPHA = 2, BLEND_ADDITIVE = 3, BLEND_ADDSMOOTH = 4, BLEND_SOLID = 0 };
enum { AB_ONE = 2, AB_SRC_ALPHA = 5, AB_INV_SRC_ALPHA = 6, AB_INV_SRC_COLOR = 4 };
enum { UV_WRAP = 1, UV_CLAMP = 3 };
enum { PS_2D_DIFFUSE = 1, PS_2D_DIFFUSE_ADD, PS_2D_DIFFUSE_ONLY, PS_2D_DIFFUSE_ONLY_ADD, PS_CLOCK, PS_CLOCK_ADD };

struct Draw { const void* tex0; const void* tex1; int vertex_count; const unsigned short* indices; long tris; float colour[4]; };
struct Out {
    bool z, zwrite, blend, atest; unsigned long aref;
    bool point1, point0; int uv1, uv0; int src, dst; float c2[4];
    int ps_diffuse, ps_clock; float factor[4]; int draws; Draw d[2];
};
struct Sub {
    bool clock; unsigned char alpha; int blend; bool filter, wrap;
    const void* texture[2]; const void* mask; unsigned char index_offset; unsigned char colour[4], colour2[4];   // R G B A
    unsigned short indices[18];
};

static void SetupBlend(int mode, unsigned long alpha, unsigned long& scale, Out& o)
{
    switch (mode) {
    case BLEND_ADDITIVE: o.src = AB_ONE; o.dst = AB_ONE; scale = alpha; break;
    case BLEND_ADDSMOOTH: o.src = AB_ONE; o.dst = AB_INV_SRC_COLOR; scale = alpha; break;
    default: o.src = AB_SRC_ALPHA; o.dst = AB_INV_SRC_ALPHA; scale = 255; break;
    }
}
static void Diffuse(bool textured, unsigned long scale, unsigned long alpha, int mode, Out& o)
{
    float s = scale / 255.0f;
    o.c2[0] = o.c2[1] = o.c2[2] = s; o.c2[3] = alpha / 255.0f;
    o.ps_diffuse = textured ? (mode == BLEND_ALPHA ? PS_2D_DIFFUSE : PS_2D_DIFFUSE_ADD)
                            : (mode == BLEND_ALPHA ? PS_2D_DIFFUSE_ONLY : PS_2D_DIFFUSE_ONLY_ADD);
}
static void Render(const Sub& s, bool mask_filter, const unsigned short* manager_indices, Out& o)
{
    o.z = false; o.zwrite = false; o.blend = true; o.atest = true; o.aref = 1;
    o.point1 = !mask_filter; o.uv1 = UV_CLAMP;
    o.uv0 = s.wrap ? UV_WRAP : UV_CLAMP;
    o.point0 = !s.filter;
    unsigned long alpha = s.alpha, scale;
    SetupBlend(s.blend, alpha, scale, o);
    o.ps_clock = 0; o.draws = 0;
    for (int i = 0; i < 4; ++i) o.factor[i] = 1.0f;
    if (s.clock) {
        Diffuse(true, scale, alpha, s.blend, o);
        o.ps_clock = s.blend == BLEND_ALPHA ? PS_CLOCK : PS_CLOCK_ADD;
        long first = s.index_offset / 3;
        Draw& a = o.d[o.draws++];
        a.tex0 = s.texture[0]; a.tex1 = s.mask; a.vertex_count = 7; a.indices = s.indices; a.tris = first;
        for (int i = 0; i < 4; ++i) a.colour[i] = s.colour[i] / 255.0f;
        Draw& b = o.d[o.draws++];
        b.tex0 = s.texture[1]; b.tex1 = s.mask; b.vertex_count = 7; b.indices = &s.indices[s.index_offset]; b.tris = 6 - first;
        for (int i = 0; i < 4; ++i) b.colour[i] = s.colour2[i] / 255.0f;
    } else {
        Diffuse(s.texture[0] != 0, scale, alpha, s.blend, o);
        Draw& a = o.d[o.draws++];
        a.tex0 = s.texture[0]; a.tex1 = s.mask; a.vertex_count = 4; a.indices = manager_indices; a.tris = 2;
    }
}

static int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static bool feq(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    int t0, t1, m;
    unsigned short mgr[6] = { 0, 1, 2, 0, 2, 3 };
    Sub s = {};
    Out o;
    // 2D sprite, alpha blend, textured, filtered, clamped
    s.clock = false; s.alpha = 128; s.blend = BLEND_ALPHA; s.filter = true; s.wrap = false; s.texture[0] = &t0; s.mask = &m;
    Render(s, true, mgr, o);
    CHECK(!o.z && !o.zwrite && o.blend && o.atest && o.aref == 1);
    CHECK(!o.point1 && o.uv1 == UV_CLAMP && o.uv0 == UV_CLAMP && !o.point0);
    CHECK(o.src == AB_SRC_ALPHA && o.dst == AB_INV_SRC_ALPHA);
    CHECK(feq(o.c2[0], 1.0f) && feq(o.c2[3], 128 / 255.0f));
    CHECK(o.ps_diffuse == PS_2D_DIFFUSE && o.ps_clock == 0);
    CHECK(o.draws == 1 && o.d[0].tex0 == &t0 && o.d[0].tex1 == &m && o.d[0].vertex_count == 4 && o.d[0].indices == mgr && o.d[0].tris == 2);
    CHECK(feq(o.factor[0], 1.0f) && feq(o.factor[3], 1.0f));
    // untextured additive sprite, wrapped, unfiltered, unfiltered mask
    s.texture[0] = 0; s.blend = BLEND_ADDITIVE; s.wrap = true; s.filter = false;
    Render(s, false, mgr, o);
    CHECK(o.point1 && o.point0 && o.uv0 == UV_WRAP);
    CHECK(o.src == AB_ONE && o.dst == AB_ONE && feq(o.c2[0], 128 / 255.0f) && feq(o.c2[2], 128 / 255.0f));
    CHECK(o.ps_diffuse == PS_2D_DIFFUSE_ONLY_ADD);
    s.blend = BLEND_ADDSMOOTH; Render(s, false, mgr, o);
    CHECK(o.src == AB_ONE && o.dst == AB_INV_SRC_COLOR && o.ps_diffuse == PS_2D_DIFFUSE_ONLY_ADD);
    s.blend = BLEND_SOLID; Render(s, false, mgr, o);
    CHECK(o.src == AB_SRC_ALPHA && o.ps_diffuse == PS_2D_DIFFUSE_ONLY_ADD && feq(o.c2[1], 1.0f));
    s.blend = BLEND_ALPHA; Render(s, false, mgr, o);
    CHECK(o.ps_diffuse == PS_2D_DIFFUSE_ONLY);
    // clock sprite: always textured (the Texture array), split draw
    Sub c = {};
    c.clock = true; c.alpha = 51; c.blend = BLEND_ADDITIVE; c.filter = true; c.wrap = true; c.texture[0] = &t0; c.texture[1] = &t1; c.mask = &m;
    c.index_offset = 9; c.colour[0] = 255; c.colour[1] = 0; c.colour[2] = 51; c.colour[3] = 255; c.colour2[0] = 0; c.colour2[3] = 102;
    Render(c, true, mgr, o);
    CHECK(o.ps_diffuse == PS_2D_DIFFUSE_ADD && o.ps_clock == PS_CLOCK_ADD && feq(o.c2[0], 0.2f) && feq(o.c2[3], 0.2f));
    CHECK(o.draws == 2 && o.d[0].tex0 == &t0 && o.d[0].tris == 3 && o.d[0].indices == c.indices && o.d[0].vertex_count == 7);
    CHECK(o.d[1].tex0 == &t1 && o.d[1].tex1 == &m && o.d[1].tris == 3 && o.d[1].indices == &c.indices[9]);
    CHECK(feq(o.d[0].colour[0], 1.0f) && feq(o.d[0].colour[2], 0.2f) && feq(o.d[1].colour[3], 0.4f));
    c.index_offset = 12; c.blend = BLEND_ALPHA; c.texture[0] = 0;
    Render(c, true, mgr, o);
    CHECK(o.ps_diffuse == PS_2D_DIFFUSE && o.ps_clock == PS_CLOCK && o.d[0].tris == 4 && o.d[1].tris == 2 && o.d[1].indices == &c.indices[12]);
    CHECK(feq(o.c2[0], 1.0f) && o.src == AB_SRC_ALPHA);
    if (fails == 0) std::printf("00bae2d0_TEST PASS\n");
    return fails;
}
