// 00b47bc0 CEngineLightingManager::UpdateShadowedSpotLights
// Flags: /d2inlT39
// Leaf: ?UpdateShadowedSpotLights@CEngineLightingManager@@AAEXXZ
// Inline-budget: /d2inlT39 -- retains the real out-of-line element constructor and expands the original caller helpers; default expands the constructor (1098 B), T10..T28 differ, T29 gives the 1028-B leaf; measured T29..T32, T35..T40 and T45 all give it, and T39 (NF344, 2026-11-24) is the smallest of those that also gives the leaf-called _M_insert_overflow copy retail's body (0x00B49DF0; NF340 /d2inl audit).
// Retail: Fable.exe00b47bc0,1028bytes.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_CRGBFLOATCOLOUR_SCALE
#define FABLE_RETAIL_STLP_TAG_CTORS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
namespace NEngineInternal {extern CEngineCamera* GPCamera;}
CEngineLocalLightElement::CEngineLocalLightElement(const C3DVector& world_pos,float radius,float spot_radius,const CRGBFloatColour& colour,unsigned long channel_mask):WorldPos(world_pos),Radius(radius),RadiusSquared(radius*radius),SpotRadius(spot_radius),Colour(colour),ChannelMask(channel_mask)
{
 AttenuationDistSquared=1.0f/(SpotRadius*SpotRadius)-1.0f/(Radius*Radius);
 AttenuationConstant=(1.0f/(Radius*Radius)/AttenuationDistSquared)*-1.0f;
}

void CEngineLightingManager::UpdateShadowedSpotLights()
{
    if (!ShadowedSpotLightsEnabled) {
        CurrentShadowedSpotLightParams.Active = false;
        return;
    }
    long nearest_index = -1;
    float nearest_distance_squared = 1e30f;
    long second_nearest_index = -1;
    float second_nearest_distance_squared = 1e30f;
    C3DVector cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
    for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
        float distance_squared=(cam_pos-ShadowedSpotLightList[n]->Position).GetSquaredMagnitude();
        if (distance_squared < nearest_distance_squared) {
            nearest_distance_squared = distance_squared;
            nearest_index = n;
        }
    }
    for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
        float distance_squared=(cam_pos-ShadowedSpotLightList[n]->Position).GetSquaredMagnitude();
        if (n != nearest_index && distance_squared < second_nearest_distance_squared) {
            second_nearest_distance_squared = distance_squared;
            second_nearest_index = n;
        }
    }
    if (nearest_index >= 0) {
        CManagerShadowedSpotLight* nearest_light = ShadowedSpotLightList[nearest_index];
        CurrentShadowedSpotLightParams.Active = true;
        CurrentShadowedSpotLightParams.Colour = nearest_light->Colour;
        CurrentShadowedSpotLightParams.ConeWidth = nearest_light->ConeRadius * 2.0f;
        float inv_fade_alpha = 0.0f;
        CurrentShadowedSpotLightParams.Radius = nearest_light->Radius;
        CurrentShadowedSpotLightParams.SpotRadius = nearest_light->SpotRadius;
        CurrentShadowedSpotLightParams.Vector = nearest_light->Direction;
        CurrentShadowedSpotLightParams.Pos = nearest_light->Position;
        if (second_nearest_index >= 0) {
            CManagerShadowedSpotLight* second_light=ShadowedSpotLightList[second_nearest_index];
            float dist0=(cam_pos-nearest_light->Position).GetAccurateMagnitude();
            float dist1=(cam_pos-second_light->Position).GetAccurateMagnitude();
            float total=dist0+dist1;
            dist0/=total;dist1/=total;
            float faded_range=0.5f/2.0;
            inv_fade_alpha=(dist0-(0.5-faded_range))/faded_range;
            if (inv_fade_alpha < 0.0f)
                inv_fade_alpha = 0.0f;
            else if (inv_fade_alpha > 1.0f)
                inv_fade_alpha = 1.0f;
            float fade_alpha=1.0f-inv_fade_alpha;
            CurrentShadowedSpotLightParams.Colour*=fade_alpha;
        }
        for (long n = 0; n < ShadowedSpotLightList.size(); ++n) {
            float scale = (n == nearest_index) ? inv_fade_alpha : 1.0f;
            CManagerShadowedSpotLight* spot_light = ShadowedSpotLightList[n];
            CManagerLocalLight* local_light=new CManagerLocalLight(spot_light->Position,spot_light->Radius,spot_light->SpotRadius,spot_light->Colour*scale,0xFFFFFFFF);
            LocalLightList.push_back(local_light);
        }
    } else
        CurrentShadowedSpotLightParams.Active = false;
}