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
#include "engine/CIDrawEnvironmentTextureSet.h"   // the repo model (24 bytes)
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#include "engine/CIDrawEnvironmentScreenEffectParams.h"
#endif
struct CIDrawEnvironmentParams {
    struct CThemeChannel {                                               // 0x1f0 bytes in retail (Ego_r 0x208: three 0x20 texture sets)
        CRGBFloatColour             Colours[19];                         // +0x000
        float                       Scalers[30];                         // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];                      // +0x1a8
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
        // Opt-in (chunk 2019, row 0x006B42F0 CEnvironment::RenderUpdate): Ego_r.pdb / FableWin.pdb
        // ??0CThemeChannel@CIDrawEnvironmentParams@@QAE@XZ and ??1CThemeChannel@CIDrawEnvironmentParams@@QAE@XZ, out of line in retail at
        // 0x006B5FB0 (landed; zeroes the three texture sets) and 0x006B5B80 (landed; frees them last to first). RenderUpdate's
        // ThemeChannels.resize(6) builds its value temporary with both (0x006B4383 / 0x006B4399). Declarations only.
        CThemeChannel();                                                 // 0x006B5FB0
        ~CThemeChannel();                                                // 0x006B5B80
#endif
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
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
    // Opt-in (chunk 2019): Ego_r.pdb ScreenEffectParams (CIDrawEnvironmentScreenEffectParams, Ego_r +0x64 = retail +0x60 after the
    // 12-byte CArray); RenderUpdate fills RadialBlur (+0x60, `rep movsd` into this +0x18F4) and Displacement (+0xB0, this +0x1944).
    CIDrawEnvironmentScreenEffectParams ScreenEffectParams;              // +0x60
#endif
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams::CThemeChannel) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CIDrawEnvironmentParams, ThemeChannels) == 0x54);
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
FABLE_STATIC_ASSERT(offsetof(CIDrawEnvironmentParams, ScreenEffectParams) == 0x60);
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams) == 0xc0);
#endif
#endif // FABLE_ENGINE_CIDRAWENVIRONMENTPARAMS_H
