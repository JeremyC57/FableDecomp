// CEngineLightingManager::FrameStart(const CEngineComponentParams& params) -- per frame: pull the sun/moon/diffuse vectors and every theme channel's colours, fog range, shadow factors and fog transform from the environment state; seed the primitive colours from channel 0, reset the per-frame state, recentre the 16x16 light lookup grid on the camera, render the scene's always-visible layer, rebuild the shadowed spot light and the lookup grid, and update the shadow scene when the scene filter asks for it
// Grade: DIFFER(1177v1177) -- one instruction placed differently: retail pushes the resize() default-channel temporary's address ([ebp-94h]) after the normalised DiffuseVector stores, ours before them (13 probes; levers that closed the rest are listed below).
// Leaf: ?FrameStart@CEngineLightingManager@@UAEXABVCEngineComponentParams@@@Z
// Flags: none
// Retail-asm: 0x00B4883B..0x00B48855 and 0x00B4888F..0x00B488A9 -- two expansions of the inline-asm floor helper GFFloatToLongFloor (bare dword fistp; standalone 0x0048D940).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B484B0, 1177 bytes (the 1840-byte gap holds the next functions: `ret 4` at 0x00B48946, int3 padding from 0x00B48949), ebp frame 0x94; `this` is the CEngineComponent subobject at +0x0C.
// Name: Ego_r.pdb + FableWin.pdb ?FrameStart@CEngineLightingManager@@UAEXABVCEngineComponentParams@@@Z (CEngineComponent::FrameStart override).
// Levers: the same-TU body of CalculateFogTransformMatrix (0x00B451D0) -- once VC7.1 sees that callee leaves edx alone it keeps
//   &ThemeChannels[i] in edx across the call as retail does (a body that touches edx, or none, spills it); UpdateShadowScene
//   (0x00B444F0, PDB QAEXABVCEngineCamera@@@Z) as the inline member call (retail addresses ShadowMode through the adjusted this
//   in esi); the fast normalise reads X, Y, Z into locals first (load order); FABLE_RETAIL_STLP_TAG_CTORS overlay.
// Callees: CEngineScene::RenderEntireScene 0x00B943F0, vector<CThemeChannel>::resize 0x00B4A4C0, UpdateShadowedSpotLights 0x00B47BC0,
//   AddLightsToLookupGrid 0x00B46660, UpdateSIMDVarBlock 0x00B44540, CEngineShadowRenderer::UpdateShadowScene 0x00B4F390.
// Globals: 0x01436E8C (environment owner; reloc_bindings carries six conflicting spellings), g_pEngineCamera 0x01436EA0,
//   g_pShadowRenderer 0x01436E60, GFastOneOverSqrtTable 0x013CE558; 128.0f 0x012571B0, 0.0625f 0x0122DC98, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: modelled locally (the generated CEngineLightingManager.h / CEngineComponent.h are flat and cannot express the two
//   polymorphic bases, as in landed FrameEnd 0x00B47980); the environment theme data is FableWin CThemeChannel (Colours[19],
//   Scalers[30]; retail stride 0x1F0).
#define FABLE_RETAIL_STLP_TAG_CTORS
#include "engine/C3DVector.h"
#include "engine/CArray.h"

class CRGBFloatColour { public: float R, G, B, A; };
class C4DVector { public: float X, Y, Z, W; C4DVector() { } C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { } };
class CEngineCameraDesc { public: float Window[4]; float RotationMatrix[12]; C3DVector WorldPosition; float HorizontalFOV; float VerticalFOV; bool Use2DFOV; float NearZPlane; float FarZPlane; };
class CEngineCamera { public: unsigned char _pad[0x14]; CEngineCameraDesc CameraDesc; unsigned char _pad80[0x114 - 0x80]; float InverseViewTransform[12]; };
extern CEngineCamera* g_pEngineCamera;                     // 0x01436EA0
class CEngineComponentParams { public:
    float FrameInterpolation;
    unsigned char InterpolationInfo[0x70];
    unsigned long FrameID;
    unsigned long SceneFilterFlags;                        // +0x78
    bool OffWorldTickProcessFlag;
};
struct CEngineLightingManagerSpotlightParams;
class CEngineShadowRenderer { public:
    void UpdateShadowScene(const CEngineCamera& camera, bool spotLight, const CEngineLightingManagerSpotlightParams& params);
};
extern CEngineShadowRenderer* g_pShadowRenderer;           // 0x01436E60
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_0, ENGINE_RENDER_LAYER_1 };
class CEngineScene { public: void RenderEntireScene(EEngineRenderLayer layer); };

