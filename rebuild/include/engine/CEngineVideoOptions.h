#ifndef FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
#define FABLE_ENGINE_CENGINEVIDEOOPTIONS_H
#ifdef FABLE_RETAIL_CENGINECAMERA_SETSTATE_CONTEXT_PASS26
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
};
#endif
#endif
