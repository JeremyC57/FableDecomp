// CEngineWeatherRenderer::OverrideRain(float strength) -- a non-negative amount switches the rain override on with the amount clamped to the maximum at +0x12f8; a negative amount switches it off
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B50EE0, length 86 bytes.
// Callees: none. Constants: 0.0 (0x0122DEDC). Shape: `fcomp 0.0 ; test ah,1 ; jne` is the `>= 0` test (its false arm, the byte cleared, sits at the end);
// `fcomp [max] ; test ah,5 ; jp` is the ordered `amount < max`, and the flag store that follows the clamp is duplicated into both arms (between their `fld` and `fstp`). The
// chunk label "CEngine::OverrideRain" names the method; the object (amount/flag pairs at +0x12e8.. and the shared maximum) is the
// CEngineWeatherRenderer (not CEngine, sizeof 0x188).
// Leaf: ?OverrideRain@CEngineWeatherRenderer@@QAEXM@Z
// NF161 (2026-11-20, q4599): was the invented CWeatherState::OverrideRain on a row-local layout (own leaf ?OverrideRain@CWeatherState@@QAEXM@Z);
//   Ego_r.pdb / FableWin.pdb ?OverrideRain@CEngineWeatherRenderer@@QAEXM@Z (the landed CEngine::OverrideRain 0x00B24010 tail-jumps here on
//   NEngineInternal's weather renderer 0x01436E44, bound as that spelling). Moved onto engine/CEngineWeatherRenderer.h (the generated
//   Ego_r layout: OverridenRainStrength +0x12e8, RainOverriden +0x12ec, MaximumWeatherParticlePercentage +0x12f8; NF161 opt-in
//   FABLE_RETAIL_CENGINEWEATHERRENDERER_OVERRIDEWEATHER declares the member).
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_OVERRIDEWEATHER
#include "engine/CEngineWeatherRenderer.h"

void CEngineWeatherRenderer::OverrideRain(float strength)
{
    if (strength >= 0.0f) {
        OverridenRainStrength = strength < MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
        RainOverriden = true;
    } else {
        RainOverriden = false;
    }
}
