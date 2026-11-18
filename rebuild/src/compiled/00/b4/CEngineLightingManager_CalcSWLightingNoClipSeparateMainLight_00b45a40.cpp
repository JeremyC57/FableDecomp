// CEngineLightingManager::CalcSWLightingNoClipSeparateMainLight(const C3DVector&, const C3DVector&, const CEngineLocalLightArray&, CRGBFloatColour&, CRGBFloatColour&) -- accumulate diffuse lighting separately from ambient, backlight and local contributions.
// Leaf: ?CalcSWLightingNoClipSeparateMainLight@CEngineLightingManager@@QAEXABVC3DVector@@0ABVCEngineLocalLightArray@@AAVCRGBFloatColour@@2@Z
// Retail: 0x00B45A40, 683 bytes, thiscall.
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_SEPARATE_MAIN
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/GFLimit.h"
#include "engine/GFWorldDistance.h"
#include "engine/CEngineLightingManager.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void CEngineLightingManager::CalcSWLightingNoClipSeparateMainLight(const C3DVector& pos,
    const C3DVector& normal, const CEngineLocalLightArray& light_array,
    CRGBFloatColour& main_light_result, CRGBFloatColour& misc_light_result)
{
    float dp = -normal.Dot(DiffuseVector);
    float frontval = MIN(GFSqr(MAX(dp, 0.0f)), 1.0f);
    float backval = MAX(-dp, 0.0f);
    C3DVector main_col = PrimitiveDiffuseColour * frontval;
    main_light_result.R = main_col.X;
    main_light_result.G = main_col.Y;
    main_light_result.B = main_col.Z;
    C3DVector col = PrimitiveAmbientColour + PrimitiveBacklightColour * backval;
    for (long l = 0; l < light_array.GetCount(); ++l) {
        const CEngineLocalLightElement& light = light_array.PeekLight(l);
        C3DVector light_vec = light.WorldPos - pos;
        light_vec.Normalise();
        float dot = light_vec.Dot(normal);
        GFLimit(dot, 0.0f, 1.0f);
        if (dot > DELTA_NOTIONAL_ZERO) {
            float dist2 = (light.WorldPos - pos).GetSquaredMagnitude();
            float falloff = 1.0f / (dist2 * light.AttenuationDistSquared) + light.AttenuationConstant;
            dot *= falloff;
            GFLimit(dot, 0.0f, 1.0f);
            col += light.Colour * dot;
        }
    }
    misc_light_result.R = col.X;
    misc_light_result.G = col.Y;
    misc_light_result.B = col.Z;
}