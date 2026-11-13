// Behaviour test for 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive: a host model of the row's decisions with recording
// stand-ins for the engine: the occlusion test (layer set, occlusion info present, occlusion enabled -> handle allocated, test added,
// Start / EndOcclusionTest around RenderPolygons), the preloading bail-out (flush + restore defaults, nothing drawn), the five outline
// passes (each with its own states / vertex / pixel shader; the smooth-fill pass picks the alpha variant only for a transparent material
// with a diffuse map), and the default pass: stencil mask on render flag 0x80, the light-count vertex shader (clamped at 5, spot /
// shadow tables by flags 0x10 / 0x200), the alpha states (pass 1: z-write, no blend, alpha test with the parent's alpha-ref, no colour
// writes; flag 2: alpha test with alpha * ref >> 8, else none and ref 0; flag 1: blending), the tinted (non-white) shadow pixel shader
// and the spot-light set-up, and the mipmap level (floor of width / (multiplier / distance * scale), clamped to 1..255, top bit).
#include <cstdio>
#include <cmath>

enum { RENDER_PASS_DEFAULT = 0, RENDER_PASS_SHADOW = 1, OUTLINE_SMOOTH_STENCIL = 5, OUTLINE_SMOOTH_FILL, OUTLINE_SMOOTH_ALPHA_STENCIL_MASK,
       OUTLINE_STENCIL, OUTLINE_FILL };

struct SState { unsigned long ZWrite, AlphaBlend, AlphaTest, AlphaRef, ColourWrite, Src, Dest; };
struct SLog {
    SState rs; int occ_alloc, occ_add, occ_start, occ_end, flush, restore, polygons, stencil_mask, shadow_func, spot;
    int vs_table, vs_index, ps; int outline_kind, outline_alpha; unsigned long mipmap;
};
struct SInput {
    int layer; bool has_info, occ_enable; bool preloading; int pass; unsigned long flags; unsigned char alpha, alpha_ref;
    long light_count; unsigned long tint; bool transparent; unsigned long diffuse_index; float distance, scale, multiplier;
    unsigned long width;
};
enum { PS_DIFFUSE_ONLY = 1, PS_OUTLINE_FILL, PS_OUTLINE_FILL_ALPHA, PS_DIFFUSE_COLOUR_TEX_ALPHA, PS_FOG_SHADOW, PS_FOG_SHADOW_FACTOR, PS_DIFFUSE };
enum { VS_TRI = 1, VS_SHADOW, VS_SPOT_SHADOW, VS_OUTLINE, VS_OUTLINE_FILL };

static unsigned char TopBit(unsigned char v) { unsigned char b = 0; while (v >>= 1) ++b; return b; }

static unsigned long Mipmap(const SInput& in)
{
    unsigned long mipmap = 0;
    if (in.distance > 0.0001f && (long)in.diffuse_index > 0) {
        float ratio = (float)(long)in.width / (in.multiplier / in.distance * in.scale);
        long size = (long)floor(ratio);
        if (size < 1) size = 1; else if (size > 255) size = 255;
        mipmap = TopBit((unsigned char)size);
    }
    return mipmap;
}

static void SetupAlpha(SLog& g, unsigned long flags, int pass, unsigned char alpha, unsigned char ref)
{
    if (pass == 1) {
        g.rs.ZWrite = 1; g.rs.AlphaBlend = 0; g.rs.AlphaTest = 1; g.rs.AlphaRef = ref; g.rs.ColourWrite = 0;
    } else {
        if (flags & 2) { g.rs.AlphaTest = 1; g.rs.AlphaRef = (alpha * ref) >> 8; }
        else { g.rs.AlphaTest = 0; g.rs.AlphaRef = 0; }
        if (flags & 1) { g.rs.ZWrite = 0; g.rs.AlphaBlend = 1; g.rs.Src = 5; g.rs.Dest = 6; g.rs.AlphaTest = 1; }
        else { g.rs.ZWrite = 1; g.rs.AlphaBlend = 0; }
    }
}

