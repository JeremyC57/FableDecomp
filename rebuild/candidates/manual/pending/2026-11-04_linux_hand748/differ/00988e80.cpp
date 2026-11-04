// CShaderRenderManager::UpdateLocalisedSphereMapping() -- upload the normalised first and negated second rows of ViewMatrix * WorldMatrix as
// Grade: DIFFER(889v917) -- lane gap: the x87 tail (two fast inverse-sqrt normalisations and the 0.5 scale/bias) and the AlignedSetMultiply slot order (retail b/this/a at 8/0Ch/10h) -- the SSE kernel itself is GF_SSE_MATRIX_MULTIPLY; 10 tail spellings tried
//   two (v * 0.5, 0.5) sphere-map constants when the layout has room; clear dirty bit 0x8000. Retail 0x00988E80, 917 bytes.
// Retail-asm: 0x00988ED3 `movaps xmm0,[edx]` + `shufps xmm4,xmm4,00h` + `addps xmm4,xmm6` (GF_SSE_MATRIX_MULTIPLY via AlignedSetMultiply; NOTES site 3)
// Leaf: ?UpdateLocalisedSphereMapping@CShaderRenderManager@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CMATRIX4X4
#include "engine/CShaderRenderManager.h"
extern unsigned long GFastOneOverSqrtTable[128];                        // 0x013CE558
static __forceinline float FastOneOverSqrt(float value)
{
    unsigned long bits = *reinterpret_cast<unsigned long*>(&value);
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *reinterpret_cast<float*>(&estimate);
    return (3.0f - r * r * value) * r * 0.5f;
}
inline void CShaderRenderManager::SetVSConstants(long start,const void* data,unsigned long count)
{
    if(count!=0) D3DDevice->SetVertexShaderConstantF(start,(const float*)data,count);
}
void CShaderRenderManager::UpdateLocalisedSphereMapping()
{
    if (CurrentVSConstantLayout->SphereMapMatrix.Count >= 2) {
        __declspec(align(16)) CMatrix4x4 transform;
        transform.AlignedSetMultiply(ViewMatrix, WorldMatrix);
        C4DVector axes[2];
        float s = FastOneOverSqrt(transform.E11 * transform.E11 + transform.E12 * transform.E12 + transform.E13 * transform.E13);
        axes[0].X = transform.E11 * s * 0.5f; axes[0].Y = transform.E12 * s * 0.5f; axes[0].Z = transform.E13 * s * 0.5f; axes[0].W = 0.5f;
        C3DVector v(-transform.E21, -transform.E22, -transform.E23); float x = v.X, y = v.Y, z = v.Z;
        s = FastOneOverSqrt(z * z + x * x + y * y);
        axes[1].X = x * s * 0.5f; axes[1].Y = y * s * 0.5f; axes[1].Z = z * s * 0.5f; axes[1].W = 0.5f;
        SetVSConstants(CurrentVSConstantLayout->SphereMapMatrix.Offset, axes, 2);
    }
    UpdateFlags &= ~0x8000;
}
