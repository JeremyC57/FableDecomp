// Leaf: ?GetLightsInSphereFast@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z
// Flags: none
#define FABLE_RETAIL_GET_OPINIONS_LIGHT_CACHE_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#include "engine/C3DBoundingSphere.h"
#include "engine/CEngineLightingManager.h"

CEngineLocalLightArray* CEngineLightingManager::GetLightsInSphereFast(const C3DBoundingSphere& sphere, long layerMask,
                                                                      const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (sphere.Radius + sphere.Radius <= 2.0f && theme.ChannelIndex0 == 0
        && theme.ChannelBlendFactor == 0 && EnableLightCache) {
        CEngineLocalLightArray* lights =
            LookupSmallObjectLightCache(sphere.Center.X - sphere.Radius, sphere.Center.Y - sphere.Radius, layerMask);
        if (lights)
            return lights;
    }
    return GetLightsInSphere(sphere, layerMask, theme);
}
