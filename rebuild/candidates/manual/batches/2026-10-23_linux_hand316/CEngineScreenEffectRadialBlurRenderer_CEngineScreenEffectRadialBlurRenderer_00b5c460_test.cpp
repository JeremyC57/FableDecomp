#include <cstdio>
#include <cstring>
// Model of the radial blur renderer constructor: supported only with init flag 0x80 (else the component is
// disabled); when supported the six shaders are looked up, the textures created and -- if that kept the effect
// supported -- the four screen quads filled (the blur-to-blur quad with +-0.5 texture coordinates, the texture ->
// back-buffer quad with a 1/512 half-texel offset, the blur -> back-buffer quad on the lower-left quarter);
// always: no debug view, no textures, page handle -1, environment parameters on, parameters reset.
struct Vert { float X, Y, Z; unsigned long Colour; float U, V; };
static const char* g_names[8]; static int g_nameCount = 0; static int g_texInits = 0, g_resets = 0;
static bool g_textureFails = false;
static int Lookup(const char* name) { g_names[g_nameCount++] = name; return g_nameCount; }
static void Quad(Vert* q, float x1, float y1, float u0, float u1, float v0, float v1)
{
    q[0].X = -1.0f; q[0].Y = -1.0f; q[1].X = x1; q[1].Y = -1.0f; q[2].X = x1; q[2].Y = y1; q[3].X = -1.0f; q[3].Y = y1;
    for (int i = 0; i < 4; ++i) q[i].Z = 0.0f;
    q[0].U = u0; q[0].V = v1; q[1].U = u1; q[1].V = v1; q[2].U = u1; q[2].V = v0; q[3].U = u0; q[3].V = v0;
}
struct Renderer {
    bool RenderEnabled, EffectSupported, UseEnvironment; long ShowDebug, PageHandle; void* Blur1; void* Blur2;
    int Shaders[6];
    Vert BackToTex[4], BlurToBlur[4], TexToBack[4], BlurToBack[4];
    Renderer(unsigned long flags) : RenderEnabled(true), Blur1(&g_names), Blur2(&g_names)
    {
        static const char* names[6] = { "VSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY", "PSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY",
            "VSHADER_SCREEN_FX_RADIAL_BLUR", "PSHADER_SCREEN_FX_RADIAL_BLUR", "PSHADER_TEXTURE_ONLY", "PSHADER_TEXTURE_DIFFUSE" };
        int i;
        for (i = 0; i < 6; ++i) Shaders[i] = 0;
        for (i = 0; i < 4; ++i) BackToTex[i].Colour = 0;
        if (flags & 0x80) EffectSupported = true; else { EffectSupported = false; RenderEnabled = false; }
        if (EffectSupported) for (i = 0; i < 6; ++i) Shaders[i] = Lookup(names[i]);
        ShowDebug = 0; Blur1 = 0; Blur2 = 0; PageHandle = -1;
        if (EffectSupported) {
            ++g_texInits; if (g_textureFails) EffectSupported = false;
            if (EffectSupported) {
                Quad(BackToTex, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
                Quad(BlurToBlur, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.5f);
                Quad(TexToBack, 1.0f, 1.0f, 0.001953125f, 1.001953125f, 0.001953125f, 1.001953125f);
                Quad(BlurToBack, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
                for (long k = 0; k < 4; ++k) { BackToTex[k].Colour = 0xffffffff; BlurToBlur[k].Colour = 0xffffffff; TexToBack[k].Colour = 0xffffffff; BlurToBack[k].Colour = 0xffffffff; }
            }
        }
        UseEnvironment = true; ++g_resets;
    }
};
int main()
{
    int bad = 0;
    Renderer off(0x08);
    if (off.EffectSupported || off.RenderEnabled || g_nameCount != 0 || g_texInits != 0 || off.Blur1 || off.PageHandle != -1 || !off.UseEnvironment || g_resets != 1) bad = 1;
    g_textureFails = true;
    Renderer failed(0x80);
    if (g_nameCount != 6 || g_texInits != 1 || failed.EffectSupported || failed.BackToTex[0].Colour != 0) bad = 1;
    g_textureFails = false; g_nameCount = 0;
    Renderer on(0x80);
    if (!on.EffectSupported || !on.RenderEnabled || g_nameCount != 6 || on.Shaders[5] != 6 || std::strcmp(g_names[4], "PSHADER_TEXTURE_ONLY") != 0) bad = 1;
    if (on.BlurToBlur[0].U != -0.5f || on.BlurToBlur[2].V != -0.5f || on.TexToBack[0].V != 1.001953125f) bad = 1;
    if (on.BlurToBack[1].X != 0.0f || on.BlurToBack[3].Y != 0.0f || on.BlurToBack[3].Colour != 0xffffffff || g_resets != 3) bad = 1;
    if (bad) { std::printf("00b5c460_TEST FAIL\n"); return 1; }
    std::printf("00b5c460_TEST PASS\n");
    return 0;
}