static void Render(const SInput& in, SLog& g)
{
    bool occlusion_test = false;
    if (in.layer != 0 && in.has_info && in.occ_enable) { g.occ_alloc++; g.occ_add++; occlusion_test = true; }
    g.mipmap = Mipmap(in);
    if (in.preloading) { g.flush++; g.restore++; return; }
    switch (in.pass) {
    case OUTLINE_SMOOTH_STENCIL: g.outline_kind = 1; g.vs_table = VS_OUTLINE; g.ps = PS_DIFFUSE_ONLY; break;
    case OUTLINE_SMOOTH_FILL:
        g.vs_table = VS_OUTLINE_FILL; g.outline_kind = 2;
        if (in.diffuse_index != 0 && in.transparent) { g.ps = PS_OUTLINE_FILL_ALPHA; g.outline_alpha = 1; }
        else { g.ps = PS_OUTLINE_FILL; g.outline_alpha = 0; }
        break;
    case OUTLINE_SMOOTH_ALPHA_STENCIL_MASK: g.vs_table = VS_OUTLINE; g.outline_kind = 3; g.ps = PS_DIFFUSE_COLOUR_TEX_ALPHA; break;
    case OUTLINE_STENCIL: g.outline_kind = 4; g.vs_table = VS_OUTLINE; g.ps = PS_DIFFUSE_ONLY; break;
    case OUTLINE_FILL: g.outline_kind = 5; g.vs_table = VS_OUTLINE; g.ps = PS_DIFFUSE_ONLY; break;
    default: {
        if (in.flags & 0x80) g.stencil_mask++;
        g.vs_table = (in.flags & 0x200) ? ((in.flags & 0x10) ? VS_SPOT_SHADOW : VS_SHADOW) : VS_TRI;
        g.vs_index = in.light_count < 5 ? in.light_count : 5;
        SetupAlpha(g, in.flags, in.pass, in.alpha, in.alpha_ref);
        bool tinted = (in.tint & 0xffffff) != 0xffffff;
        if (in.flags & 0x200) {
            g.ps = tinted ? PS_FOG_SHADOW_FACTOR : PS_FOG_SHADOW;
            g.shadow_func++;
            if (in.flags & 0x10) g.spot++;
        } else {
            g.ps = PS_DIFFUSE;
        }
    } }
    if (occlusion_test) g.occ_start++;
    g.polygons++;
    if (occlusion_test) g.occ_end++;
    g.flush++; g.restore++;
}

static int g_fail = 0;
static void Check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++g_fail; } }
#define CHECK(c) Check((c), __LINE__)

static SInput Base()
{
    SInput in = { 1, true, true, false, RENDER_PASS_DEFAULT, 0, 200, 128, 3, 0xffffffff, false, 1, 10.0f, 1.0f, 640.0f, 256 };
    return in;
}

