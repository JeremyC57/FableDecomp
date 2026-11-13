// Behaviour test for 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates(long, const CMatrix3x4&, const CColorBufferDepthPackParameters&):
// a host model of the body over mock render-state / shader managers (the state setters record the D3D value per stage, the texture
// attach records the texture per stage, the shader manager records constants and the two transforms). Kept in step with the source's
// named accessor results (chunk 3004).
#include <cstdio>

struct Matrix3x4 { float E[4][3]; };
struct Matrix4x4 {
    float Data[4][4];
    Matrix4x4() { }
    Matrix4x4(const Matrix3x4& m) { Initialise(m); }
    void Initialise(const Matrix3x4& m)
    {
        for (int i = 0; i < 4; ++i) { Data[i][0] = m.E[i][0]; Data[i][1] = m.E[i][1]; Data[i][2] = m.E[i][2]; Data[i][3] = (i == 3) ? 1.0f : 0.0f; }
    }
    void PostMultiply(const Matrix4x4& m)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.Data[i][j] = Data[i][0] * m.Data[0][j] + Data[i][1] * m.Data[1][j] + Data[i][2] * m.Data[2][j] + Data[i][3] * m.Data[3][j];
        *this = result;
    }
};
struct Vec4 { float X, Y, Z, W; Vec4() { } Vec4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { } };
struct Texture { int Id; };

