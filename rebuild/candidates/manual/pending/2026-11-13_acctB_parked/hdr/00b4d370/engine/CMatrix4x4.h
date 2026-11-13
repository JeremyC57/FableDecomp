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
#ifdef FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#include "engine/CMatrix3x4.h"
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_SHADOWSPACE
enum EMatrix34Init { MATRIX34_INIT_IDENTITY };
#endif
class CMatrix4x4 {
public:
    union {
        struct { float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44; };
        float Data[4][4];
    };
#ifdef FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
    // Retail opt-in (Codex pass20): both original TPI overloads; FableWin02e8e480.
    float& operator()(int row,int column) { return Data[row][column]; }
    const float operator()(int row,int column) const { return Data[row][column]; }
#endif
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
#ifdef FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
    // Retail opt-in (chunk 2024, row 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates): FableWin.pdb ??0CMatrix4x4@@QAE@XZ
    // (0x02D792F0), ??0CMatrix4x4@@QAE@ABVCMatrix3x4@@@Z (0x02D79420) and ?PostMultiply@CMatrix4x4@@QAEXABV1@@Z (0x02D796C0; the
    // FableWin COMDATs after the shadow renderer's), inline in that row: the 3x4's rows are copied as the first three columns with
    // 0 / 0 / 0 / 1 in the fourth (0x00B501A9..0x00B5021B), and the PostMultiply loop (rows kept, columns unrolled) is expanded
    // (0x00B50223..0x00B50301; its unreferenced out-of-line copy is the landed 0x00B507E0, whose body this is).
    CMatrix4x4() { }
    CMatrix4x4(const CMatrix3x4& m) { Initialise(m); }
    void Initialise(const CMatrix3x4& m) {
        E11=m.E11; E12=m.E12; E13=m.E13; E14=0.0f;
        E21=m.E21; E22=m.E22; E23=m.E23; E24=0.0f;
        E31=m.E31; E32=m.E32; E33=m.E33; E34=0.0f;
        E41=m.E41; E42=m.E42; E43=m.E43; E44=1.0f;
    }
    void PostMultiply(const CMatrix4x4& m) {
        CMatrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.Data[i][j] = Data[i][0] * m.Data[0][j] + Data[i][1] * m.Data[1][j] + Data[i][2] * m.Data[2][j] + Data[i][3] * m.Data[3][j];
        *this = result;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_SHADOWSPACE
    // Retail opt-in (row 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace): FableWin.pdb ??0CMatrix4x4@@QAE@W4EMatrix34Init@@@Z
    // (0x02D79310), ?Initialise@CMatrix4x4@@QAEXMMMMMMMMMMMMMMMM@Z (0x02D795F0), ?PerspectiveTransformPoint@CMatrix4x4@@QAEXAAVC3DVector@@@Z
    // (0x02D797F0), ?PostTranslate@CMatrix4x4@@QAEXABVC3DVector@@@Z (0x02D79920), ?PostScale@CMatrix4x4@@QAEXABVC3DVector@@@Z (0x02D79A10):
    // all expanded in place in retail (no out-of-line copies).
    FABLE_FORCEINLINE CMatrix4x4(EMatrix34Init) {
        for (long j = 0; j < 4; ++j)
            for (long i = 0; i < 4; ++i)
                Data[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    FABLE_FORCEINLINE void Initialise(float e11,float e12,float e13,float e14,float e21,float e22,float e23,float e24,
                                      float e31,float e32,float e33,float e34,float e41,float e42,float e43,float e44) {
        E11=e11; E12=e12; E13=e13; E14=e14; E21=e21; E22=e22; E23=e23; E24=e24;
        E31=e31; E32=e32; E33=e33; E34=e34; E41=e41; E42=e42; E43=e43; E44=e44;
    }
    FABLE_FORCEINLINE void PerspectiveTransformPoint(C3DVector& point) {
        // FableWin 0x02D797F0 text (fwsize SAME, 87i / 241b at /Od): the four row sums from the point, then the divisions.
        const float x = point.X * E11 + point.Y * E21 + point.Z * E31 + E41;
        const float y = point.X * E12 + point.Y * E22 + point.Z * E32 + E42;
        const float z = point.X * E13 + point.Y * E23 + point.Z * E33 + E43;
        const float w = point.X * E14 + point.Y * E24 + point.Z * E34 + E44;
        point.X = x / w;
        point.Y = y / w;
        point.Z = z / w;
    }
    FABLE_FORCEINLINE void PostTranslate(const C3DVector& translation) {
        for (int i = 0; i < 4; ++i) {
            Data[i][0] += Data[i][3] * translation.X;
            Data[i][1] += Data[i][3] * translation.Y;
            Data[i][2] += Data[i][3] * translation.Z;
        }
    }
    FABLE_FORCEINLINE void PostScale(const C3DVector& scale) {
        for (int i = 0; i < 4; ++i) {
            Data[i][0] *= scale.X;
            Data[i][1] *= scale.Y;
            Data[i][2] *= scale.Z;
        }
    }
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