// the environment's theme data (FableWin CThemeChannel of the environment: Colours[19], Scalers[30]; retail stride 0x1F0)
class CEnvironmentThemeChannel { public:
    CRGBFloatColour Colours[19];
    float Scalers[30];
    unsigned char TextureSets[0x1f0 - 0x1a8];
};
class CEnvironmentState { public:
    unsigned char _pad[0x24];
    C3DVector SunVector;                                   // +0x24
    C3DVector MoonVector;                                  // +0x30
    C3DVector DiffuseVector;                               // +0x3c
    unsigned char _pad48[0x54 - 0x48];
    CArray<CEnvironmentThemeChannel> ThemeChannels;        // +0x54
};
class CEnvironmentSource { public: virtual void v0(); virtual CEnvironmentState* GetState(); };
class CEnvironmentHolder { public: virtual void v0(); virtual CEnvironmentSource* GetSource(); };
class CEngineEnvironmentOwner { public: unsigned char _pad[0x198]; CEnvironmentHolder* Environment; };
extern CEngineEnvironmentOwner* g_pEngineEnvironment;      // 0x01436E8C

extern unsigned long GFastOneOverSqrtTable[128];           // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = ((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000 | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

__forceinline void FastNormalise(C3DVector& v)
{
    float x = v.X, y = v.Y, z = v.Z;
    float scale = FastInvSqrt(x * x + y * y + z * z);
    v.X *= scale;
    v.Y *= scale;
    v.Z *= scale;
}

