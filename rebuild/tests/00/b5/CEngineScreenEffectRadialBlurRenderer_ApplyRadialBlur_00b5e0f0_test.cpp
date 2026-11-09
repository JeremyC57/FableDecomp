// Behaviour test for CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur 0x00B5E0F0: the four TextureToBackBufferVerts get
// the opaque grey 0xFFgggggg with g = Fade * 255 truncated and clamped to 0..255; inside one capture block (opened first,
// restored last), vertex constant layout 2 and both shader kinds forced on (restored in reverse order), the screen-copy vertex
// shader and texture-diffuse pixel shader are applied, the nine render states are set (alpha blend on, alpha test off, src/dst
// blend ONE, z off, cull none, dither off, linear mag/min filter on stage 0) with each changed state bookmarked once, the shader
// constants prepared, and BlurTexture256_2 drawn as two indexed triangles of the four 0x18-byte vertices.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }

enum { ALPHA_BLEND_ONE = 1 };
enum { POLY_CULL_NONE = 2 };
enum { TEXTURE_FILTER_LINEAR = 1 };
static const unsigned long D3DAlphaBlendModeTable[13] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
static const unsigned long D3DPolyCullModeTable[3] = { 3, 2, 1 };
static const unsigned long D3DTextureFilterModeTable[3] = { 1, 2, 3 };

struct CStateInfo { unsigned long DesiredState, BookmarkMask; bool DirtyListFlag; };
class CTexture { public: int Id; };
class CVertexShader { public: int Id; };
class CPixelShader { public: int Id; };
struct CTVertexColTex1 { float X, Y, Z; unsigned int DiffuseColour; float U1, V1; };   // 0x18 bytes (32-bit colour on LP64 hosts too)

struct CRenderManager {                     // the inline CRenderStateManager setters
    CStateInfo AlphaBlendEnable, AlphaTestEnable, SourceAlphaBlendMode, DestAlphaBlendMode, ZBufferEnable, PolyCullMode,
               DitherEnable, FilterMag, FilterMin;
    unsigned long CurrentBookmarkMask; int StackSize, DirtySize, Depth;
    const CTexture* DrawnTexture; unsigned long DrawnCount; const void* DrawnVerts; long Stride, VertexCount, Triangles;
    const unsigned short* Indices;
    void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            if ((state.BookmarkMask & CurrentBookmarkMask) == 0) { state.BookmarkMask |= CurrentBookmarkMask; ++StackSize; }
            state.DesiredState = value;
            if (!state.DirtyListFlag) { state.DirtyListFlag = true; ++DirtySize; }
        }
    }
    void SetAlphaBlendEnable(bool e) { SetRenderState(e ? 1 : 0, AlphaBlendEnable); }
    void SetAlphaTestEnable(bool e) { SetRenderState(e ? 1 : 0, AlphaTestEnable); }
    void SetSourceAlphaBlendMode(int m) { SetRenderState(D3DAlphaBlendModeTable[m], SourceAlphaBlendMode); }
    void SetDestAlphaBlendMode(int m) { SetRenderState(D3DAlphaBlendModeTable[m], DestAlphaBlendMode); }
    void SetZBufferEnable(bool e) { SetRenderState(e ? 1 : 0, ZBufferEnable); }
    void SetPolyCullMode(int m) { SetRenderState(D3DPolyCullModeTable[m], PolyCullMode); }
    void SetDitherEnable(bool e) { SetRenderState(e ? 1 : 0, DitherEnable); }
    void SetTextureFilterModeMag(long, int m) { SetRenderState(D3DTextureFilterModeTable[m], FilterMag); }
    void SetTextureFilterModeMin(long, int m) { SetRenderState(D3DTextureFilterModeTable[m], FilterMin); }
    void StartCaptureBlock() { ++Depth; CurrentBookmarkMask <<= 1; Log("["); }
    void RestoreCaptureBlock() { --Depth; CurrentBookmarkMask >>= 1; Log("]"); }
    void DrawAsIndexedTris(const CTexture** textures, unsigned long count, const void* verts, long stride, long vertexCount,
                           unsigned short* indices, long triangles)
    {
        DrawnTexture = *textures; DrawnCount = count; DrawnVerts = verts; Stride = stride; VertexCount = vertexCount;
        Indices = indices; Triangles = triangles; Log("D");
    }
};
static CRenderManager g_rm;
struct CShaderRenderManager {
    bool VertexShadersEnabled, PixelShadersEnabled; long Layout;
    const CVertexShader* VS; const CPixelShader* PS;
    void SetVertexShadersEnable(bool e, bool* old) { if (old) *old = VertexShadersEnabled; VertexShadersEnabled = e; Log(e ? "v+" : "v-"); }
    void SetPixelShadersEnable(bool e, bool* old) { if (old) *old = PixelShadersEnabled; PixelShadersEnabled = e; Log(e ? "p+" : "p-"); }
    void SetVertexShaderConstantLayout(long l) { Layout = l; Log("L"); }
    void ApplyVertexShader(const CVertexShader& s, bool) { VS = &s; Log("VS"); }
    void ApplyPixelShader(const CPixelShader& s) { PS = &s; Log("PS"); }
    void PrepareForRender(unsigned long) { Log("R"); }
};
static CShaderRenderManager g_srm;
struct CRenderStateScopedCaptureBlock { CRenderStateScopedCaptureBlock() { g_rm.StartCaptureBlock(); } ~CRenderStateScopedCaptureBlock() { g_rm.RestoreCaptureBlock(); } };
struct CScopedVertexShaderConstantLayout {
    long OldIndex;
    CScopedVertexShaderConstantLayout(long i) { OldIndex = g_srm.Layout; g_srm.SetVertexShaderConstantLayout(i); }
    ~CScopedVertexShaderConstantLayout() { g_srm.SetVertexShaderConstantLayout(OldIndex); }
};
struct CScopedShadersEnable {
    bool OldVertexEnable, OldPixelEnable;
    CScopedShadersEnable(bool v, bool p) { g_srm.SetVertexShadersEnable(v, &OldVertexEnable); g_srm.SetPixelShadersEnable(p, &OldPixelEnable); }
    ~CScopedShadersEnable() { g_srm.SetVertexShadersEnable(OldVertexEnable, 0); g_srm.SetPixelShadersEnable(OldPixelEnable, 0); }
};

