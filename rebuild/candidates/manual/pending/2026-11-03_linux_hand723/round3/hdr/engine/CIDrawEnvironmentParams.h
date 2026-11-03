#ifndef FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
#define FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
// CIDrawEnvironmentParams (chunk 723b): Ego_r.pdb struct (U-keyed: ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ).
// Members up to ThemeChannels typed with Ego_r.pdb names and offsets (no container before it, so retail = Ego_r); ThemeChannels
// is a 12-byte STLport CArray whose start pointer the water passes read at +0x54 (0x00B70762). The nested CThemeChannel (Ego_r.pdb
// struct; retail 0x1F0, Ego_r 0x208): Colours[19], Scalers[30] +0x130, TextureSets[3] +0x1A8 (typed below, chunk 723c). The
// remainder (ScreenEffectParams ...) is not modelled: the struct is only ever reached by reference.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
// CIDrawEnvironmentTextureSet (Ego_r.pdb struct, U-keyed: ??0CIDrawEnvironmentTextureSet@@QAE@ABU0@@Z): CArray<long> Textures,
// CArray<float> Blends. Retail STLport CArrays are 12 bytes, so the set is 0x18 (Ego_r 0x20): RenderEnvironmentMap 0x00B70A90 reads
// ThemeChannels[0].TextureSets[0].Textures at channel +0x1A8 and .Blends at +0x1B4 (chunk 723c).
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                                              // +0x00
    CArray<float> Blends;                                                // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentTextureSet) == 0x18);
struct CIDrawEnvironmentParams {
    struct CThemeChannel {                                               // 0x1f0 bytes in retail (Ego_r 0x208: three 0x20 texture sets)
        CRGBFloatColour             Colours[19];                         // +0x000
        float                       Scalers[30];                         // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];                      // +0x1a8
    };
    float                  Time;                                         // +0x00
    C3DVector              WindDirection;                                // +0x04
    float                  WindSpeed;                                    // +0x10
    C3DVector              ThemeWindDirection;                           // +0x14
    float                  ThemeWindSpeed;                               // +0x20
    C3DVector              SunVector;                                    // +0x24
    C3DVector              MoonVector;                                   // +0x30
    C3DVector              DiffuseVector;                                // +0x3c
    C3DVector              ShadowVector;                                 // +0x48
    CArray<CThemeChannel>  ThemeChannels;                                // +0x54
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams::CThemeChannel) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CIDrawEnvironmentParams, ThemeChannels) == 0x54);
#endif // FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
