// Behaviour test for CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer (0x00B5F090): the effect is supported only
// when the init flags carry 0x80; unsupported, the component is disabled and nothing is looked up; supported, the nine shaders
// are looked up by name in retail's order, the textures are initialised, the texture pointers cleared and the five quads filled
// (half-texel offsets 1/256 and 1/64 on the down-sample quads, the debug quad on the lower-left quarter, white colours on the
// four full-screen quads); both ways the page handle is -1, ShowDebug 0, thresholds 1.33, intensities 0.275, motion blur 1 and
// the environment theme on. A log model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Vert { float X, Y, Z; unsigned long DiffuseColour; float U1, V1; };
struct Shader { const char* Name; };
static Shader Lookup(const char* name) { Log(name); Shader s = { name }; return s; }
struct Glow {
    bool RenderEnabled, UseEnvironmentTheme, EffectSupported;
    void* GlowTexture[4];
    long DynamicTexturePageHandle, ShowDebug;
    Shader VCopy, VBlur, VApply, PGlow, PGlowNeg, PBlur, PApply, VDebug, PDebug;
    Vert Q[5][4];
    float ThresholdR, ThresholdG, ThresholdB, IntensityR, IntensityG, IntensityB, MotionBlur;
    void InitialiseTextures() { Log("init"); GlowTexture[0] = GlowTexture[1] = GlowTexture[2] = GlowTexture[3] = (void*)1; }
    void Construct(long flags)
    {
        RenderEnabled = true;
        if (flags & 0x80) EffectSupported = true;
        else { EffectSupported = false; RenderEnabled = false; }
        DynamicTexturePageHandle = -1;
        ShowDebug = 0;
        if (EffectSupported) {
            VCopy = Lookup("VSHADER_SCREEN_FX_GLOW"); VBlur = Lookup("VSHADER_SCREEN_FX_GLOW_BLUR");
            PGlowNeg = Lookup("PSHADER_SCREEN_FX_GLOW_NEG"); PGlow = Lookup("PSHADER_SCREEN_FX_GLOW");
            PBlur = Lookup("PSHADER_SCREEN_FX_GLOW_BLUR"); VApply = Lookup("VSHADER_SCREEN_FX_DISPLACEMENT");
            PApply = Lookup("PSHADER_TEXTURE_APPLY_GLOW"); VDebug = Lookup("VSHADER_SCREEN_FX_COPY"); PDebug = Lookup("PSHADER_TEXTURE_ONLY");
            InitialiseTextures();
            GlowTexture[0] = GlowTexture[1] = GlowTexture[2] = GlowTexture[3] = 0;
            static const float full[4][2] = { { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };
            static const float quarter[4][2] = { { -1, -1 }, { 0, -1 }, { 0, 0 }, { -1, 0 } };
            static const float uv[4][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };
            const float half[5] = { 0, 1.0f / 256, 1.0f / 64, 0, 0 };
            for (int q = 0; q < 5; ++q)
                for (int i = 0; i < 4; ++i) {
                    Q[q][i].X = q == 4 ? quarter[i][0] : full[i][0];
                    Q[q][i].Y = q == 4 ? quarter[i][1] : full[i][1];
                    Q[q][i].Z = 0;
                    if (q > 0) { Q[q][i].U1 = uv[i][0] + half[q]; Q[q][i].V1 = uv[i][1] + half[q]; }
                    if (q < 4) Q[q][i].DiffuseColour = 0xffffffff;
                }
        }
        ThresholdR = ThresholdG = ThresholdB = 1.33f;
        IntensityR = IntensityG = IntensityB = 0.275f;
        MotionBlur = 1.0f;
        UseEnvironmentTheme = true;
    }
};
static bool Common(const Glow& g)
{
    return g.DynamicTexturePageHandle == -1 && g.ShowDebug == 0 && g.ThresholdR == 1.33f && g.ThresholdG == 1.33f
        && g.ThresholdB == 1.33f && g.IntensityR == 0.275f && g.IntensityB == 0.275f && g.MotionBlur == 1.0f && g.UseEnvironmentTheme;
}
int main()
{
    static Glow g;
    bool ok = true;
    g_log[0] = 0; std::memset(&g, 0, sizeof g);
    g.Construct(0x7f);
    ok = ok && !g.EffectSupported && !g.RenderEnabled && g_log[0] == 0 && Common(g);
    g_log[0] = 0; std::memset(&g, 0, sizeof g);
    g.Construct(0x80);
    ok = ok && g.EffectSupported && g.RenderEnabled && Common(g) && g.GlowTexture[0] == 0 && g.GlowTexture[3] == 0
        && std::strcmp(g_log, "VSHADER_SCREEN_FX_GLOW;VSHADER_SCREEN_FX_GLOW_BLUR;PSHADER_SCREEN_FX_GLOW_NEG;PSHADER_SCREEN_FX_GLOW;"
                              "PSHADER_SCREEN_FX_GLOW_BLUR;VSHADER_SCREEN_FX_DISPLACEMENT;PSHADER_TEXTURE_APPLY_GLOW;"
                              "VSHADER_SCREEN_FX_COPY;PSHADER_TEXTURE_ONLY;init;") == 0
        && g.Q[1][0].U1 == 0.00390625f && g.Q[1][0].V1 == 1.00390625f && g.Q[2][2].U1 == 1.015625f && g.Q[2][2].V1 == 0.015625f
        && g.Q[3][1].U1 == 1.0f && g.Q[4][2].X == 0.0f && g.Q[4][3].X == -1.0f && g.Q[4][3].Y == 0.0f
        && g.Q[0][2].DiffuseColour == 0xffffffff && g.Q[3][3].DiffuseColour == 0xffffffff && g.Q[4][0].DiffuseColour == 0;
    if (!ok) { std::printf("00b5f090_TEST FAIL\n"); return 1; }
    std::printf("00b5f090_TEST PASS\n");
    return 0;
}