// Behaviour test for 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive: a host model of the row's decisions with recording
// stand-ins for the engine (same style as the sibling 0x00BB3E80 test): the alpha states from the sub-primitive render flags (pass 1:
// z-write, no blend, alpha test with the parent's alpha-ref, no colour writes; flag 2: alpha test with alpha * ref >> 8, else none and
// ref 0; flag 1: blending src-alpha / inv-src-alpha with alpha test), the 2D alpha-mask texture mode on stage 1 (point filtering and no
// mipmaps unless the current mask filters; U / V clamped either way), the mask position factors as vertex constant 0 (X, Y, Z, W), the
// cloth vertex shader for 0 lights from the table the flags pick (0x200 shadow, 0x10 spot shadow), the 2D diffuse pixel shader of the
// alpha blend mode, the texture factor (pixel shaders on: constant 2 = tint / 255 with the sub-primitive's alpha; off: the packed
// colour), the diffuse texture at mipmap 1 with the morph info's composite surfaces (none without morph info) and the mask texture
// (bail out with one flush and nothing drawn while either preloads), then self illumination, fog off, two-sided from the material,
// RenderPolygons with (textures, 1, true, two_sided, flags, true, true, 9, pass) and the closing flush.
#include <cstdio>

struct SState { unsigned long ZWrite, AlphaBlend, AlphaTest, AlphaRef, ColourWrite, Src, Dest; unsigned long MagPoint, MinPoint,
                MipOff, ClampU, ClampV, Fog, TextureFactor; };
struct SLog {
    SState rs; int vs_table, vs_index; float vs_const[4]; int ps; float ps_const[4]; int ps_const_set; int diffuse_mip;
    int composite; int flush, polygons, light_update; long self_illum; int poly_textures, poly_two_sided, poly_flags, poly_mode;
};
struct SInput {
    int pass; unsigned long flags; unsigned char alpha, alpha_ref; unsigned char r, g, b; bool pixel_shaders; bool mask_filter;
    float factors[4]; bool morph; bool diffuse_preloading, mask_preloading; long self_illum; bool two_sided; int blend_mode;
};
enum { VS_TRI = 1, VS_SHADOW, VS_SPOT_SHADOW };
enum { PS_2D_DIFFUSE = 1, PS_2D_DIFFUSE_ADDITIVE_ALPHA };
enum { BLEND_SOLID, BLEND_BOOLEAN_ALPHA, BLEND_ALPHA, BLEND_ADDITIVE, BLEND_ADDSMOOTH };

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

static int PixelShader2DDiffuse(int mode)
{
    return (mode == BLEND_ADDITIVE || mode == BLEND_ADDSMOOTH) ? PS_2D_DIFFUSE_ADDITIVE_ALPHA : PS_2D_DIFFUSE;
}

static void Render(const SInput& in, SLog& g)
{
    SetupAlpha(g, in.flags, in.pass, in.alpha, in.alpha_ref);
    if (!in.mask_filter) { g.rs.MagPoint = 1; g.rs.MinPoint = 1; g.rs.MipOff = 1; }
    g.rs.ClampU = 1; g.rs.ClampV = 1;
    for (int i = 0; i < 4; ++i) g.vs_const[i] = in.factors[i];
    g.vs_table = (in.flags & 0x200) ? ((in.flags & 0x10) ? VS_SPOT_SHADOW : VS_SHADOW) : VS_TRI;
    g.vs_index = 0;
    g.ps = PixelShader2DDiffuse(in.blend_mode);
    float colour[4] = { in.r * (1.0f / 255.0f), in.g * (1.0f / 255.0f), in.b * (1.0f / 255.0f), in.alpha * (1.0f / 255.0f) };
    if (in.pixel_shaders) { for (int i = 0; i < 4; ++i) g.ps_const[i] = colour[i]; g.ps_const_set = 2; }
    else {
        unsigned char c[4]; for (int i = 0; i < 4; ++i) c[i] = (unsigned char)(colour[i] * 255.0f);
        g.rs.TextureFactor = ((unsigned long)c[3] << 24) | ((unsigned long)c[0] << 16) | ((unsigned long)c[1] << 8) | c[2];
    }
    g.diffuse_mip = 1; g.composite = in.morph ? 1 : 0;
    if (in.diffuse_preloading || in.mask_preloading) { g.flush++; return; }
    g.self_illum = in.self_illum; g.light_update++;
    g.rs.Fog = 0;
    g.polygons++; g.poly_textures = 1; g.poly_two_sided = in.two_sided; g.poly_flags = (int)in.flags; g.poly_mode = 9;
    g.flush++;
}

static int g_fail = 0;
static void Check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++g_fail; } }
#define CHECK(c) Check((c), __LINE__)
static bool Near(unsigned long v, unsigned long e) { return v + 1 >= e && v <= e + 1; }
static bool NearF(float v, float e) { float d = v - e; return d < 1e-5f && d > -1e-5f; }

