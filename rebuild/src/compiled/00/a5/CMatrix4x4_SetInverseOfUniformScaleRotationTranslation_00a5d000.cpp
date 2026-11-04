// CMatrix4x4::SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& source) -- invert a uniform-scale * rotation + translation matrix: the transposed 3x3 divided by the squared scale, the translation rotated back and negated, bottom row (0, 0, 0, 1); returns the inverse squared scale
// Leaf: ?SetInverseOfUniformScaleRotationTranslation@CMatrix4x4@@QAEMABV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5D000, 211 bytes, `ret 4`. Ego_r.pdb ?SetInverseOfUniformScaleRotationTranslation@CMatrix4x4@@QAEMABV1@@Z.
// Callees: none (1.0f constant 0x0122DED8). Caller: CShaderRenderManager::UpdateObjectSpaceLighting 0x00989CB0 (out of line).
// Shape: the squared scale is C3DVector::GetSquaredMagnitude of the first column (the default header body; the FABLE_RETAIL_C3DVECTOR
//   parenthesised body reorders the later products); the first translation is one sum, the other two running `+=` totals in
//   retail's term order. VC7.1 picks each product's load order from the whole function's value numbering, not the written order.
#include "engine/C3DVector.h"
#include "engine/CMatrix4x4.h"
float CMatrix4x4::SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& source)
{
    float inverseScaleSquared = 1.0f / C3DVector(source.E11, source.E21, source.E31).GetSquaredMagnitude();
    E11 = inverseScaleSquared * source.E11;
    E12 = inverseScaleSquared * source.E21;
    E13 = inverseScaleSquared * source.E31;
    E21 = inverseScaleSquared * source.E12;
    E22 = inverseScaleSquared * source.E22;
    E23 = inverseScaleSquared * source.E32;
    E31 = inverseScaleSquared * source.E13;
    E32 = inverseScaleSquared * source.E23;
    E33 = inverseScaleSquared * source.E33;
    float dot;
    E14 = -(E11 * source.E14 + E12 * source.E24 + E13 * source.E34);
    dot = E22 * source.E24;
    dot += E23 * source.E34;
    dot += source.E14 * E21;
    E24 = -dot;
    dot = E32 * source.E24;
    dot += E33 * source.E34;
    dot += source.E14 * E31;
    E34 = -dot;
    E41 = 0.0f;
    E42 = 0.0f;
    E43 = 0.0f;
    E44 = 1.0f;
    return inverseScaleSquared;
}