static const unsigned long FilterTable[3] = { 1, 2, 3 };              // D3DTEXF_POINT, LINEAR, ANISOTROPIC
static const unsigned long UVTable[4] = { 1, 2, 3, 4 };               // WRAP, MIRROR, CLAMP, BORDER
static const unsigned long TransformTable[10] = { 0, 1, 2, 3, 4, 0x100, 0x101, 0x102, 0x103, 0x104 };
struct RenderManager {
    const Texture* Textures[8]; unsigned long Mag[8], Min[8], U[8], V[8], Transform[8]; long StagesInUse;
    void Attach(long stage, const Texture* t) { Textures[stage] = t; if (StagesInUse < stage + 1) StagesInUse = stage + 1; }
};
struct ShaderManager {
    Vec4 PS[8]; Vec4 VS[8]; Matrix4x4 ShadowTexture; Vec4 Fade; bool FadeSet; unsigned long UpdateFlags;
    void SetPixelShaderConstant(unsigned long i, const Vec4& v) { PS[i] = v; UpdateFlags |= 2; }
};
static RenderManager g_rm; static ShaderManager g_srm; static long g_shadowMode;
struct Pack { long DepthToColourLookupTextureStage, LowByteCarryPixelShaderConstant, LowRepeatFactorUserVertexShaderConstant; };
struct ShadowRenderer;
static ShadowRenderer* g_shadowRenderer;
struct ShadowRenderer {
    Matrix4x4 WorldToShadowBufferMatrixTextureSpace; float ShadowFadeStart, ShadowFadeEnd;
    Texture ShadowBufferTexture, DepthPackingLookupTexture; long RenderingTechnique;
    void Setup(long stage, const Matrix3x4& texture_transform, const Pack& pack)
    {
        if (pack.DepthToColourLookupTextureStage < 0)
            return;
        Texture& shadow_buffer_texture = ShadowBufferTexture;
        g_rm.Attach(stage, &shadow_buffer_texture);
        long technique = RenderingTechnique;
        if (technique == 2) {
            Texture& depth_packing_texture = g_shadowRenderer->DepthPackingLookupTexture;
            g_rm.Attach(pack.DepthToColourLookupTextureStage, &depth_packing_texture);
            g_rm.Mag[stage] = FilterTable[0]; g_rm.Min[stage] = FilterTable[0];
            g_rm.Mag[pack.DepthToColourLookupTextureStage] = FilterTable[0]; g_rm.Min[pack.DepthToColourLookupTextureStage] = FilterTable[0];
            g_rm.U[pack.DepthToColourLookupTextureStage] = UVTable[0]; g_rm.V[pack.DepthToColourLookupTextureStage] = UVTable[2];
            g_srm.SetPixelShaderConstant(pack.LowByteCarryPixelShaderConstant, Vec4(0.0048828125f, -0.0048828125f, 0.0f, 0.0f));
            g_srm.VS[pack.LowRepeatFactorUserVertexShaderConstant] = Vec4(128.0f, 128.0f, 128.0f, 128.0f);
        } else {
            g_rm.Mag[stage] = FilterTable[1]; g_rm.Min[stage] = FilterTable[1]; g_rm.Transform[stage] = TransformTable[5];
        }
        Matrix4x4 shadow_texture_transform(texture_transform);
        const Matrix4x4& world_to_shadow = WorldToShadowBufferMatrixTextureSpace;
        shadow_texture_transform.PostMultiply(world_to_shadow);
        g_srm.ShadowTexture = shadow_texture_transform;
        long shadow_mode = g_shadowMode;
        if (shadow_mode == 1) {
            float fade_scale = 1.0f / (ShadowFadeStart - ShadowFadeEnd);
            g_srm.Fade = Vec4(fade_scale, 0.0f, 0.0f, -(fade_scale * ShadowFadeEnd)); g_srm.FadeSet = true;
        }
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    bool ok = true;
    static ShadowRenderer r;
    g_shadowRenderer = &r;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) r.WorldToShadowBufferMatrixTextureSpace.Data[i][j] = (float)(i * 4 + j + 1);
    r.ShadowFadeStart = 50.0f; r.ShadowFadeEnd = 10.0f; r.ShadowBufferTexture.Id = 7; r.DepthPackingLookupTexture.Id = 9; r.RenderingTechnique = 2;
    Matrix3x4 t; for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) t.E[i][j] = (i == j) ? 2.0f : (i == 3 ? 1.0f : 0.0f);
    Pack pack = { 4, 5, 0 };
    g_shadowMode = 1;
    r.Setup(1, t, pack);
    ok = ok && g_rm.Textures[1] == &r.ShadowBufferTexture && g_rm.Textures[4] == &r.DepthPackingLookupTexture && g_rm.StagesInUse == 5;
    ok = ok && g_rm.Mag[1] == 1 && g_rm.Min[4] == 1 && g_rm.U[4] == 1 && g_rm.V[4] == 3 && g_srm.UpdateFlags == 2;
    ok = ok && Near(g_srm.PS[5].X, 0.0048828125f) && Near(g_srm.PS[5].Y, -0.0048828125f) && Near(g_srm.VS[0].W, 128.0f);
    // row 0 = (2,0,0,0) * M = 2 * row0(M); row 3 = (1,1,1,1) * M = column sums
    ok = ok && Near(g_srm.ShadowTexture.Data[0][0], 2.0f) && Near(g_srm.ShadowTexture.Data[0][3], 8.0f)
            && Near(g_srm.ShadowTexture.Data[3][0], 1.0f + 5.0f + 9.0f + 13.0f) && Near(g_srm.ShadowTexture.Data[2][2], 2.0f * 11.0f);
    ok = ok && g_srm.FadeSet && Near(g_srm.Fade.X, 1.0f / 40.0f) && Near(g_srm.Fade.W, -10.0f / 40.0f) && Near(g_srm.Fade.Y, 0.0f);
    r.RenderingTechnique = 1; g_shadowMode = 0; g_srm.FadeSet = false;
    r.Setup(2, t, pack);
    ok = ok && g_rm.Mag[2] == 2 && g_rm.Min[2] == 2 && g_rm.Transform[2] == 0x100 && g_rm.Textures[2] == &r.ShadowBufferTexture && !g_srm.FadeSet;
    Pack none = { -1, 5, 0 };
    g_rm.Textures[3] = 0;
    r.Setup(3, t, none);
    ok = ok && g_rm.Textures[3] == 0;
    if (!ok) { std::printf("00b4fc40_TEST FAIL\n"); return 1; }
    std::printf("00b4fc40_TEST PASS\n");
    return 0;
}