int main()
{
    { SInput in = Base(); SLog g = SLog(); Render(in, g);
      CHECK(g.occ_alloc == 1 && g.occ_start == 1 && g.occ_end == 1 && g.polygons == 1 && g.flush == 1 && g.restore == 1);
      CHECK(g.vs_table == VS_TRI && g.vs_index == 3 && g.ps == PS_DIFFUSE && g.stencil_mask == 0);
      CHECK(g.rs.AlphaTest == 0 && g.rs.AlphaRef == 0 && g.rs.ZWrite == 1 && g.rs.AlphaBlend == 0);
      CHECK(g.mipmap == 2); }                                  // 256 / (640 / 10) = 4 -> top bit 2
    { SInput in = Base(); in.layer = 0; SLog g = SLog(); Render(in, g); CHECK(g.occ_alloc == 0 && g.occ_start == 0 && g.polygons == 1); }
    { SInput in = Base(); in.has_info = false; SLog g = SLog(); Render(in, g); CHECK(g.occ_alloc == 0 && g.occ_end == 0); }
    { SInput in = Base(); in.occ_enable = false; SLog g = SLog(); Render(in, g); CHECK(g.occ_add == 0); }
    { SInput in = Base(); in.preloading = true; SLog g = SLog(); Render(in, g);
      CHECK(g.polygons == 0 && g.flush == 1 && g.restore == 1 && g.occ_start == 0 && g.ps == 0); }
    { SInput in = Base(); in.flags = 0x2 | 0x1 | 0x80; in.light_count = 9; SLog g = SLog(); Render(in, g);
      CHECK(g.stencil_mask == 1 && g.vs_index == 5);
      CHECK(g.rs.AlphaTest == 1 && g.rs.AlphaRef == ((200 * 128) >> 8) && g.rs.ZWrite == 0 && g.rs.AlphaBlend == 1 && g.rs.Src == 5 && g.rs.Dest == 6); }
    { SInput in = Base(); in.pass = RENDER_PASS_SHADOW; in.flags = 1; SLog g = SLog(); Render(in, g);
      CHECK(g.rs.ZWrite == 1 && g.rs.AlphaBlend == 0 && g.rs.AlphaTest == 1 && g.rs.AlphaRef == 128 && g.rs.ColourWrite == 0); }
    { SInput in = Base(); in.flags = 0x200; SLog g = SLog(); Render(in, g);
      CHECK(g.vs_table == VS_SHADOW && g.ps == PS_FOG_SHADOW && g.shadow_func == 1 && g.spot == 0); }
    { SInput in = Base(); in.flags = 0x210; in.tint = 0xff808080; SLog g = SLog(); Render(in, g);
      CHECK(g.vs_table == VS_SPOT_SHADOW && g.ps == PS_FOG_SHADOW_FACTOR && g.shadow_func == 1 && g.spot == 1); }
    { SInput in = Base(); in.tint = 0x00ffffff; in.flags = 0x200; SLog g = SLog(); Render(in, g); CHECK(g.ps == PS_FOG_SHADOW); }
    { SInput in = Base(); in.pass = OUTLINE_SMOOTH_FILL; in.transparent = true; SLog g = SLog(); Render(in, g);
      CHECK(g.vs_table == VS_OUTLINE_FILL && g.ps == PS_OUTLINE_FILL_ALPHA && g.outline_alpha == 1 && g.polygons == 1); }
    { SInput in = Base(); in.pass = OUTLINE_SMOOTH_FILL; in.transparent = true; in.diffuse_index = 0; SLog g = SLog(); Render(in, g);
      CHECK(g.ps == PS_OUTLINE_FILL && g.outline_alpha == 0); }
    { SInput in = Base(); in.pass = OUTLINE_SMOOTH_ALPHA_STENCIL_MASK; SLog g = SLog(); Render(in, g);
      CHECK(g.ps == PS_DIFFUSE_COLOUR_TEX_ALPHA && g.vs_table == VS_OUTLINE && g.outline_kind == 3); }
    { SInput in = Base(); in.pass = OUTLINE_STENCIL; SLog g = SLog(); Render(in, g); CHECK(g.ps == PS_DIFFUSE_ONLY && g.outline_kind == 4 && g.stencil_mask == 0); }
    { SInput in = Base(); in.pass = OUTLINE_FILL; SLog g = SLog(); Render(in, g); CHECK(g.outline_kind == 5); }
    { SInput in = Base(); in.distance = 0.0f; SLog g = SLog(); Render(in, g); CHECK(g.mipmap == 0); }
    { SInput in = Base(); in.diffuse_index = 0; SLog g = SLog(); Render(in, g); CHECK(g.mipmap == 0); }
    { SInput in = Base(); in.width = 1; SLog g = SLog(); Render(in, g); CHECK(g.mipmap == 0); }        // clamped up to 1
    { SInput in = Base(); in.width = 100000; SLog g = SLog(); Render(in, g); CHECK(g.mipmap == 7); }   // clamped down to 255
    if (g_fail == 0) std::printf("00bb3e80_TEST PASS\n");
    return g_fail;
}
