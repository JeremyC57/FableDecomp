#ifndef FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
#define FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
#if defined(FABLE_RETAIL_CENGINECAMERA_SETSTATE_CONTEXT_PASS26) || defined(FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR)
#include "engine/C2DExtentsI.h"
#include "engine/C2DExtentsF.h"
#include "engine/EEngineVideoOptionQuality.h"
#include "engine/ESurfaceMultisampleType.h"
// Ego_r and FableWin type records. Native code reads ScreenAspectRatio at +8 and +12.
class CEngineVideoOptions {
public:
    C2DExtentsI ScreenResolution;
    C2DExtentsF ScreenAspectRatio;
    long BitsPerPixel, RefreshRate;
    bool VerticalSync;
    EEngineVideoOptionQuality LandscapeDetail, MeshDetail, TextureDetail, ShadowQuality, EffectsQuality;
    ESurfaceMultisampleType Antialiasing;
#ifdef FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR
    // NF249 opt-in (q12692): FableWin.pdb ??0CEngineVideoOptions@@QAE@XZ, inline in retail (expanded in the CEngine ctor 0x00B260B0: the
    // resolution 0 x 0, the aspect ratio 4 x 3, vertical sync on, the five qualities NORMAL, bits / refresh / antialiasing 0, stored in
    // that order). Declaration with its body only: no layout change.
    CEngineVideoOptions() : ScreenResolution(0, 0), ScreenAspectRatio(4.0f, 3.0f)
    {
        VerticalSync = true;
        LandscapeDetail = ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
        MeshDetail = ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
        TextureDetail = ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
        ShadowQuality = ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
        EffectsQuality = ENGINE_VIDEO_OPTION_QUALITY_NORMAL;
        BitsPerPixel = 0;
        RefreshRate = 0;
        Antialiasing = SURFACE_MULTISAMPLE_NONE;
    }
#endif
};
#endif
#endif
