#ifndef FABLE_ENGINE_CMATRIX4X4_H
#define FABLE_ENGINE_CMATRIX4X4_H
#include "fable_compiler.h"
// CMatrix4x4: one retail model (new 2026-11-03 from Codex pass 13; its FABLE_RETAIL_SHADER_LIGHTING part folded in). Ego_r TPI
// 0xA231: the 16 named float entries E11..E44 overlap the public Data[4][4] (0x40 bytes). CShaderRenderManager::Initialise
// 0x0098ACF0 stores four diagonal ones and twelve off-diagonal zeroes per matrix (?SetIdentity@CMatrix4x4@@QAEXXZ, inline);
// UpdateObjectSpaceLighting 0x00989CB0 calls ?SetInverseOfUniformScaleRotationTranslation@CMatrix4x4@@QAEMABV1@@Z out of line and
// expands ?TransformTransposedAssumeW@CMatrix4x4@@QBEXABVC4DVector@@AAV2@M@Z in place (no retail out-of-line copy).
#include "rebuild_abi.h"
#include "engine/C4DVector.h"
class CMatrix4x4 {
public:
    union {
        struct { float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44; };
        float Data[4][4];
    };
    float SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4&);
    FABLE_FORCEINLINE void TransformTransposedAssumeW(const C4DVector& src,C4DVector& dest,float w) const {
        C4DVector vect=src;
        dest.X=E11*vect.X+E12*vect.Y+E13*vect.Z+E14*w;
        dest.Y=E21*vect.X+E22*vect.Y+E23*vect.Z+E24*w;
        dest.Z=E31*vect.X+E32*vect.Y+E33*vect.Z+E34*w;
        dest.W=w;
    }
    void TransformTransposedAssumeW(C4DVector& vect,float w) const;
#ifdef FABLE_RETAIL_CMATRIX4X4
    // Retail opt-in (chunk 748): the Aligned* members of the 16-byte-row matrix (FableWin.pdb names, adjacent in FableWin and
    // in Fable.exe to C3DAnimationManager::ApplyBoneMorphScaling 0x00A9D750, which expands both PreScale forms in place):
    // ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z 0x00A9D480 (row r times scale[r]) and
    // ?AlignedPreScaleTransposed@CMatrix4x4@@QAEXABVC3DVector@@@Z 0x00A9D580 (column c times scale[c]); both keep E14/E24/E34
    // and leave row 4 alone. Their SSE arms are the isolated kernels of engine/CMatrix4x4AlignedSSE.h.
    void AlignedPreScale(const C3DVector& scale);
    void AlignedPreScaleTransposed(const C3DVector& scale);
    // ?AlignedSetMultiply@CMatrix4x4@@QAEXABV1@0@Z (FableWin; no retail out-of-line copy): this = a * b (row vectors), the SSE
    // arm the GF_SSE_MATRIX_MULTIPLY kernel (operands re-read from the parameter slots), the x87 arm the row loop. Expanded in
    // place by CParticleProgram::ClingToMesh 0x00AE1320 / SetupSimulation 0x00AE29D0 and CShaderRenderManager::
    // UpdateObjectSpaceTextureTransform 0x00989230 / UpdateLocalisedSphereMapping 0x00988E80.
    FABLE_FORCEINLINE void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b);
#endif
    void SetIdentity() {
        E11=1.0f; E22=1.0f; E33=1.0f; E44=1.0f;
        E12=0.0f; E13=0.0f; E14=0.0f; E21=0.0f; E23=0.0f; E24=0.0f; E31=0.0f; E32=0.0f; E34=0.0f; E41=0.0f; E42=0.0f; E43=0.0f;
    }
};
FABLE_STATIC_ASSERT(sizeof(CMatrix4x4)==64);
#ifdef FABLE_RETAIL_CMATRIX4X4
#include "engine/CMatrix4x4AlignedSSE.h"   // the SSE kernels and the inline AlignedSetMultiply body
#endif
#endif
