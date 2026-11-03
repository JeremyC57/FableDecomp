#ifndef FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
#define FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
// CIDrawEnvironmentParams (chunk 723b): Ego_r.pdb struct (U-keyed: ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ).
// Members up to ThemeChannels typed with Ego_r.pdb names and offsets (no container before it, so retail = Ego_r); ThemeChannels
// is a 12-byte STLport CArray whose start pointer the water passes read at +0x54 (0x00B70762). The nested CThemeChannel (Ego_r.pdb
// struct, 0x208 bytes): Colours[19], Scalers[30] +0x130, TextureSets[3] +0x1A8 (CIDrawEnvironmentTextureSet, opaque here). The
// remainder (ScreenEffectParams ...) is not modelled: the struct is only ever reached by reference.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
struct CIDrawEnvironmentParams {
    struct CThemeChannel {                                               // 0x208 bytes
        CRGBFloatColour Colours[19];                                     // +0x000
        float           Scalers[30];                                     // +0x130
        unsigned char   TextureSets[0x60];                               // +0x1a8 CIDrawEnvironmentTextureSet[3]
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
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams::CThemeChannel) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CIDrawEnvironmentParams, ThemeChannels) == 0x54);
#endif // FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
