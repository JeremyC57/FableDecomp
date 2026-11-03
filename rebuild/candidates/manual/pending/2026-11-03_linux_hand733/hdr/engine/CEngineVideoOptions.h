#ifndef FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
#define FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
// Retail model (chunk 733, CUserProfileManager::ApplyVideoValues 0x0040A7F0): the engine video-option block handed to
// CIEngine::SetVideoOptions (slot 14). Ego_r.pdb CEngineVideoOptions (struct_layouts_egor.tsv, 0x34 bytes, no containers):
// ScreenResolution +0x00, ScreenAspectRatio +0x08, BitsPerPixel +0x10, RefreshRate +0x14, VerticalSync +0x18, the five
// EEngineVideoOptionQuality members +0x1C..+0x2C, Antialiasing +0x30. Default constructor: FableWin.pdb ??0CEngineVideoOptions@@QAE@XZ,
// inline in retail; its stores (0x0040A800..0x0040A81D) are the body below -- ScreenResolution (0, 0), BitsPerPixel 0,
// RefreshRate 0, VerticalSync true, LandscapeDetail 1, Antialiasing 0; ScreenAspectRatio and the other four qualities are left
// to the caller. EEngineVideoOptionQuality's enumerators are in neither PDB name list: MS forward declaration, values as casts.
#include "engine/C2DExtentsI.h"
#include "engine/C2DExtentsF.h"
#include "engine/ESurfaceMultisampleType.h"
enum EEngineVideoOptionQuality;                                     // Ego_r.pdb global enum (0 .. 3, see ConvertFloatToVideoOptionQuality)
class CEngineVideoOptions {
public:
    CEngineVideoOptions()
        : ScreenResolution(0, 0), BitsPerPixel(0), RefreshRate(0), VerticalSync(true),
          LandscapeDetail((EEngineVideoOptionQuality)1), Antialiasing((ESurfaceMultisampleType)0) {}
    C2DExtentsI               ScreenResolution;                   // +0x00
    C2DExtentsF               ScreenAspectRatio;                  // +0x08
    long                      BitsPerPixel;                       // +0x10
    long                      RefreshRate;                        // +0x14
    bool                      VerticalSync;                       // +0x18
    EEngineVideoOptionQuality LandscapeDetail;                    // +0x1c
    EEngineVideoOptionQuality MeshDetail;                         // +0x20
    EEngineVideoOptionQuality TextureDetail;                      // +0x24
    EEngineVideoOptionQuality ShadowQuality;                      // +0x28
    EEngineVideoOptionQuality EffectsQuality;                     // +0x2c
    ESurfaceMultisampleType   Antialiasing;                       // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CEngineVideoOptions) == 0x34);
#endif
