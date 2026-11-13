#ifndef FABLE_ENGINE_CIDRAWENVIRONMENT_H
#define FABLE_ENGINE_CIDRAWENVIRONMENT_H
// New header (chunk 2031, row 0x00B65A20 CEngineSkyRenderer::RenderOuterSky): the engine's draw-side view of the game environment.
// - CIDrawEnvironmentParams (struct: the PDB name sets mangle it U, ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ)
//   with the Ego_r.pdb members up to ThemeChannels (struct_layouts_egor.tsv: Time +0, WindDirection +4, WindSpeed +0x10,
//   ThemeWindDirection +0x14, ThemeWindSpeed +0x20, SunVector +0x24, MoonVector +0x30, DiffuseVector +0x3C, ShadowVector +0x48,
//   ThemeChannels +0x54 -- no container before it, so retail = Ego_r; RenderOuterSky reads ThemeChannels' start at +0x54). The members
//   after ThemeChannels (ScreenEffectParams ..) are not modelled: the struct is only reached by reference.
// - The nested struct CThemeChannel (??0CThemeChannel@CIDrawEnvironmentParams@@QAE@XZ, U): Colours[19] +0, Scalers[30] +0x130,
//   TextureSets[3] +0x1A8 (Ego_r.pdb; the 24-byte retail CIDrawEnvironmentTextureSet; RenderOuterSky reads TextureSets[0].Textures +0x1A8,
//   .Blends +0x1B4 and Colours[12] / [13] +0xC0 / +0xD0).
// - CIDrawEnvironment: RTTI .?AVCIDrawEnvironment@@ (vtable 0x0125D4E4: slot 0 the scalar deleting destructor 0x006B2AD0, slot 1 pure);
//   the slot order of the overrides is CEnvironment's vtable 0x0125D53C (slot 0 0x006B7360, 1 PeekInterpolatedParams 0x006B7350,
//   2 PeekWindDirection 0x006B7310, 3 PeekWindSpeed 0x006B7320, 4 PeekSunVector 0x006B7330, 5 PeekMoonVector 0x006B7340; names and
//   signatures from the CEnvironment overrides in Ego_r.pdb / FableWin.pdb). The interface's own access is not in the PDB name sets
//   (only CEnvironment's private override); the members are public here so the engine can call them.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CIDrawEnvironmentTextureSet.h"
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#include <stddef.h>
#include "engine/CIDrawEnvironmentScreenEffectParams.h"
#endif
struct CIDrawEnvironmentParams {
    struct CThemeChannel {
        CRGBFloatColour Colours[19];                                     // +0x000
        float Scalers[30];                                               // +0x130
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
    float Time;                                                          // +0x00
    C3DVector WindDirection;                                             // +0x04
    float WindSpeed;                                                     // +0x10
    C3DVector ThemeWindDirection;                                        // +0x14
    float ThemeWindSpeed;                                                // +0x20
    C3DVector SunVector;                                                 // +0x24
    C3DVector MoonVector;                                                // +0x30
    C3DVector DiffuseVector;                                             // +0x3c
    C3DVector ShadowVector;                                              // +0x48
    CArray<CThemeChannel> ThemeChannels;                                 // +0x54
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
    // Opt-in (chunk 2019): Ego_r.pdb ScreenEffectParams (CIDrawEnvironmentScreenEffectParams, Ego_r +0x64 = retail +0x60 after the
    // 12-byte CArray); RenderUpdate fills RadialBlur (+0x60, `rep movsd` into this +0x18F4) and Displacement (+0xB0, this +0x1944).
    CIDrawEnvironmentScreenEffectParams ScreenEffectParams;              // +0x60
#endif
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams::CThemeChannel) == 0x1f0);
#ifdef FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
FABLE_STATIC_ASSERT(offsetof(CIDrawEnvironmentParams, ScreenEffectParams) == 0x60);
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentParams) == 0xc0);
#endif
class CIDrawEnvironment {
public:
    virtual ~CIDrawEnvironment();                                        // slot 0
    virtual const CIDrawEnvironmentParams& PeekInterpolatedParams() const = 0;   // slot 1
    virtual const C3DVector& PeekWindDirection() const = 0;              // slot 2
    virtual float PeekWindSpeed() const = 0;                             // slot 3
    virtual const C3DVector& PeekSunVector() const = 0;                  // slot 4
    virtual const C3DVector& PeekMoonVector() const = 0;                 // slot 5
};
#endif // FABLE_ENGINE_CIDRAWENVIRONMENT_H
