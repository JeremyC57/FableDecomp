#ifdef FABLE_LIGHT_OPINION_LINK
// Leaf: ?GetLightsInSphereFast@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z
// Flags: none
#define FABLE_RETAIL_GET_OPINIONS_LIGHT_CACHE_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#include "engine/C3DBoundingSphere.h"
#include "engine/CEngineLightingManager.h"

#else
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

#endif
#include <stdio.h>
static unsigned long managerData[512], cacheTag, fullTag;
static long layer, cacheCalls, fullCalls;
static float minX, minY;
static bool cacheHit;
static const C3DBoundingSphere* expectedSphere;
static const CEnginePrimitiveThemeChannelDesc* expectedTheme;
static int issues;
CEngineLocalLightArray* CEngineLightingManager::LookupSmallObjectLightCache(float x,float y,long m)
{
    if(m!=layer || x!=minX || y!=minY) ++issues;
    ++cacheCalls;
    return cacheHit ? reinterpret_cast<CEngineLocalLightArray*>(&cacheTag) : 0;
}
CEngineLocalLightArray* CEngineLightingManager::GetLightsInSphere(const C3DBoundingSphere& s,long m,
                                                                const CEnginePrimitiveThemeChannelDesc& t)
{
    if(m!=layer || &s!=expectedSphere || &t!=expectedTheme) ++issues;
    ++fullCalls;
    return reinterpret_cast<CEngineLocalLightArray*>(&fullTag);
}
int main()
{
    C3DBoundingSphere sphere;
    CEnginePrimitiveThemeChannelDesc theme;
    CEngineLightingManager* light=reinterpret_cast<CEngineLightingManager*>(managerData);
    expectedSphere=&sphere; expectedTheme=&theme;
    int cases=0;
    for (int config=0;config<32;++config) {
        for(int r=0;r<5;++r) {
            sphere.Center.X=3; sphere.Center.Y=-4; sphere.Radius=0.5f+0.25f*r;
            minX=sphere.Center.X-sphere.Radius; minY=sphere.Center.Y-sphere.Radius;
            theme.ChannelIndex0=static_cast<unsigned char>(config&1);
            theme.ChannelIndex1=static_cast<unsigned char>(config&2);
            theme.ChannelBlendFactor=static_cast<unsigned char>(config&4);
            reinterpret_cast<unsigned char*>(managerData)[0xa6]=(config&8)!=0;
            layer=config-13; cacheHit=(config&16)!=0; cacheCalls=fullCalls=0;
            bool tryCache=r<=2 && !(config&5) && (config&8);
            void* expected=tryCache && cacheHit ? &cacheTag : &fullTag;
            if(light->GetLightsInSphereFast(sphere,layer,theme)!=expected ||
               cacheCalls!=int(tryCache) || fullCalls!=int(!(tryCache && cacheHit))) ++issues;
            ++cases;
        }
    }
    if(issues) return 1;
    printf("00b46b90_TEST PASS\n"); return 0;
}
