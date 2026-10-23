// Behaviour test for CEngineWeatherRenderer::RenderMist @ 0x00B54310 (restated with fakes).
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { C3DVector XAxis, YAxis, ZAxis, Position; };

struct FakeDevice {
    void* textures[8];
    int setTextureCalls;
    void* indices;
    void* stream;
    unsigned int stride;
    FakeDevice() : setTextureCalls(0), indices(0), stream(0), stride(0) { for (int i = 0; i < 8; ++i) textures[i] = 0; }
    virtual void SetTexture(unsigned long stage, void* t) { textures[stage] = t; ++setTextureCalls; }
    virtual void SetStreamSource(unsigned int, void* d, unsigned int, unsigned int s) { stream = d; stride = s; }
    virtual void SetIndices(void* i) { indices = i; }
};

struct CBaseTextureView { void* vp; void* PD3DTexture; };
struct CIndexBufferView { void* vp; void* PIndexBuffer; };
struct CVertexBufferView { void* vp; void* PVertexBuffer; unsigned char pad[8]; unsigned char Stride; };

struct CTrackedRenderState { long id; unsigned long m_value; unsigned long m_captureMask; long applied; bool m_queued; };

struct Core {
    FakeDevice* m_pDevice;
    void* m_attachedTextures[8];
    long m_activeTextureStageCount;
    const CIndexBufferView* m_pCurrentIndices;
    CTrackedRenderState m_state2890;
    CTrackedRenderState* m_dirty[16];
    long m_dirtyCount;
    int realised;
    unsigned long draw[4];
    void SetTexture(long stage, const CBaseTextureView* texture)
    {
        void* deviceTexture = texture ? texture->PD3DTexture : 0;
        if (m_attachedTextures[stage] != deviceTexture) {
            m_attachedTextures[stage] = deviceTexture;
            m_pDevice->SetTexture(stage, deviceTexture);
        }
        if (m_activeTextureStageCount < stage + 1)
            m_activeTextureStageCount = stage + 1;
    }
    void SetIndices(const CIndexBufferView* indices)
    {
        m_pCurrentIndices = indices;
        if (indices) m_pDevice->SetIndices(indices->PIndexBuffer); else m_pDevice->SetIndices(0);
    }
    void SetStreamSource(const CVertexBufferView* v)
    {
        if (v) m_pDevice->SetStreamSource(0, v->PVertexBuffer, 0, v->Stride); else m_pDevice->SetStreamSource(0, 0, 0, 0);
    }
};

struct Shader {
    float c[10][4];
    int pixel, vertex, prepared;
    void SetUserVertexShaderConstant(unsigned long i, float x, float y, float z, float w) { c[i][0] = x; c[i][1] = y; c[i][2] = z; c[i][3] = w; }
};

struct Weather {
    CVertexBufferView vb;
    CIndexBufferView ib;
    CBaseTextureView* tex;
    float height;
    C3DVector colour, scale, p1, p2, p3;
    float fogScale;
    C3DVector fog;
};

static int restores = 0;
static int opens = 0;

