// CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
// Grade: RELOCATION_MATCH 457v457 (asm oracle; SSE retail_asm pending the owner -- the brief forbids landing SSE retail_asm). Names respelled to Ego_r.pdb: TransformVertices private const with const CArray<C3DGroup2>&, NVectorMath::GFClear4DVectors(..., const C4DVector&), static __fastcall CParticleProgram::GetGroupVectorBuffer, C3DGroup2::PeekVertexBlends, CMatrix4x4::AlignedTransformTransposedAssumeW const, g_SSEAvailable; this modelled on engine/CParticleProgram.h. Intrinsics best: differ/00ae0e60.cpp 445v457.
// Retail-asm: 0x00AE0F86..0x00AE0FA3 `mov eax,[esp+10h] ; mov edx,[esp+1Ch] ; movaps xmm0,[eax] ; movaps xmm1,[edx] ; movaps xmm2,[esp+30h] ; shufps xmm2,xmm2,0 ; mulps xmm0,xmm2 ; addps xmm1,xmm0 ; movaps [edx],xmm1` -- source/target stored to frame slots before the SSE test and reloaded into fixed registers (an __asm block naming C locals); _mm_* intrinsics never reload a register-held pointer and pick their own xmm registers (probe: differ/00ae0e60.cpp, 445v457).
// const CArray<C3DGroup2>& groups) const -- clear the output, then for every blend group gather its source vertices
// into the shared group buffer, transform them by the group's matrix and accumulate each into its output vertex scaled
// by the blend weight (SSE when the processor supports it, x87 otherwise)
// Leaf: ?TransformVertices@CParticleProgram@@ABEXPAVC4DVector@@PBV2@KPBVCMatrix4x4@@ABV?$CArray@VC3DGroup2@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE0E60, 0x1C9 bytes (padded to 464), `ret 14h`, `and esp,-16` frame.
// Callees: GFClear4DVectors(C4DVector*, unsigned long, C4DVector*) 0x00A5C300 (cdecl), C3DGroup2's GetList 0x00AC40A0,
//   GetGroupVectorBuffer(unsigned long) 0x00AE05A0 (__fastcall), CMatrix4x4::AlignedTransformTransposedAssumeW
//   0x00A5CB90. Data: the SSE-available flag byte 0x013D2880.
// Layout: local model. C3DGroup2 is 16 bytes (matrix index, then a 12-byte vector of 8-byte C3DVertexBlend2 {index,
//   weight}); CMatrix4x4 is 64 bytes; C4DVector is the 16-byte SSE-aligned vector.
#define CParticleProgram CParticleProgramLayout
#include "engine/CParticleProgram.h"
#undef CParticleProgram
class __declspec(align(16)) C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    C4DVector& operator+=(const C4DVector& o) { X += o.X; Y += o.Y; Z += o.Z; W += o.W; return *this; }
};
namespace NVectorMath {
void GFClear4DVectors(C4DVector* vectors, unsigned long count, const C4DVector& value);   // 0x00A5C300
}
extern bool g_SSEAvailable;                                                              // 0x013D2880
class CMatrix4x4 {
public:
    float M[16];
    void AlignedTransformTransposedAssumeW(const C4DVector* in, C4DVector* out, unsigned long count, float w) const;   // 0x00A5CB90
};
class C3DVertexBlend2 {
public:
    unsigned long VertexIndex;                          // +0x00
    float         BlendValue;                           // +0x04
};
template <class T> class CArray {                       // retail STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};
class C3DGroup2 {
public:
    long                     BoneIndex;                 // +0x00
    CArray<C3DVertexBlend2>  VertexBlends;              // +0x04
    unsigned long GetNumVertexBlends() const { return VertexBlends.size(); }
    const C3DVertexBlend2* PeekVertexBlends() const;    // 0x00AC40A0
};
class CParticleProgram : public CParticleProgramLayout {
    static C4DVector* __fastcall GetGroupVectorBuffer(unsigned long count);   // 0x00AE05A0
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
                           const CArray<C3DGroup2>& groups) const;
};
void CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count,
                                         const CMatrix4x4* matrices, const CArray<C3DGroup2>& groups) const
{
    {
        C4DVector zero(0.0f, 0.0f, 0.0f, 0.0f);
        NVectorMath::GFClear4DVectors(out, count, zero);
    }
    for (unsigned long i = 0; i < groups.size(); ++i) {
        const C3DGroup2& group = groups[i];
        unsigned long blendCount = group.GetNumVertexBlends();
        if (!blendCount)
            continue;
        const CMatrix4x4& matrix = matrices[group.BoneIndex];
        const C3DVertexBlend2* blends = group.PeekVertexBlends();
        C4DVector* buffer = GetGroupVectorBuffer(blendCount);
        for (unsigned long j = 0; j < blendCount; ++j)
            buffer[j] = in[blends[j].VertexIndex];
        matrix.AlignedTransformTransposedAssumeW(buffer, buffer, blendCount, 1.0f);
        for (unsigned long k = 0; k < blendCount; ++k) {
            const C3DVertexBlend2& blend = blends[k];
            const C4DVector* source = &buffer[k];
            C4DVector* target = &out[blend.VertexIndex];
            if (g_SSEAvailable) {
                C4DVector weight;
                weight.X = blend.BlendValue;
                __asm {
                    mov     eax, source
                    mov     edx, target
                    movaps  xmm0, [eax]
                    movaps  xmm1, [edx]
                    movaps  xmm2, weight
                    shufps  xmm2, xmm2, 0
                    mulps   xmm0, xmm2
                    addps   xmm1, xmm0
                    movaps  [edx], xmm1
                }
            } else {
                *target += *source * blend.BlendValue;
            }
        }
    }
}
