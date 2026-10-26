#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };

template <class T> class CArray { public:
    T* First; T* Last; T* End;
    unsigned long Size() const { return (unsigned long)(Last - First); }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CShaderConstantRange { public: void* _vptr; long Offset; unsigned long Count; };
class CVertexShaderConstantLayout { public: void* _vptr; CShaderConstantRange Reserved; CArray<C4DVector> ReservedPresets; };

static char g_log[256];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
static long g_vsReg; static const void* g_vsData; static unsigned long g_vsCount;
static unsigned long g_constIndex; static float g_cx, g_cy, g_cz, g_cw;

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w)
    { g_constIndex = index; g_cx = x; g_cy = y; g_cz = z; g_cw = w; Log('U'); }
    void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1) UpdatePixelShader();
        if (UpdateFlags & 2) UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20)) Log('b');
        if (flags & 0x40) { if (UpdateFlags & 0x1000) Log('t'); }
        else if (flags & 0x80) { if (UpdateFlags & 0x800) Log('o'); }
        if (flags & 0x200) { if (UpdateFlags & 0x2000) Log('f'); }
        else if (flags & 0x100) { if (UpdateFlags & 0x4000) Log('g'); }
        if (UpdateFlags & 4) UpdateAmbient();
        if (UpdateFlags & 0x20000) UpdateFogColour();
        if (UpdateFlags & 0x400) UpdateLightGlobals();
        if (flags & 8) { if (UpdateFlags & 0x100) Log('l'); }
        else if (flags & 0x10) { if (UpdateFlags & 0x200) Log('w'); }
        if ((flags & 0x20) && (UpdateFlags & 0x8000)) Log('s');
        if (!(flags & 1) && !WorldMatrixIsIdentity) Log('R');
        if (UpdateFlags & 0x80) UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8)) UpdateCameraPos();
    }
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    bool WorldMatrixIsIdentity;
    unsigned long UpdateFlags;
private:
    void SetVSConstants(long reg, const void* data, unsigned long count) { g_vsReg = reg; g_vsData = data; g_vsCount = count; Log('V'); }
    void UpdatePixelShader() { UpdateFlags &= ~1ul; Log('P'); }
    void UpdatePixelShaderConstants() { UpdateFlags &= ~2ul; Log('Q'); }
    void UpdateAmbient() { UpdateFlags &= ~4ul; Log('A'); }
    void UpdateFogColour() { UpdateFlags &= ~0x20000ul; Log('F'); }
    void UpdateLightGlobals() { UpdateFlags &= ~0x400ul; Log('L'); }
    void UpdateCombinedProjectionTransform() { UpdateFlags &= ~0x80ul; Log('C'); }
    void UpdateCameraPos() { UpdateFlags &= ~8ul; Log('K'); }
};
static CShaderRenderManager* g_pShaderRenderManager;

static unsigned long g_draw[4]; static int g_drawKind;
class CRenderManagerCore {
public:
    void DrawCurrentBuffersAsIndexedTris(unsigned long a, unsigned long b, unsigned long c, unsigned long d)
    { g_draw[0] = a; g_draw[1] = b; g_draw[2] = c; g_draw[3] = d; g_drawKind = 1; Log('D'); }
    void DrawCurrentBuffersAsPointList(long a, long b) { g_draw[0] = a; g_draw[1] = b; g_drawKind = 2; Log('D'); }
};
static CRenderManagerCore* g_pRenderManagerCore;

struct CEngineWeatherSettingsView {
    unsigned char _pad00[0x34];
    float RainVolumeDepthSelectMultiplier;
    unsigned char _pad38[0xc];
    float SnowWeighting;
    unsigned char _pad48[0x46c];
    float CurrentRainStrength;
    float CurrentSnowStrength;
};
class CEngineWeatherRenderer { public:
    CEngineWeatherSettingsView S;
    CEngineWeatherSettingsView& GetSettings() { return S; }
    void RenderRainBox(const C3DVector& boxPos)
    {
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, boxPos.X, boxPos.Y, boxPos.Z,
                                                            GetSettings().RainVolumeDepthSelectMultiplier);
        g_pShaderRenderManager->PrepareForRender(5);
        g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, (long)(GetSettings().CurrentRainStrength * 512.0f), 0,
                                                              (long)(GetSettings().CurrentRainStrength * 256.0f));
    }
    void RenderSnowBox(const C3DVector& boxPos)
    {
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, boxPos.X, boxPos.Y, boxPos.Z, GetSettings().SnowWeighting);
        g_pShaderRenderManager->PrepareForRender(5);
        g_pRenderManagerCore->DrawCurrentBuffersAsPointList(0, (long)(GetSettings().CurrentSnowStrength * 1024.0f));
    }
};

static C4DVector g_presets[3];
static CVertexShaderConstantLayout g_layout;
static CShaderRenderManager g_srm;
static CRenderManagerCore g_core;
static CEngineWeatherRenderer g_weather;
static void Setup(unsigned long flags, int presets)
{
    g_log[0] = 0;
    g_layout.Reserved.Offset = 17;
    g_layout.ReservedPresets.First = g_presets;
    g_layout.ReservedPresets.Last = g_presets + presets;
    g_layout.ReservedPresets.End = g_presets + 3;
    g_srm.CurrentVSConstantLayout = &g_layout;
    g_srm.WorldMatrixIsIdentity = false;
    g_srm.UpdateFlags = flags;
    g_pShaderRenderManager = &g_srm;
    g_pRenderManagerCore = &g_core;
    std::memset(&g_weather, 0, sizeof(g_weather));
    g_weather.S.RainVolumeDepthSelectMultiplier = 3.5f;
    g_weather.S.SnowWeighting = 0.25f;
    g_weather.S.CurrentRainStrength = 0.5f;
    g_weather.S.CurrentSnowStrength = 0.75f;
}

int main()
{
    C3DVector pos = { -1.0f, 4.0f, 9.0f };
    Setup(0x88, 1);
    g_weather.RenderSnowBox(pos);
    if (std::strcmp(g_log, "UCKD") != 0 || g_cx != -1.0f || g_cy != 4.0f || g_cz != 9.0f || g_cw != 0.25f ||
        g_drawKind != 2 || g_draw[0] != 0 || g_draw[1] != 768) {
        std::printf("00b54210_TEST FAIL\n"); return 1;
    }
    Setup(0x20403, 3);
    g_weather.RenderSnowBox(pos);
    if (std::strcmp(g_log, "UPQFLD") != 0) { std::printf("00b54210_TEST FAIL\n"); return 1; }
    std::printf("00b54210_TEST PASS\n");
    return 0;
}