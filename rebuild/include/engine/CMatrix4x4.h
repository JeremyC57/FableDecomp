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
#ifdef FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED
#include "engine/CMatrix3x4.h"
#endif
class CMatrix4x4 {
public:
#ifdef FABLE_RETAIL_CMATRIX4X4_CAMERA_INITIALISE_PASS26
    // Both original type records place the sixteen floats in row order at offsets 0 through 60.
    float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44;
    void Initialise(float e11,float e12,float e13,float e14,float e21,float e22,float e23,float e24,
                                      float e31,float e32,float e33,float e34,float e41,float e42,float e43,float e44) {
        E11=e11; E12=e12; E13=e13; E14=e14; E21=e21; E22=e22; E23=e23; E24=e24;
        E31=e31; E32=e32; E33=e33; E34=e34; E41=e41; E42=e42; E43=e43; E44=e44;
    }
#else
    union {
        struct { float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44; };
        float Data[4][4];
    };
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
    // Retail opt-in (Codex pass20): both original TPI overloads; FableWin02e8e480.
    float& operator()(int row,int column) { return Data[row][column]; }
    const float operator()(int row,int column) const { return Data[row][column]; }
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_SHADOW_PERSPECTIVE_PASS22
    // Original donor02d797f0 procedure locals x/y/z/w; method signature in both PDBs.
#ifndef FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
    CMatrix4x4() { }
#endif
    void SetInverse(const CMatrix4x4&);
    void PerspectiveTransformPoint(C3DVector& point) {
        float x=point.X*E11+point.Y*E21+point.Z*E31+E41;
        float y=point.X*E12+point.Y*E22+point.Z*E32+E42;
        float z=point.X*E13+point.Y*E23+point.Z*E33+E43;
        float w=point.X*E14+point.Y*E24+point.Z*E34+E44;
        point.X=x/w;point.Y=y/w;point.Z=z/w;
    }
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
#ifdef FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
    // Retail opt-in (chunk 2047, row 0x00A9E1E0 C3DAnimationManager::ApplyParentTransformations): FableWin.pdb
    // ?AlignedSetMultiplyTransposed@CMatrix4x4@@QAEXABV1@0@Z (0x02E8E150, 29 bytes / 13 debug instructions = one forwarding call; VC7.1
    // /Od of this body gives the same 13 / 29), inline in retail: this = b * a, i.e. the product of two transposed (column-vector)
    // matrices. The row's transposed loop expands it with a = the bone's local matrix and b = its parent's (SSE kernel operands
    // 0x00A9E515..0x00A9E530: eax = parent, edx = local).
#ifdef FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
    FABLE_FORCEINLINE
#endif
    void AlignedSetMultiplyTransposed(const CMatrix4x4& a, const CMatrix4x4& b) { AlignedSetMultiply(b, a); }
#ifdef FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED
    // Retail opt-in (agentC40, row 0x00BD2D90 CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations): FableWin.pdb
    // ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z, inline in retail: an aligned copy of *this (`rep movsd` of 16 words to an
    // `and esp,-16` frame, 0x00BD2E70), then this = copy * m through AlignedSetMultiplyTransposed (SSE operands eax = copy, edx = m).
    // Text by FableWin size (30i / 70b): default-construct + assign with the FableWin ??0CMatrix4x4@@QAE@XZ visible
    // (FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4) is 30i / 69b at /Od; copy-initialisation is 28i / 61b. Same retail bytes either way.
#ifdef FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_FORCEINLINE
    FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
    // Nested opt-in (p989): the Ego_d.pdb spelling (debug length 70): param `matrix`, local `m` -- the text of the out-of-line copy
    // 0x00ADEFC0. Same bytes as the #else text (0x00AE1320 / 0x00BD2D90 RELOCATION_MATCH either way): a naming change, kept under the
    // opt-in so the landed users' preprocessed text is unchanged.
    void AlignedPreMultiplyTransposed(const CMatrix4x4& matrix) { __declspec(align(16)) CMatrix4x4 m; m = *this; AlignedSetMultiplyTransposed(matrix, m); }
#else
    void AlignedPreMultiplyTransposed(const CMatrix4x4& m) { __declspec(align(16)) CMatrix4x4 copy; copy = *this; AlignedSetMultiplyTransposed(m, copy); }
#endif
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_DEFINITION
    // Retail opt-in (Codex pass 24): the FableWin/Ego_r public member also has an out-of-line copy at 0x00ADEFC0.
    void AlignedPreMultiplyTransposed(const CMatrix4x4& matrix);
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
    // Retail opt-in (agentC42, row 0x00A5CB90): Ego_r.pdb / FableWin.pdb ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z
    // (reloc-bound from CParticleSimulation::ApplyDragging 0x00ADF300): count aligned vectors, SSE kernel of
    // engine/CMatrix4x4TransformSSE.h four at a time, the inline TransformTransposedAssumeW for the rest.
    void AlignedTransformTransposedAssumeW(const C4DVector* src, C4DVector* dest, unsigned long count, float w) const;
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED
    // Retail opt-in (wfA6, row 0x00ADE080 C3DBoundingBox::InitialiseFromOBB): FableWin.pdb ?InitialiseTransposed@CMatrix4x4@@QAEXABVCMatrix3x4@@@Z,
    // inline in retail: the 3x4's rows become the first three rows' columns (E11..E14 = m.E11 / m.E21 / m.E31 / m.E41, ...) and the
    // fourth row is 0 / 0 / 0 / 1 (0x00ADE16C..0x00ADE238 + 0x00ADE281..0x00ADE2A9).
    FABLE_FORCEINLINE void InitialiseTransposed(const CMatrix3x4& m) {
        E11=m.E11; E12=m.E21; E13=m.E31; E14=m.E41;
        E21=m.E12; E22=m.E22; E23=m.E32; E24=m.E42;
        E31=m.E13; E32=m.E23; E33=m.E33; E34=m.E43;
        E41=0.0f; E42=0.0f; E43=0.0f; E44=1.0f;
    }
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
#ifdef FABLE_RETAIL_MATRIX_POST_MULTIPLY_PASS30
    // FableWin 02D796C0 and Ego_d retain temp_matrix, long v and long u.
    // Both debug loop openings use the original VC6 for-scope form.
    void PostMultiply(const CMatrix4x4& matrix) {
        CMatrix4x4 temp_matrix;
        if (false) {} else for (long v=0; v<4; ++v)
            if (false) {} else for (long u=0; u<4; ++u)
                temp_matrix.Data[v][u] = Data[v][0] * matrix.Data[0][u] + Data[v][1] * matrix.Data[1][u] + Data[v][2] * matrix.Data[2][u] + Data[v][3] * matrix.Data[3][u];
        *this = temp_matrix;
    }
#else
    void PostMultiply(const CMatrix4x4& m) {
        CMatrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.Data[i][j] = Data[i][0] * m.Data[0][j] + Data[i][1] * m.Data[1][j] + Data[i][2] * m.Data[2][j] + Data[i][3] * m.Data[3][j];
        *this = result;
    }
#endif
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
#ifdef FABLE_RETAIL_SHADOW_TRANSFORMS_PASS30
    // Inline-budget: retail expands this complete transform at each call site. The original debug loops have long row.
    FABLE_FORCEINLINE void PostTranslate(const C3DVector& vec) {
        if (false) {} else for (long row = 0; row < 4; ++row) {
            Data[row][0] += vec.X * Data[row][3];
            Data[row][1] += vec.Y * Data[row][3];
            Data[row][2] += vec.Z * Data[row][3];
        }
    }
#else
    FABLE_FORCEINLINE void PostTranslate(const C3DVector& translation) {
        for (int i = 0; i < 4; ++i) {
            Data[i][0] += Data[i][3] * translation.X;
            Data[i][1] += Data[i][3] * translation.Y;
            Data[i][2] += Data[i][3] * translation.Z;
        }
    }
#endif
#ifdef FABLE_RETAIL_SHADOW_TRANSFORMS_PASS30
    // Inline-budget: retail expands this complete transform at each call site. The original debug loops have long row.
    FABLE_FORCEINLINE void PostScale(const C3DVector& vec) {
        if (false) {} else for (long row = 0; row < 4; ++row) {
            Data[row][0] *= vec.X;
            Data[row][1] *= vec.Y;
            Data[row][2] *= vec.Z;
        }
    }
#else
    FABLE_FORCEINLINE void PostScale(const C3DVector& scale) {
        for (int i = 0; i < 4; ++i) {
            Data[i][0] *= scale.X;
            Data[i][1] *= scale.Y;
            Data[i][2] *= scale.Z;
        }
    }
#endif
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_COPYTO
    void CopyTo(class CMatrix3x4& matrix) const;                        // FableWin ?CopyTo@CMatrix4x4@@QBEXAAVCMatrix3x4@@@Z (inline)
#endif
    void SetIdentity() {
#ifdef FABLE_RETAIL_MATRIX_IDENTITY_ROW_ORDER_PASS30
        // The original SetIdentity stores sixteen values in row order.
        E11=1.0f; E12=0.0f; E13=0.0f; E14=0.0f;
        E21=0.0f; E22=1.0f; E23=0.0f; E24=0.0f;
        E31=0.0f; E32=0.0f; E33=1.0f; E34=0.0f;
        E41=0.0f; E42=0.0f; E43=0.0f; E44=1.0f;
#else
        E11=1.0f; E22=1.0f; E33=1.0f; E44=1.0f;
        E12=0.0f; E13=0.0f; E14=0.0f; E21=0.0f; E23=0.0f; E24=0.0f; E31=0.0f; E32=0.0f; E34=0.0f; E41=0.0f; E42=0.0f; E43=0.0f;
#endif
    }
};
FABLE_STATIC_ASSERT_AT(86, sizeof(CMatrix4x4)==64);
#ifdef FABLE_RETAIL_CMATRIX4X4
#include "engine/CMatrix4x4AlignedSSE.h"   // the SSE kernels and the inline AlignedSetMultiply body
#endif
#ifdef FABLE_RETAIL_CMATRIX4X4_COPYTO
// Opt-in (wfA5, rows 0x00AA0FA0 / 0x00AA2110): FableWin.pdb ?CopyTo@CMatrix4x4@@QBEXAAVCMatrix3x4@@@Z (0x0337F090, 155 debug bytes /
// 55 instructions = twelve element copies; emitted right before CalcBoneTransformation, its first user): the 3x3 part and the
// translation row copied into a CMatrix3x4 (retail 0x00AA20AE..0x00AA20FB). Defined out of the class: it needs CMatrix3x4.
#include "engine/CMatrix3x4.h"
inline void CMatrix4x4::CopyTo(CMatrix3x4& matrix) const
{
    matrix.E11 = E11; matrix.E12 = E12; matrix.E13 = E13;
    matrix.E21 = E21; matrix.E22 = E22; matrix.E23 = E23;
    matrix.E31 = E31; matrix.E32 = E32; matrix.E33 = E33;
    matrix.E41 = E41; matrix.E42 = E42; matrix.E43 = E43;
}
#endif
#endif
