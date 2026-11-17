// CEngineLightingManager::CalcSWLightingNoClip(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array, CRGBFloatColour& light_result) -- software lighting of one vertex: global diffuse / ambient / backlight terms plus every local light whose clamped Lambert term passes DELTA_NOTIONAL_ZERO (inverse-square falloff), RGB written unclamped
// Leaf: ?CalcSWLightingNoClip@CEngineLightingManager@@QAEXABVC3DVector@@0ABVCEngineLocalLightArray@@AAVCRGBFloatColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B45CF0, 697 bytes (`ret 10h`). Constants 0.0f 0x0122DEDC, 1.0f 0x0122DED8, DELTA_NOTIONAL_ZERO 0x0129BA3C,
//   GFastOneOverSqrtTable 0x013CE558 (the shared inline Normalise / GFFastOneOverSqrt).
// Naming: Ego_r.pdb / FableWin.pdb ?CalcSWLightingNoClip@CEngineLightingManager@@QAEXABVC3DVector@@0ABVCEngineLocalLightArray@@AAVCRGBFloatColour@@@Z.
// Source: Ego_d.pdb locals pos, normal, light_array, light_result; backval, dp, frontval, col (a C3DVector); { l { light_vec, light, dot
//   { dist2, falloff } } } and the object's inline CEngineLocalLightArray::GetCount / PeekLight and CRGBFloatColour::operator C3DVector
//   (FableWin ??BCRGBFloatColour@@QBE?AVC3DVector@@XZ): the colour sums are CRGBFloatColour arithmetic converted to C3DVector. The
//   light's product reuses `dot` (no other name in Ego_d): `dot *= falloff;` then the clamp -- that closed the last residue (the loop
//   pointer base and the colour-temporary frame words). `-normal.Dot(DiffuseVector)` gives retail's normal.X-first load; dist2 is the
//   squared length of the un-normalised difference (light.WorldPos - pos), recomputed, as retail does (0x00B45F02..).
// Header opt-ins: CEngineLightingManager.h FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP / FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT,
//   CRGBFloatColour.h FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR. MIN / MAX are the row's own macros (as the landed twin 0x00B45510).
// FITTED: `-normal.Dot(DiffuseVector)` (operand side chosen by grading: retail loads normal.X first, 0x00B45D05), the recomputed
//   `(light.WorldPos - pos).GetSquaredMagnitude()` for dist2 and the `dot *= falloff` reuse (both chosen by grading within Ego_d's locals).
// Macro evidence: MIN / MAX expand their argument twice, and retail evaluates the clamped square twice -- the `MAX(dp, 0)` test at
//   0x00B45D2B..0x00B45D3C and the square again at 0x00B45D3E..0x00B45D63 -- so the original used argument-duplicating macros here.
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/GFLimit.h"
#include "engine/GFWorldDistance.h"
#include "engine/CEngineLightingManager.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void CEngineLightingManager::CalcSWLightingNoClip(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array, CRGBFloatColour& light_result)
{
    float dp = -normal.Dot(DiffuseVector);
    float frontval = MIN(GFSqr(MAX(dp, 0.0f)), 1.0f);
    float backval = MAX(-dp, 0.0f);
    C3DVector col = PrimitiveDiffuseColour * frontval + PrimitiveAmbientColour + PrimitiveBacklightColour * backval;
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
    light_result.R = col.X;
    light_result.G = col.Y;
    light_result.B = col.Z;
}
