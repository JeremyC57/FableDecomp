// CEngineLightingManager::CalcSWLighting(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array) -- software lighting of one vertex: the global diffuse / ambient / backlight terms plus every local light (fast-normalised direction, clamped lambert above DELTA_NOTIONAL_ZERO, distance falloff), each colour channel clamped to 1, alpha 1
// Leaf: ?CalcSWLighting@CEngineLightingManager@@QAE?AVCRGBFloatColour@@ABVC3DVector@@0ABVCEngineLocalLightArray@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B45720, 787 bytes (`ret 10h`: hidden result, pos, normal, light_array). No calls.
// Naming: Ego_r.pdb / FableWin.pdb ?CalcSWLighting@CEngineLightingManager@@QAE?AVCRGBFloatColour@@ABVC3DVector@@0ABVCEngineLocalLightArray@@@Z
//   (Ego_r order: the global-only overload, then this one = retail 0x00B45510, 0x00B45720). Ego_d.pdb has no record for this overload (its
//   one CalcSWLighting record is the global-only 0x00B45510: backval, dp, frontval, col); the body is the e08 twin CalcSWLightingNoClip
//   0x00B45CF0 (Ego_d locals pos, normal, light_array, col, l, light, light_vec, dot, dist2, falloff) returning the clamped colour.
//   Constants 0.0f 0x0122DEDC, 1.0f 0x0122DED8, DELTA_NOTIONAL_ZERO 0x0129BA3C, GFastOneOverSqrtTable 0x013CE558.
// Header opt-ins: e08's CEngineLightingManager.h FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP / FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT and
//   CRGBFloatColour.h FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR (same names, same bodies) plus FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_LOCALLIGHTS.
// FITTED: `falloff` is not named here (the product is `dot *= 1.0f / (dist2 * ...) + ...`). With NoClip's full set of names the X term of the
//   global dot loads DiffuseVector.X first (0x00B45735: retail `fld [normal.X] ; fmul [DiffuseVector.X]`); a diagnostic sweep of 1..7 dead
//   named locals matched only at +6 / +7 (the r3004 per-function symbol key, mod 8), i.e. one or two FEWER names than this text had:
//   dropping falloff, dist2 or both all match. Without an Ego_d record for this overload, which name the original lacked is not known.
// Macro evidence: MIN / MAX expand their argument twice and retail evaluates the clamped value twice -- the MAX(dp, 0) test at
//   0x00B4573E..0x00B4574D and the square of the re-loaded dp again at 0x00B4576E..0x00B45787 inside MIN -- so the original used
//   argument-duplicating macros (as the landed twin 0x00B45510 and e08's NoClip 0x00B45CF0 define them).
//   MIN / MAX are the row's own macros (as the landed twin 0x00B45510 and e08's NoClip; retail evaluates the clamped square twice).
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_LOCALLIGHTS
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/GFLimit.h"
#include "engine/GFWorldDistance.h"
#include "engine/CEngineLightingManager.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

CRGBFloatColour CEngineLightingManager::CalcSWLighting(const C3DVector& pos, const C3DVector& normal, const CEngineLocalLightArray& light_array)
{
    float dp = -DiffuseVector.Dot(normal);
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
            dot *= 1.0f / (dist2 * light.AttenuationDistSquared) + light.AttenuationConstant;
            GFLimit(dot, 0.0f, 1.0f);
            col += light.Colour * dot;
        }
    }
    if (col.X > 1.0f) col.X = 1.0f;
    if (col.Y > 1.0f) col.Y = 1.0f;
    if (col.Z > 1.0f) col.Z = 1.0f;
    return CRGBFloatColour(col.X, col.Y, col.Z, 1.0f);
}
