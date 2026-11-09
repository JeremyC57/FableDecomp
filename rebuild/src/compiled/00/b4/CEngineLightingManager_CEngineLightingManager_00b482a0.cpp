// CEngineLightingManager::CEngineLightingManager() -- builds the lighting manager: primitive-manager and engine-component bases, every list and grid empty, light cache and local/shadowed-spot lights enabled, the SIMD lighting block allocated (0x110 bytes, 16-aligned), sun/moon/diffuse vectors, one default theme channel (grey diffuse/ambient, black backlight/fog, fog 1000..2000), then registers itself with the primitive type info
// Leaf: ??0CEngineLightingManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B482A0, length 528 bytes, plain `ret`. Manifest label
//   "CEnginePrimitiveManager::Initialise" is wrong: the body is the lighting manager's constructor (vtables 0x012A2274 / 0x012A223C).
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>&) @ 0x00BACDB0 (the renderer's SceneList +0x338),
//   CEngineComponent::CEngineComponent @ 0x00B59710, CMemoryAllocator::AllocateAligned(ulong, long) @ 0x009F2BE0 (static __fastcall),
//   vector<CEngineLightingManager::CThemeChannel>::resize(size_type, const T&) @ 0x00B4A4C0 (landed),
//   CEnginePrimitiveTypeInfo::RegisterPrimitiveManager @ 0x00B8FAD0.
// Globals: the renderer component @ 0x01436E80, the primitive type info @ 0x01436E84 (landed spellings; no PDB data symbols).
// Shape: the members are built by their own inline constructors in declaration order (the STLport vector constructors with
//   their allocator temporaries, CShaderLightAttenuation's zeroed floats, the theme-channel desc, CEngineLocalLightArray's
//   LightCount); the body then clears both grids and sets the state in Ego_r.pdb member order.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SCENELIST
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_REGISTER
#define FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include <string.h>
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CMemoryAllocator.h"

extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;    // 0x01436E80
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;    // 0x01436E84

CEngineLightingManager::CEngineLightingManager()
    : CEnginePrimitiveManager(g_pEnginePrimitiveRenderer->SceneList)
{
    memset(LightLookupGrid, 0, sizeof(LightLookupGrid));
    memset(SmallObjectLookupGrid, 0, sizeof(SmallObjectLookupGrid));
    ShadowMode = (ELightingManagerShadowMode)1;
    DrawLightRadius = false;
    DrawSpotLightCone = false;
    EnableLightCache = true;
    ShaderPointLightsEnabledCount = 0;
    LocalLightsEnabled = true;
    ShadowedSpotLightsEnabled = true;
    FadedShadowDiffuseFactorEnabled = false;
    SelfIlluminationAmbientPercentage = 0;
    SIMDVarBlock = (CLightingSIMDVarBlock*)CMemoryAllocator::AllocateAligned(0x110, 16);
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
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(this);
    CurrentShadowedSpotLightParams.Active = false;
    StateUpdateFlags = 0xf;
}