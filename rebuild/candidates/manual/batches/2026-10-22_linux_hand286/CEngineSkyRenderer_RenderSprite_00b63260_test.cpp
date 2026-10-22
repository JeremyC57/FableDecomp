// Behaviour test for CEngineSkyRenderer::RenderSprite @ 0x00B63260
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
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((unsigned long)A << 8) + R) << 8) + G << 8) + B; }
};
struct SSkySpriteVertex { C3DVector Pos; unsigned long Colour; float U, V; };
static SSkySpriteVertex g_verts[4];
static unsigned short g_indices[6];
static void DrawSprite(CTexture** tex, const SSkySpriteVertex* v, const unsigned short* idx)
{
    for (int i = 0; i < 4; ++i) g_verts[i] = v[i];
    for (int i = 0; i < 6; ++i) g_indices[i] = idx[i];
    DrawAsIndexedTris(tex, 1, v, 0x18, 4, idx, 2);
}
static void RenderSprite(CTexture* texture, const C3DVector& centre, const C3DVector& right, const C3DVector& up, CRGBColour colour)
{
    SSkySpriteVertex verts[4];
    unsigned long argb = colour.GetARGB();
    verts[0].Pos = centre - right + up; verts[0].U = 0.0f; verts[0].V = 0.0f; verts[0].Colour = argb;
    verts[1].Pos = centre + right + up; verts[1].U = 1.0f; verts[1].V = 0.0f; verts[1].Colour = argb;
    verts[2].Pos = centre + right - up; verts[2].U = 1.0f; verts[2].V = 1.0f; verts[2].Colour = argb;
    verts[3].Pos = centre - right - up; verts[3].U = 0.0f; verts[3].V = 1.0f; verts[3].Colour = argb;
    unsigned short indices[6];
    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 0; indices[4] = 2; indices[5] = 3;
    g_shaderMgr.ApplyPixelShader(0);
    g_shaderMgr.ApplyVertexShader(0, false);
    g_shaderMgr.PrepareForRender(0);
    DrawSprite(&texture, verts, indices);
}
int main()
{
    CTexture t;
    CRGBColour c; c.R = 0x11; c.G = 0x22; c.B = 0x33; c.A = 0x44;
    ResetEnv();
    RenderSprite(&t, C3DVector(10, 20, 30), C3DVector(1, 0, 0), C3DVector(0, 2, 0), c);
    if (g_verts[0].Colour != 0x44112233UL || g_verts[0].Pos.X != 9 || g_verts[0].Pos.Y != 22 || g_verts[2].Pos.X != 11
        || g_verts[2].Pos.Y != 18 || g_verts[3].V != 1.0f || g_indices[5] != 3 || g_drawnTexture != &t
        || std::strcmp(g_log, "PSVSRD") != 0) {
        std::printf("00b63260_TEST FAIL\n"); return 1;
    }
    std::printf("00b63260_TEST PASS\n");
    return 0;
}
