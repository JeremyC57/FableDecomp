// CEngineLightingManager::UpdateGlobalLightingStates() -- refresh dirty global lighting, ambient and shadow factors.
// Leaf: ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ
// Retail: 0x00B46C80,1531 bytes; public thiscall.
// Flags: none
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_FLOAT_CTORS
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C4DVECTOR_ORIGINAL_COLOUR_CHAIN
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWMODE_ENUM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CEngineLightingManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CShaderDirectionalLight.h"
#include "engine/CLightingSIMDVarBlock.h"
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }
void CEngineLightingManager::UpdateGlobalLightingStates()
{
    float channel_blend_factor1=CurrentThemeChannel.ChannelBlendFactor/255.0f;
    if(StateUpdateFlags&1) {
        if(CurrentThemeChannel.ChannelBlendFactor>0) {
            PrimitiveDiffuseColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].DiffuseColour,channel_blend_factor1);
            PrimitiveBacklightColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].BacklightColour,channel_blend_factor1);
        } else {
            PrimitiveDiffuseColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour;
            PrimitiveBacklightColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour;
        }
        if(FadedShadowDiffuseFactorEnabled) {
            float shadow_factor;
            if(CurrentThemeChannel.ChannelBlendFactor>0)
                shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor;
            else
                shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
            PrimitiveDiffuseColour=PrimitiveDiffuseColour*shadow_factor;
        }
        CShaderDirectionalLight sunlight(DiffuseVector,PrimitiveDiffuseColour,PrimitiveBacklightColour);
        NEngineInternal::GPShaderRenderManager->InitialiseLight(0,sunlight);
        NEngineInternal::GPShaderRenderManager->SetLightGlobal(0,C4DVector(PrimitiveBacklightColour));
        StateUpdateFlags&=~1;
        for(long c=0;c<4;++c) {
            SIMDVarBlock->DiffuseColourR[c]=PrimitiveDiffuseColour.R;
            SIMDVarBlock->DiffuseColourG[c]=PrimitiveDiffuseColour.G;
            SIMDVarBlock->DiffuseColourB[c]=PrimitiveDiffuseColour.B;
        }
    }
    if(StateUpdateFlags&2) {
        if(CurrentThemeChannel.ChannelBlendFactor>0)
            PrimitiveAmbientColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour.Interpolate(ThemeChannels[CurrentThemeChannel.ChannelIndex1].AmbientColour,channel_blend_factor1);
        else
            PrimitiveAmbientColour=ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour;
        if(SelfIlluminationAmbientPercentage>0) {
            float sqr_illum=GFSqr(SelfIlluminationAmbientPercentage*0.01f);
            CRGBFloatColour ambient(GFRoot(GFSqr(PrimitiveAmbientColour.GetR())+sqr_illum),GFRoot(GFSqr(PrimitiveAmbientColour.GetG())+sqr_illum),GFRoot(GFSqr(PrimitiveAmbientColour.GetB())+sqr_illum));
            if(ambient.R>1.0f)ambient.R=1.0f;
            if(ambient.G>1.0f)ambient.G=1.0f;
            if(ambient.B>1.0f)ambient.B=1.0f;
            PrimitiveAmbientColour=ambient;
        }
        NEngineInternal::GPShaderRenderManager->SetAmbientColour(PrimitiveAmbientColour);
        StateUpdateFlags&=~2;
    }
    if(ShadowMode==LIGHTING_MANAGER_SHADOW_MODE_MAIN_LIGHT&&(StateUpdateFlags&8)&&!FadedShadowDiffuseFactorEnabled) {
        float shadow_factor,faded_shadow_factor;
        if(CurrentThemeChannel.ChannelBlendFactor>0) {
            shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].ShadowFactor;
            faded_shadow_factor=(1.0f-channel_blend_factor1)*ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor+channel_blend_factor1*ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor;
        } else {
            shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor;
            faded_shadow_factor=ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
        }
        shadow_factor=1.0f-shadow_factor;
        float std_light_factor=1.0f-faded_shadow_factor-shadow_factor;
        float std_light_offset=faded_shadow_factor;
        NEngineInternal::GPShaderRenderManager->SetShadowFadeFactors(shadow_factor,std_light_factor,std_light_offset);
    }
}