static SInput Base()
{
    SInput in = { 0, 0, 200, 128, 255, 128, 0, true, false, { 0.5f, 0.25f, 2.0f, -1.0f }, false, false, false, 40, true, BLEND_ALPHA };
    return in;
}

int main()
{
    { SInput in = Base(); SLog g = SLog(); g.rs.Fog = 1; Render(in, g);
      CHECK(g.polygons == 1 && g.flush == 1 && g.light_update == 1 && g.self_illum == 40 && g.rs.Fog == 0);
      CHECK(g.rs.AlphaTest == 0 && g.rs.AlphaRef == 0 && g.rs.ZWrite == 1 && g.rs.AlphaBlend == 0);
      CHECK(g.rs.MagPoint == 1 && g.rs.MinPoint == 1 && g.rs.MipOff == 1 && g.rs.ClampU == 1 && g.rs.ClampV == 1);
      CHECK(g.vs_const[0] == 0.5f && g.vs_const[1] == 0.25f && g.vs_const[2] == 2.0f && g.vs_const[3] == -1.0f);
      CHECK(g.vs_table == VS_TRI && g.vs_index == 0 && g.ps == PS_2D_DIFFUSE);
      CHECK(g.ps_const_set == 2 && NearF(g.ps_const[0], 1.0f) && NearF(g.ps_const[1], 128.0f / 255.0f) && NearF(g.ps_const[2], 0.0f) && NearF(g.ps_const[3], 200.0f / 255.0f));
      CHECK(g.diffuse_mip == 1 && g.composite == 0 && g.poly_textures == 1 && g.poly_two_sided == 1 && g.poly_mode == 9); }
    { SInput in = Base(); in.mask_filter = true; SLog g = SLog(); Render(in, g);
      CHECK(g.rs.MagPoint == 0 && g.rs.MinPoint == 0 && g.rs.MipOff == 0 && g.rs.ClampU == 1 && g.rs.ClampV == 1); }
    { SInput in = Base(); in.pixel_shaders = false; SLog g = SLog(); Render(in, g);
      unsigned long f = g.rs.TextureFactor;
      CHECK(g.ps_const_set == 0 && Near((f >> 24) & 0xff, 200) && Near((f >> 16) & 0xff, 255) && Near((f >> 8) & 0xff, 128) && Near(f & 0xff, 0)); }
    { SInput in = Base(); in.diffuse_preloading = true; SLog g = SLog(); Render(in, g);
      CHECK(g.polygons == 0 && g.flush == 1 && g.light_update == 0 && g.ps == PS_2D_DIFFUSE); }
    { SInput in = Base(); in.mask_preloading = true; SLog g = SLog(); g.rs.Fog = 1; Render(in, g);
      CHECK(g.polygons == 0 && g.flush == 1 && g.rs.Fog == 1); }
    { SInput in = Base(); in.morph = true; SLog g = SLog(); Render(in, g); CHECK(g.composite == 1); }
    { SInput in = Base(); in.flags = 0x2 | 0x1; SLog g = SLog(); Render(in, g);
      CHECK(g.rs.AlphaTest == 1 && g.rs.AlphaRef == ((200 * 128) >> 8) && g.rs.ZWrite == 0 && g.rs.AlphaBlend == 1 && g.rs.Src == 5 && g.rs.Dest == 6);
      CHECK(g.poly_flags == 3); }
    { SInput in = Base(); in.pass = 1; in.flags = 1; SLog g = SLog(); Render(in, g);
      CHECK(g.rs.ZWrite == 1 && g.rs.AlphaBlend == 0 && g.rs.AlphaTest == 1 && g.rs.AlphaRef == 128 && g.rs.ColourWrite == 0); }
    { SInput in = Base(); in.flags = 0x200; SLog g = SLog(); Render(in, g); CHECK(g.vs_table == VS_SHADOW); }
    { SInput in = Base(); in.flags = 0x210; SLog g = SLog(); Render(in, g); CHECK(g.vs_table == VS_SPOT_SHADOW); }
    { SInput in = Base(); in.flags = 0x10; SLog g = SLog(); Render(in, g); CHECK(g.vs_table == VS_TRI); }
    { SInput in = Base(); in.two_sided = false; SLog g = SLog(); Render(in, g); CHECK(g.poly_two_sided == 0); }
    { CHECK(PixelShader2DDiffuse(BLEND_ADDITIVE) == PS_2D_DIFFUSE_ADDITIVE_ALPHA && PixelShader2DDiffuse(BLEND_ADDSMOOTH) == PS_2D_DIFFUSE_ADDITIVE_ALPHA);
      CHECK(PixelShader2DDiffuse(BLEND_ALPHA) == PS_2D_DIFFUSE && PixelShader2DDiffuse(BLEND_SOLID) == PS_2D_DIFFUSE); }
    if (g_fail == 0) std::printf("00bb30a0_TEST PASS\n");
    return g_fail;
}
