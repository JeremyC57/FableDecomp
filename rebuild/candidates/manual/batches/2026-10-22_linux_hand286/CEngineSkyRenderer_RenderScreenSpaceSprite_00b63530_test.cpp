// Behaviour test for CEngineSkyRenderer::RenderScreenSpaceSprite @ 0x00B63530
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CTexture { int id; };
struct CVertexShader { int id; };
struct CPixelShader { int id; };
struct CStaticShaderManager {
    bool VertexShadersEnabled, PixelShadersEnabled;
    long CurrentVSConstantLayoutIndex;
    void SetVertexShadersEnabled(bool b) { VertexShadersEnabled = b; Log(b ? "v+" : "v-"); }
    void SetPixelShadersEnabled(bool b) { PixelShadersEnabled = b; Log(b ? "p+" : "p-"); }
    void SetVertexShaderConstantLayout(long l) { CurrentVSConstantLayoutIndex = l; Log("L"); }
};
static CStaticShaderManager g_ShaderRenderManager;
struct CShaderRenderManagerMock {
    CVertexShader* vs; CPixelShader* ps;
    void ApplyVertexShader(CVertexShader* s, bool) { vs = s; Log("VS"); }
    void ApplyPixelShader(CPixelShader* s) { ps = s; Log("PS"); }
    void PrepareForRender(unsigned long) { Log("R"); }
};
static CShaderRenderManagerMock g_shaderMgr;
struct CRenderStateManagerMock {
    int depth;
    void OpenCaptureBlock() { ++depth; Log("["); }
    void RestoreCaptureBlock() { --depth; Log("]"); }
};
static CRenderStateManagerMock g_rsm;
static CTexture* g_drawnTexture;
static long g_drawnStride, g_drawnVerts;
static void DrawAsIndexedTris(CTexture** tex, long, const void*, long stride, long verts, const unsigned short*, long)
{
    g_drawnTexture = *tex; g_drawnStride = stride; g_drawnVerts = verts; Log("D");
}
struct CShaderEnableSaver {
    bool m_vertexShaders, m_pixelShaders;
    CShaderEnableSaver(bool v, bool p)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled; g_ShaderRenderManager.SetVertexShadersEnabled(v);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled; g_ShaderRenderManager.SetPixelShadersEnabled(p);
    }
    ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};
struct CShaderLayoutSaver {
    long m_layout;
    CShaderLayoutSaver(long l) { m_layout = g_ShaderRenderManager.CurrentVSConstantLayoutIndex; g_ShaderRenderManager.SetVertexShaderConstantLayout(l); }
    ~CShaderLayoutSaver() { g_ShaderRenderManager.SetVertexShaderConstantLayout(m_layout); }
};
struct CRenderStateCaptureBlock {
    CRenderStateCaptureBlock() { g_rsm.OpenCaptureBlock(); }
    ~CRenderStateCaptureBlock() { g_rsm.RestoreCaptureBlock(); }
};
static void ResetEnv() { g_log[0] = 0; g_drawnTexture = 0; g_ShaderRenderManager.VertexShadersEnabled = false; g_ShaderRenderManager.PixelShadersEnabled = false; g_ShaderRenderManager.CurrentVSConstantLayoutIndex = 0; g_rsm.depth = 0; }
struct C2DVector { float X, Y; };
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) + R) << 8) + G << 8) + B; }
};
static void ScreenToProjection(C2DVector& p) { p.X = p.X * 2.0f - 1.0f; p.Y = 1.0f - p.Y * 2.0f; }
struct SScreenSpriteVertex { float X, Y, Z, W; unsigned long Colour, Unused; float U, V; };
static SScreenSpriteVertex g_verts[4];
static void RenderScreenSpaceSprite(CTexture* texture, C2DVector topLeft, C2DVector size, CRGBColour colour)
{
    C2DVector bottomRight;
    bottomRight.X = size.X + topLeft.X;
    bottomRight.Y = size.Y + topLeft.Y;
    unsigned long argb = colour.GetARGB();
    C2DVector corner = topLeft;
    ScreenToProjection(corner);
    ScreenToProjection(bottomRight);
    SScreenSpriteVertex verts[4];
    float xs[4] = { corner.X, bottomRight.X, bottomRight.X, corner.X };
    float ys[4] = { corner.Y, corner.Y, bottomRight.Y, bottomRight.Y };
    float us[4] = { 0, 1, 1, 0 };
    float vs[4] = { 0, 0, 1, 1 };
    for (int i = 0; i < 4; ++i) {
        verts[i].X = xs[i]; verts[i].Y = ys[i]; verts[i].Z = 0.9999f; verts[i].U = us[i]; verts[i].V = vs[i];
        verts[i].Colour = argb; verts[i].W = 1.0f;
    }
    g_shaderMgr.ApplyVertexShader(0, false);
    g_shaderMgr.ApplyPixelShader(0);
    g_shaderMgr.PrepareForRender(0);
    for (int i = 0; i < 4; ++i) g_verts[i] = verts[i];
    DrawAsIndexedTris(&texture, 1, verts, 0x20, 4, 0, 2);
}
int main()
{
    CTexture t; CRGBColour c; c.R = 1; c.G = 2; c.B = 3; c.A = 0x80;
    C2DVector tl; tl.X = 0.25f; tl.Y = 0.25f; C2DVector sz; sz.X = 0.5f; sz.Y = 0.25f;
    ResetEnv();
    RenderScreenSpaceSprite(&t, tl, sz, c);
    // corner (-0.5, 0.5), bottomRight (0.75,0.5) -> (0.5, 0)
    if (g_verts[0].X != -0.5f || g_verts[0].Y != 0.5f || g_verts[2].X != 0.5f || g_verts[2].Y != 0.0f
        || g_verts[1].Colour != 0x80010203UL || g_drawnStride != 0x20 || std::strcmp(g_log, "VSPSRD") != 0) {
        std::printf("00b63530_TEST FAIL\n"); return 1;
    }
    std::printf("00b63530_TEST PASS\n");
    return 0;
}
