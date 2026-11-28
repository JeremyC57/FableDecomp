// Flags: 
// Leaf: ?GetCurrentFadeValue@CPSCUpdateNormal@NParticleEngine@@IBEMXZ
// Retail-asm: existing GFFloatToLongNear expands fld and fistp at 00B0A84C / 00B0A850 and 00B0A8D0 / 00B0A8D4.
#define FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
#define FABLE_RETAIL_PARTICLE_FADE_VALUE_PASS30
#include "engine/CPSCUpdateNormal.h"
#include "engine/NParticleEngine_GFSmoothFadeValue.h"

namespace NParticleEngine {
float CPSCUpdateNormal::GetCurrentFadeValue() const
{
    if (!UseSystemLifeSecs)
        return 1.0f;
    float age = 0.0f;
    if (SystemLifeSecs > 0.0f) {
        age = (NParticleEngine::GetOneOverTickRate() * LifeTimer) / SystemLifeSecs;
        if (age < 0.0f)
            age = 0.0f;
        else if (age > 1.0f)
            age = 1.0f;
    }
    float fadeIn = (float)FadeInEndInteger * 0.001f;
    float fadeOut = (float)FadeOutBeginInteger * 0.001f;
    float result = 1.0f;
    if (age < fadeIn)
        result = GFSmoothFadeValue(age / fadeIn);
    if (age > fadeOut)
        result *= GFSmoothFadeValue((1.0f - age) / (1.0f - fadeOut));
    return result;
}
}
