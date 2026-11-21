// NBBBMaths::IsPosInFOVWithWeight(const C3DVector& origin_pos, const C3DVector& view_direction, float view_angle, const C3DVector& test_pos, float& weight) -- is test_pos inside the view cone of view_angle (in turns) around view_direction from origin_pos; weight = how far inside (0 on the edge, 1 on the axis), 0 outside
// Leaf: ?IsPosInFOVWithWeight@NBBBMaths@@YI_NABVC3DVector@@0M0AAM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4B670, 314 bytes, __fastcall (origin_pos ecx, view_direction edx), `ret 0Ch`. Grade:
//   RELOCATION_MATCH (314/314). Identity: Ego_r.pdb / FableWin.pdb ?IsPosInFOVWithWeight@NBBBMaths@@YI_NABVC3DVector@@0M0AAM@Z; callers
//   0x006EB7DA (CCombatManager::GetBestEnemyForCombatant) and 0x00A4B80F. No callees: GFFastCos (engine/GFFastCos.h, the shared
//   retail-asm GFFloatToLongNear inside: 0x00A4B68E..96 `fstp / fld / fistp dword`), C3DVector::Normalise (GFFastOneOverSqrt) and Dot
//   expand in place. Globals: GFastCosTable 0x013CD550, GFastOneOverSqrtTable 0x013CE558.
// Shape: Ego_d.pdb lines 250..264 with its parameter names and locals cos_half_fov, to_test_pos, cos_angle_between: 252 cos_half_fov
//   (27 = VC7.1 /Od 21 + 6, the float return spill), 253 to_test_pos (16), 254 Normalise (8), 255 the dot (14 v 15), 257 the test, 259
//   the weight, 260 `return true` (4), 262 `weight = 0.0f` (7: VC10 fldz / fstp), 263 `return false` (2). Line 251 (297 B, before
//   cos_half_fov) emits nothing in retail; no text stands for it.
// Inferred: FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY (existing opt-in, row 0x00BAA870), the Normalise body that accumulates the
//   squared length Z*Z, += X*X, += Y*Y: retail's order (0x00A4B6DE `fst [esp+14h] ; fmul`, then X*X, then Y*Y) with the factor spilled
//   and the components scaled from memory (0x00A4B743 `fstp [esp+18h]`, 0x00A4B749..59). Every other shared Normalise body differs
//   (the default, ZXY, NAMED_SQUARED and PAREN_SQUARES_XYZ bodies grade DIFFER(314v314), LOCALS 312v314).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY
#include "engine/C3DVector.h"
#include "engine/GFFastCos.h"
namespace NBBBMaths {
bool FABLE_FASTCALL IsPosInFOVWithWeight(const C3DVector& origin_pos, const C3DVector& view_direction, float view_angle, const C3DVector& test_pos, float& weight);
}
bool FABLE_FASTCALL NBBBMaths::IsPosInFOVWithWeight(const C3DVector& origin_pos, const C3DVector& view_direction, float view_angle, const C3DVector& test_pos, float& weight)
{
    float cos_half_fov = GFFastCos(view_angle * 0.5f);
    C3DVector to_test_pos = test_pos - origin_pos;
    to_test_pos.Normalise();
    float cos_angle_between = to_test_pos.Dot(view_direction);
    if (cos_angle_between > cos_half_fov) {
        weight = (cos_angle_between - cos_half_fov) / (1.0f - cos_half_fov);
        return true;
    }
    weight = 0.0f;
    return false;
}