__forceinline long GFFloatToLongFloor(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

class CEnginePrimitiveManager { public:
    virtual void v0();
    CArray<CEngineScene*>* SceneList;                      // +0x04
    bool Enable;                                           // +0x08
};
class CEngineComponent { public:
    virtual void v0();
    virtual void FrameStart(const CEngineComponentParams& params);
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
};
class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {
public:
    struct CThemeChannel {
        CRGBFloatColour DiffuseColour;
        CRGBFloatColour AmbientColour;
        CRGBFloatColour BacklightColour;
        CRGBFloatColour ReflectionColour;
        CRGBFloatColour FogColour;
        float FogStartZ;
        float FogEndZ;
        float ShadowFactor;
        float FadedShadowFactor;
        C4DVector FogTransform;
        CThemeChannel() { }
    };
    C3DVector SunVector;                                   // +0x18
    C3DVector MoonVector;                                  // +0x24
    C3DVector DiffuseVector;                               // +0x30
    CArray<CThemeChannel> ThemeChannels;                   // +0x3c
    CRGBFloatColour PrimitiveDiffuseColour;                // +0x48
    CRGBFloatColour PrimitiveAmbientColour;                // +0x58
    CRGBFloatColour PrimitiveBacklightColour;              // +0x68
    unsigned char _pad78[0xa8 - 0x78];
    void* CurrentShaderLocalLights;                        // +0xa8
    unsigned char _padac[0xc4 - 0xac];
    float LookupGridWorldX;                                // +0xc4
    float LookupGridWorldY;                                // +0xc8
    bool LocalLightsEnabled;                               // +0xcc
    bool ShadowedSpotLightsEnabled;                        // +0xcd
    bool FadedShadowDiffuseFactorEnabled;                  // +0xce
    CRGBFloatColour MistEffectColour;                      // +0xd0
    unsigned char CurrentThemeChannel_ChannelIndex0;       // +0xe0
    unsigned char CurrentThemeChannel_ChannelIndex1;
    unsigned char CurrentThemeChannel_ChannelBlendFactor;
    unsigned char CurrentThemeChannel_Pad;
    unsigned long StateUpdateFlags;                        // +0xe4
    long SelfIlluminationAmbientPercentage;                // +0xe8
    unsigned char _padec[0x4694 - 0xec];
    long ShadowMode;                                       // +0x4694
    unsigned char CurrentShadowedSpotLightParams[0x38];    // +0x4698
    virtual void FrameStart(const CEngineComponentParams& params);
    void UpdateShadowScene(const CEngineCamera& camera)
    {
        if (ShadowMode == 2)
            g_pShadowRenderer->UpdateShadowScene(camera, true, (const CEngineLightingManagerSpotlightParams&)CurrentShadowedSpotLightParams);
        else if (ShadowMode == 1)
            g_pShadowRenderer->UpdateShadowScene(camera, false, (const CEngineLightingManagerSpotlightParams&)CurrentShadowedSpotLightParams);
    }
private:
    C4DVector CalculateFogTransformMatrix(float startZ, float endZ);
    void UpdateShadowedSpotLights();
    void AddLightsToLookupGrid();
    void UpdateSIMDVarBlock();
};

#pragma auto_inline(off)
// same-TU body of the fog transform (0x00B451D0): the fog matrix (Z scaled into [0,1] over startZ..endZ) composed with the
// camera's inverse view transform, first column out
class CMatrix3x4 { public: float M[12]; };
C4DVector CEngineLightingManager::CalculateFogTransformMatrix(float startZ, float endZ)
{
    CMatrix3x4 fog;
    float scale = 1.0f / (endZ - startZ);
    fog.M[0] = 0.0f; fog.M[1] = 0.0f; fog.M[2] = 0.0f; fog.M[3] = 0.0f; fog.M[4] = 0.0f; fog.M[5] = 0.0f;
    fog.M[6] = scale; fog.M[7] = 0.0f; fog.M[8] = 0.0f; fog.M[10] = 0.0f; fog.M[11] = 0.0f;
    fog.M[9] = -(scale * startZ);
    CMatrix3x4 m = fog;
    const float* view = g_pEngineCamera->InverseViewTransform;
    return C4DVector(m.M[3] * view[1] + m.M[6] * view[2] + m.M[0] * view[0],
                     m.M[3] * view[4] + m.M[6] * view[5] + m.M[0] * view[3],
                     m.M[3] * view[7] + m.M[6] * view[8] + m.M[0] * view[6],
                     m.M[3] * view[10] + m.M[6] * view[11] + m.M[0] * view[9] + m.M[9]);
}
#pragma auto_inline(on)

void CEngineLightingManager::FrameStart(const CEngineComponentParams& params)
{
    if (g_pEngineEnvironment) {
        CEnvironmentState* env = g_pEngineEnvironment->Environment->GetSource()->GetState();
        SunVector = env->SunVector;
        MoonVector = env->MoonVector;
        DiffuseVector = env->DiffuseVector;
        FastNormalise(DiffuseVector);
        ThemeChannels.resize(env->ThemeChannels.size(), CThemeChannel());
        for (unsigned long i = 0; i < ThemeChannels.size(); ++i) {
            ThemeChannels[i].DiffuseColour = env->ThemeChannels[i].Colours[0];
            ThemeChannels[i].AmbientColour = env->ThemeChannels[i].Colours[1];
            ThemeChannels[i].BacklightColour = env->ThemeChannels[i].Colours[2];
            ThemeChannels[i].ReflectionColour = env->ThemeChannels[i].Colours[3];
            ThemeChannels[i].FogStartZ = env->ThemeChannels[i].Scalers[6];
            ThemeChannels[i].FogEndZ = env->ThemeChannels[i].Scalers[7];
            ThemeChannels[i].FogColour = env->ThemeChannels[i].Colours[5];
            ThemeChannels[i].ShadowFactor = env->ThemeChannels[i].Scalers[21];
            ThemeChannels[i].FadedShadowFactor = env->ThemeChannels[i].Scalers[22];
            ThemeChannels[i].FogTransform = CalculateFogTransformMatrix(ThemeChannels[i].FogStartZ, ThemeChannels[i].FogEndZ);
        }
        MistEffectColour = env->ThemeChannels[0].Colours[4];
    }
    PrimitiveDiffuseColour = ThemeChannels[0].DiffuseColour;
    PrimitiveAmbientColour = ThemeChannels[0].AmbientColour;
    PrimitiveBacklightColour = ThemeChannels[0].BacklightColour;
    FadedShadowDiffuseFactorEnabled = false;
    SelfIlluminationAmbientPercentage = 0;
    CurrentThemeChannel_ChannelIndex0 = 0;
    CurrentThemeChannel_ChannelIndex1 = 0;
    CurrentThemeChannel_ChannelBlendFactor = 0;
    StateUpdateFlags = 0xf;
    LookupGridWorldX = (float)(GFFloatToLongFloor((g_pEngineCamera->CameraDesc.WorldPosition.X - 128.0f) * 0.0625f) << 4);
    LookupGridWorldY = (float)(GFFloatToLongFloor((g_pEngineCamera->CameraDesc.WorldPosition.Y - 128.0f) * 0.0625f) << 4);
    (*SceneList)[0]->RenderEntireScene(ENGINE_RENDER_LAYER_1);
    UpdateShadowedSpotLights();
    AddLightsToLookupGrid();
    CurrentShaderLocalLights = 0;
    UpdateSIMDVarBlock();
    if (params.SceneFilterFlags & 1)
        UpdateShadowScene(*g_pEngineCamera);
}