static void RenderMist(Weather& w, Core& core, Shader& sh, const CMatrix3x4& view, const C3DVector& mpos, float mscale)
{
    ++opens;
    if (!w.tex) { ++restores; return; }
    for (long s = 0; s < 4; ++s) core.SetTexture(s, w.tex);
    ++sh.pixel; ++sh.vertex;
    core.SetIndices(&w.ib);
    core.SetStreamSource(&w.vb);
    sh.SetUserVertexShaderConstant(0, mpos.X, mpos.Y, mpos.Z, w.height * mscale);
    sh.SetUserVertexShaderConstant(1, w.scale.X, w.scale.Y, w.scale.Z, 1.0f);
    sh.SetUserVertexShaderConstant(2, view.XAxis.X, view.YAxis.X, view.ZAxis.X, view.Position.X);
    sh.SetUserVertexShaderConstant(3, view.XAxis.Y, view.YAxis.Y, view.ZAxis.Y, view.Position.Y);
    sh.SetUserVertexShaderConstant(4, view.XAxis.Z, view.YAxis.Z, view.ZAxis.Z, view.Position.Z);
    sh.SetUserVertexShaderConstant(5, w.fogScale * w.fog.X, w.fogScale * w.fog.Y, w.fogScale * w.fog.Z, 0.0f);
    sh.SetUserVertexShaderConstant(6, w.colour.X, w.colour.Y, w.colour.Z, 0.0f);
    sh.SetUserVertexShaderConstant(7, w.p1.X, w.p1.Y, w.p1.Z, 1.0f);
    sh.SetUserVertexShaderConstant(8, w.p2.X, w.p2.Y, w.p2.Z, 1.0f);
    sh.SetUserVertexShaderConstant(9, w.p3.X, w.p3.Y, w.p3.Z, 1.0f);
    if (core.m_state2890.m_value != 55) {
        core.m_state2890.m_value = 55;
        if (!core.m_state2890.m_queued) { core.m_state2890.m_queued = true; core.m_dirty[core.m_dirtyCount++] = &core.m_state2890; }
    }
    ++sh.prepared;
    ++core.realised;
    core.draw[0] = 0; core.draw[1] = 4; core.draw[2] = 0; core.draw[3] = 2;
    ++restores;
}

int main()
{
    int fails = 0;
    FakeDevice dev;
    Core core; std::memset(&core, 0, sizeof core);
    core.m_pDevice = &dev;
    Shader sh; std::memset(&sh, 0, sizeof sh);
    Weather w; std::memset(&w, 0, sizeof w);
    int devTex = 0, devIb = 0, devVb = 0;
    CBaseTextureView tex; tex.vp = 0; tex.PD3DTexture = &devTex;
    w.ib.PIndexBuffer = &devIb; w.vb.PVertexBuffer = &devVb; w.vb.Stride = 24;
    w.height = 2.0f; w.fogScale = 0.5f; w.fog.X = 2.0f; w.fog.Y = 4.0f; w.fog.Z = 6.0f;
    CMatrix3x4 view; std::memset(&view, 0, sizeof view);
    view.XAxis.X = 1.0f; view.YAxis.X = 2.0f; view.ZAxis.X = 3.0f; view.Position.X = 4.0f;
    C3DVector mpos; mpos.X = 10.0f; mpos.Y = 20.0f; mpos.Z = 30.0f;

    w.tex = 0;
    RenderMist(w, core, sh, view, mpos, 3.0f);
    if (opens != 1 || restores != 1 || dev.setTextureCalls != 0) ++fails;

    w.tex = &tex;
    RenderMist(w, core, sh, view, mpos, 3.0f);
    if (dev.setTextureCalls != 4 || core.m_activeTextureStageCount != 4) ++fails;
    if (dev.textures[3] != &devTex || dev.indices != &devIb || dev.stream != &devVb || dev.stride != 24) ++fails;
    if (sh.c[0][3] != 6.0f || sh.c[0][0] != 10.0f) ++fails;
    if (sh.c[2][1] != 2.0f || sh.c[2][3] != 4.0f) ++fails;
    if (sh.c[5][0] != 1.0f || sh.c[5][2] != 3.0f || sh.c[5][3] != 0.0f) ++fails;
    if (sh.c[9][3] != 1.0f) ++fails;
    if (core.m_dirtyCount != 1 || core.draw[1] != 4 || core.draw[3] != 2 || restores != 2) ++fails;

    RenderMist(w, core, sh, view, mpos, 3.0f);
    if (dev.setTextureCalls != 4 || core.m_dirtyCount != 1) ++fails;   // cache hits, state unchanged

    if (fails == 0) std::printf("00b54310_TEST PASS\n"); else std::printf("00b54310_TEST FAIL %d\n", fails);
    return fails;
}