struct CEngineScreenEffectRadialBlurRenderer {
    CTexture* BlurTexture256_1; CTexture* BlurTexture256_2;
    CVertexShader ScreenCopyVertexShader; CPixelShader TextureDiffusePixelShader;
    CTVertexColTex1 TextureToBackBufferVerts[4];
    struct { float Fade; } CurrentParameters;
    void ApplyRadialBlur()
    {
        static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };
        long fade = (long)(CurrentParameters.Fade * 255.0f);
        if (fade < 0)
            fade = 0;
        else if (fade > 255)
            fade = 255;
        for (long i = 0; i < 4; ++i)
            TextureToBackBufferVerts[i].DiffuseColour = fade * 0x10101 + 0xff000000;
        CRenderStateScopedCaptureBlock capture;
        CScopedVertexShaderConstantLayout layout(2);
        CScopedShadersEnable shaders(true, true);
        g_srm.ApplyVertexShader(ScreenCopyVertexShader, false);
        g_srm.ApplyPixelShader(TextureDiffusePixelShader);
        g_rm.SetAlphaBlendEnable(true);
        g_rm.SetAlphaTestEnable(false);
        g_rm.SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        g_rm.SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        g_rm.SetZBufferEnable(false);
        g_rm.SetPolyCullMode(POLY_CULL_NONE);
        g_rm.SetDitherEnable(false);
        g_rm.SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
        g_rm.SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
        g_srm.PrepareForRender(0);
        const CTexture* texture = BlurTexture256_2;
        g_rm.DrawAsIndexedTris(&texture, 1, TextureToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    }
};

static int Run(float fadeValue, unsigned long expected, int changed)
{
    std::memset(&g_rm, 0, sizeof g_rm); std::memset(&g_srm, 0, sizeof g_srm); g_log[0] = 0;
    g_rm.CurrentBookmarkMask = 1; g_srm.Layout = 7; g_srm.VertexShadersEnabled = false; g_srm.PixelShadersEnabled = true;
    if (!changed) {                          // every state already at the requested value: nothing bookmarked or queued
        g_rm.AlphaBlendEnable.DesiredState = 1; g_rm.SourceAlphaBlendMode.DesiredState = 2; g_rm.DestAlphaBlendMode.DesiredState = 2;
        g_rm.PolyCullMode.DesiredState = 1; g_rm.FilterMag.DesiredState = 2; g_rm.FilterMin.DesiredState = 2;
    } else {
        g_rm.AlphaTestEnable.DesiredState = 1; g_rm.ZBufferEnable.DesiredState = 1; g_rm.DitherEnable.DesiredState = 1;
    }
    CTexture a, b; CEngineScreenEffectRadialBlurRenderer r;
    std::memset(&r, 0, sizeof r); r.BlurTexture256_1 = &a; r.BlurTexture256_2 = &b; r.CurrentParameters.Fade = fadeValue;
    r.ApplyRadialBlur();
    for (int i = 0; i < 4; ++i) if (r.TextureToBackBufferVerts[i].DiffuseColour != expected) return 1;
    if (std::strcmp(g_log, "[Lv+p+VSPSRDv-p+L]") != 0) return 2;
    if (g_srm.Layout != 7 || g_srm.VertexShadersEnabled || !g_srm.PixelShadersEnabled) return 3;
    if (g_srm.VS != &r.ScreenCopyVertexShader || g_srm.PS != &r.TextureDiffusePixelShader) return 4;
    if (g_rm.DrawnTexture != &b || g_rm.DrawnCount != 1 || g_rm.DrawnVerts != r.TextureToBackBufferVerts || g_rm.Stride != 0x18 ||
        g_rm.VertexCount != 4 || g_rm.Triangles != 2 || g_rm.Indices[0] != 0 || g_rm.Indices[2] != 3 || g_rm.Indices[5] != 3) return 5;
    if (g_rm.AlphaBlendEnable.DesiredState != 1 || g_rm.AlphaTestEnable.DesiredState != 0 || g_rm.SourceAlphaBlendMode.DesiredState != 2 ||
        g_rm.DestAlphaBlendMode.DesiredState != 2 || g_rm.ZBufferEnable.DesiredState != 0 || g_rm.PolyCullMode.DesiredState != 1 ||
        g_rm.DitherEnable.DesiredState != 0 || g_rm.FilterMag.DesiredState != 2 || g_rm.FilterMin.DesiredState != 2) return 6;
    if (g_rm.StackSize != (changed ? 9 : 0) || g_rm.DirtySize != (changed ? 9 : 0) || g_rm.Depth != 0) return 7;
    return 0;
}

int main()
{
    int r;
    if ((r = Run(0.5f, 0xff7f7f7fUL, 1)) || (r = Run(-1.0f, 0xff000000UL, 0)) || (r = Run(3.0f, 0xffffffffUL, 1)) ||
        (r = Run(1.0f, 0xffffffffUL, 0)) || (r = Run(0.2f, 0xff333333UL, 1))) {
        std::printf("00b5e0f0_TEST FAIL %d\n", r); return 1;
    }
    std::printf("00b5e0f0_TEST PASS\n");
    return 0;
}