// 00b484b0 CEngineLightingManager::FrameStart -- update lighting from the draw environment and camera.
// Leaf: ?FrameStart@CEngineLightingManager@@UAEXABVCEngineComponentParams@@@Z
// Retail: 0x00B484B0,1177 bytes; secondary CEngineComponent base at+0C.
// Flags: none
// Retail-asm: original GFFloatToLongFloor at 0x00B4883B..0x00B48855 and 0x00B4888F..0x00B488A9 (`fistp dword ptr [ebp-0Ch]` at 0x00B48841 / 0x00B48895).
// Thirty plain-C++ and ten intrinsic lowering probes at both sites produced no dword integer store; see FLOOR_LADDER_RESULTS.json.
// Original PDB module983 proves comp_params, params(const CIDrawEnvironmentParams&), channel(long).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FOGTRANSFORM
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUS
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_STLP_TAG_CTORS
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEngineScene.h"
#include "engine/GFFloatToLongFloor.h"
namespace NEngineInternal {
 extern CEngineWorldMap* GPEngineWorldMap; // 01436E8C; donor04AC806C exact typed PUBLIC.
 extern CEngineCamera* GPCamera; // 01436EA0; donor04AC8084.
 extern CEngineShadowRenderer* GPShadowRenderer; // 01436E60; donor04AC8040.
}
inline void CEngineLightingManager::UpdateShadowScene(const CEngineCamera& camera)
{
 if(ShadowMode==(ELightingManagerShadowMode)2)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,true,CurrentShadowedSpotLightParams);
 else if(ShadowMode==(ELightingManagerShadowMode)1)
  NEngineInternal::GPShadowRenderer->UpdateShadowScene(camera,false,CurrentShadowedSpotLightParams);
}

// Original donor02CF2120: named zscale,zofs,fog_transform; matrix constructor and PreMultiply.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
C4DVector CEngineLightingManager::CalculateFogTransformMatrix(float fog_start,float fog_end)
{
 float zscale=1.0f/(fog_end-fog_start);
 float zofs=-zscale*fog_start;
 CMatrix3x4 fog_transform(0,0,0,0,0,0,zscale,0,0,zofs,0,0);
 fog_transform.PreMultiply(NEngineInternal::GPCamera->PeekNonHomogenousViewTransform());
 return C4DVector(fog_transform.E11,fog_transform.E21,fog_transform.E31,fog_transform.E41);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void CEngineLightingManager::FrameStart(const CEngineComponentParams& comp_params)
{
    if (NEngineInternal::GPEngineWorldMap) {
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        SunVector = params.SunVector;
        MoonVector = params.MoonVector;
        DiffuseVector = params.DiffuseVector;
        DiffuseVector.Normalise();
        ThemeChannels.resize(params.ThemeChannels.size());
        for (long channel = 0; channel < ThemeChannels.size(); ++channel) {
            ThemeChannels[channel].DiffuseColour = params.ThemeChannels[channel].Colours[0];
            ThemeChannels[channel].AmbientColour = params.ThemeChannels[channel].Colours[1];
            ThemeChannels[channel].BacklightColour = params.ThemeChannels[channel].Colours[2];
            ThemeChannels[channel].ReflectionColour = params.ThemeChannels[channel].Colours[3];
            ThemeChannels[channel].FogStartZ = params.ThemeChannels[channel].Scalers[6];
            ThemeChannels[channel].FogEndZ = params.ThemeChannels[channel].Scalers[7];
            ThemeChannels[channel].FogColour = params.ThemeChannels[channel].Colours[5];
            ThemeChannels[channel].ShadowFactor = params.ThemeChannels[channel].Scalers[21];
            ThemeChannels[channel].FadedShadowFactor = params.ThemeChannels[channel].Scalers[22];
            ThemeChannels[channel].FogTransform = CalculateFogTransformMatrix(ThemeChannels[channel].FogStartZ, ThemeChannels[channel].FogEndZ);
        }
        MistEffectColour = params.ThemeChannels[0].Colours[4];
    }
    PrimitiveDiffuseColour = ThemeChannels[0].DiffuseColour;
    PrimitiveAmbientColour = ThemeChannels[0].AmbientColour;
    PrimitiveBacklightColour = ThemeChannels[0].BacklightColour;
    FadedShadowDiffuseFactorEnabled = false;
    SelfIlluminationAmbientPercentage = 0;
    CurrentThemeChannel.ChannelIndex0 = 0;
    CurrentThemeChannel.ChannelIndex1 = 0;
    CurrentThemeChannel.ChannelBlendFactor = 0;
    StateUpdateFlags = 0xf;
    LookupGridWorldX = (float)(GFFloatToLongFloor((NEngineInternal::GPCamera->PeekCameraPos().X - 128.0f) * 0.0625f) << 4);
    LookupGridWorldY = (float)(GFFloatToLongFloor((NEngineInternal::GPCamera->PeekCameraPos().Y - 128.0f) * 0.0625f) << 4);
    (*SceneList)[0]->RenderEntireScene((EEngineRenderLayer)1);
    UpdateShadowedSpotLights();
    AddLightsToLookupGrid();
    CurrentShaderLocalLights = 0;
    UpdateSIMDVarBlock();
    if (comp_params.SceneFilterFlags & 1)
        UpdateShadowScene(*NEngineInternal::GPCamera);
}