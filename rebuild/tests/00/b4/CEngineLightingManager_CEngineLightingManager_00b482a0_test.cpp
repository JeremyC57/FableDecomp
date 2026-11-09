// Behaviour test for CEngineLightingManager::CEngineLightingManager 0x00B482A0: every grid cleared, the empty light array has no
// lights, shadow mode 1, light cache and local/shadowed-spot lights on, the SIMD block allocated 0x110 bytes 16-aligned, the
// sun/moon/diffuse vectors set, exactly one theme channel (grey diffuse/ambient, black backlight/fog, fog 1000..2000), the mist
// colour black opaque, the manager registered once, the spot-light params inactive and all four state-update bits set.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>

struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    void Initialise(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};
struct CThemeChannel {
    CRGBFloatColour DiffuseColour, AmbientColour, BacklightColour, ReflectionColour, FogColour;
    float FogStartZ, FogEndZ, ShadowFactor, FadedShadowFactor;
    float FogTransform[4];
};
struct CShaderLightAttenuation {
    float Attenuation0, Attenuation1, Attenuation2, AttenuationBase;
    CShaderLightAttenuation() : Attenuation0(0.0f), Attenuation1(0.0f), Attenuation2(0.0f), AttenuationBase(0.0f) { }
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEngineLocalLightArray { unsigned long LightCount; unsigned char Lights[0x1a0]; CEngineLocalLightArray() : LightCount(0) { } };
struct CSpotlightParams { bool Active; };

static unsigned long gAllocSize = 0; static long gAllocAlign = 0; static int gRegistered = 0;
static void* AllocateAligned(unsigned long size, long alignment) { gAllocSize = size; gAllocAlign = alignment; return std::malloc(size); }
struct CEngineLightingManager;
static void RegisterPrimitiveManager(CEngineLightingManager*) { ++gRegistered; }

struct CEngineLightingManager {
    C3DVector SunVector, MoonVector, DiffuseVector;
    std::vector<CThemeChannel> ThemeChannels;
    CShaderLightAttenuation LightAttenuation;
    long ShaderPointLightsEnabledCount;
    bool DrawLightRadius, DrawSpotLightCone, EnableLightCache;
    std::vector<void*> LocalLightList, ShadowedSpotLightList;
    bool LocalLightsEnabled, ShadowedSpotLightsEnabled, FadedShadowDiffuseFactorEnabled;
    CRGBFloatColour MistEffectColour;
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;
    unsigned long StateUpdateFlags;
    long SelfIlluminationAmbientPercentage;
    void* LightLookupGrid[16][16];
    CEngineLocalLightArray* SmallObjectLookupGrid[64][64];
    CEngineLocalLightArray EmptyLightArray;
    void* SIMDVarBlock;
    long ShadowMode;
    CSpotlightParams CurrentShadowedSpotLightParams;
    CEngineLightingManager()
    {
        std::memset(LightLookupGrid, 0, sizeof(LightLookupGrid));
        std::memset(SmallObjectLookupGrid, 0, sizeof(SmallObjectLookupGrid));
        ShadowMode = 1;
        DrawLightRadius = false;
        DrawSpotLightCone = false;
        EnableLightCache = true;
        ShaderPointLightsEnabledCount = 0;
        LocalLightsEnabled = true;
        ShadowedSpotLightsEnabled = true;
        FadedShadowDiffuseFactorEnabled = false;
        SelfIlluminationAmbientPercentage = 0;
        SIMDVarBlock = AllocateAligned(0x110, 16);
        SunVector = C3DVector(0.0f, 1.0f, 0.0f);
        MoonVector = C3DVector(0.0f, 0.0f, -1.0f);
        DiffuseVector = C3DVector(0.0f, 1.0f, 0.0f);
        ThemeChannels.resize(1, CThemeChannel());
        ThemeChannels[0].DiffuseColour.Initialise(0.25f, 0.25f, 0.25f, 1.0f);
        ThemeChannels[0].AmbientColour.Initialise(0.25f, 0.25f, 0.25f, 1.0f);
        ThemeChannels[0].BacklightColour.Initialise(0.0f, 0.0f, 0.0f, 1.0f);
        ThemeChannels[0].FogColour.Initialise(0.0f, 0.0f, 0.0f, 1.0f);
        ThemeChannels[0].FogStartZ = 1000.0f;
        ThemeChannels[0].FogEndZ = 2000.0f;
        MistEffectColour.Initialise(0.0f, 0.0f, 0.0f, 1.0f);
        RegisterPrimitiveManager(this);
        CurrentShadowedSpotLightParams.Active = false;
        StateUpdateFlags = 0xf;
    }
};

int main()
{
    bool ok = true;
    CEngineLightingManager* m = new CEngineLightingManager;
    for (int i = 0; i < 16; ++i) for (int j = 0; j < 16; ++j) if (m->LightLookupGrid[i][j]) ok = false;
    for (int i = 0; i < 64; ++i) for (int j = 0; j < 64; ++j) if (m->SmallObjectLookupGrid[i][j]) ok = false;
    if (m->EmptyLightArray.LightCount != 0 || m->ShadowMode != 1) ok = false;
    if (m->DrawLightRadius || m->DrawSpotLightCone || !m->EnableLightCache || m->ShaderPointLightsEnabledCount != 0) ok = false;
    if (!m->LocalLightsEnabled || !m->ShadowedSpotLightsEnabled || m->FadedShadowDiffuseFactorEnabled) ok = false;
    if (m->SelfIlluminationAmbientPercentage != 0 || !m->SIMDVarBlock || gAllocSize != 0x110 || gAllocAlign != 16) ok = false;
    if (m->SunVector.Y != 1.0f || m->SunVector.X != 0.0f || m->MoonVector.Z != -1.0f || m->DiffuseVector.Y != 1.0f) ok = false;
    if (m->ThemeChannels.size() != 1) ok = false;
    const CThemeChannel& t = m->ThemeChannels[0];
    if (t.DiffuseColour.R != 0.25f || t.AmbientColour.B != 0.25f || t.DiffuseColour.A != 1.0f) ok = false;
    if (t.BacklightColour.R != 0.0f || t.FogColour.A != 1.0f || t.FogStartZ != 1000.0f || t.FogEndZ != 2000.0f) ok = false;
    if (m->MistEffectColour.G != 0.0f || m->MistEffectColour.A != 1.0f) ok = false;
    if (m->LightAttenuation.Attenuation0 != 0.0f || m->LightAttenuation.AttenuationBase != 0.0f) ok = false;
    if (m->CurrentThemeChannel.ChannelIndex0 || m->CurrentThemeChannel.ChannelBlendFactor) ok = false;
    if (!m->LocalLightList.empty() || !m->ShadowedSpotLightList.empty()) ok = false;
    if (gRegistered != 1 || m->CurrentShadowedSpotLightParams.Active || m->StateUpdateFlags != 0xf) ok = false;
    std::free(m->SIMDVarBlock);
    delete m;
    if (!ok) { std::printf("00b482a0_TEST FAIL\n"); return 1; }
    std::printf("00b482a0_TEST PASS\n");
    return 0;
}