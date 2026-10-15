// CEngineLightingManager::GetLightsInSphereFast(const C3DBoundingSphere& sphere, long layerMask, const CEnginePrimitiveThemeChannelDesc& theme) -- small untinted spheres are answered from the small-object light cache; everything else (or a cache miss) does the full sphere query
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46B90, length 107 bytes.
// Callees: CEngineLightingManager::LookupSmallObjectLightCache(float minX, float minY, long layerMask) @ 0x00B46A30,
// CEngineLightingManager::GetLightsInSphere(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&) @ 0x00B45FB0.
// Globals: the small-object diameter limit float @ 0x0122DCDC.
// Shape: `2r <= limit` (`fadd st,st ; fcomp ; test ah,41h ; jp` to the slow path), then the theme's channel index 0
// and blend factor must be 0 and the cache enabled (+0xA6); the cache lookup takes the sphere's min corner as two floats
// (computed y first into the argument area) and a non-null result is returned as is, else the full query's result.
// Layout: EnableLightCache is +0xAA in the PDB; the retail ThemeChannels CArray is 12 bytes (PDB 16), so it sits at +0xA6
// -- modelled locally.
struct CEngineLocalLightArray;

struct C3DBoundingSphere {
    float X, Y, Z;                                 // +0x00 centre
    float Radius;                                  // +0x0C
};

struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0;                   // +0x00
    unsigned char ChannelIndex1;                   // +0x01
    unsigned char ChannelBlendFactor;              // +0x02
    unsigned char Pad;                             // +0x03
};

extern const float g_smallObjectLightCacheDiameter_0122dcdc;   // 0x0122DCDC

struct CEngineLightingManager {
    unsigned char _pad000[0xa6];
    bool          EnableLightCache;                // +0xA6
    CEngineLocalLightArray* LookupSmallObjectLightCache(float minX, float minY, long layerMask);   // 0x00B46A30
    CEngineLocalLightArray* GetLightsInSphere(const C3DBoundingSphere& sphere, long layerMask,
                                              const CEnginePrimitiveThemeChannelDesc& theme);     // 0x00B45FB0
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& sphere, long layerMask,
                                                  const CEnginePrimitiveThemeChannelDesc& theme);
};

CEngineLocalLightArray* CEngineLightingManager::GetLightsInSphereFast(const C3DBoundingSphere& sphere, long layerMask,
                                                                      const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (sphere.Radius + sphere.Radius <= g_smallObjectLightCacheDiameter_0122dcdc && theme.ChannelIndex0 == 0
        && theme.ChannelBlendFactor == 0 && EnableLightCache) {
        CEngineLocalLightArray* lights =
            LookupSmallObjectLightCache(sphere.X - sphere.Radius, sphere.Y - sphere.Radius, layerMask);
        if (lights)
            return lights;
    }
    return GetLightsInSphere(sphere, layerMask, theme);
}
