// 00887b70 NEnvironmentInternal::CBlendedEnvironmentTheme::Blend
// Leaf: ?Blend@CBlendedEnvironmentTheme@NEnvironmentInternal@@QBEXAAV12@ABV12@M@Z
// Flags:
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_CTOR4_INITIALISE
#include "engine/CBlendedEnvironmentTheme.h"

void NEnvironmentInternal::CBlendedEnvironmentTheme::Blend(CBlendedEnvironmentTheme& result, const CBlendedEnvironmentTheme& theme, float interp) const
{
    float blend0 = (1.0f - interp);
    float blend1 = interp;
    result.TimeOfDay = TimeOfDay * blend0 + theme.TimeOfDay * blend1;
    for (long n = 0; n < 19; ++n)
        result.Colours[n] = Colours[n] * blend0 + theme.Colours[n] * blend1;
    for (long n = 0; n < 30; ++n)
        result.Scalers[n] = Scalers[n] * blend0 + theme.Scalers[n] * blend1;
    for (long n = 0; n < 3; ++n)
        BlendTextureSets(TextureSets[n], theme.TextureSets[n], result.TextureSets[n], blend1);
}