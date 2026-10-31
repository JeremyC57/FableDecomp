// CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
// Grade: DIFFER(445v457) 23 diff lines (from 441v457, 50) -- sec5b: pointer locals + a C4DVector weight whose X is stored and loaded whole with _mm_load_ps, a block-scoped zero (frame now 0x34 = retail), a reference local for the blend (fixes the edi/ecx IV order). Residue is only the SSE block: retail stores source/target to frame slots and reloads them into eax/edx before fixed-register movaps xmm0/xmm1/xmm2 (inline asm referencing C locals); intrinsics schedule their own registers (movaps xmm1,xmm0 ; shufps xmm1,xmm0,0). The same source with that __asm block is RELOCATION_MATCH: retail_asm_review/00ae0e60.cpp (owner decision: SSE retail_asm, as the landed GFInterpolateVectors 0x00A5BEF0).
// const CArray<C3DGroup2>& groups) const -- clear the output, then for every blend group gather its source vertices
// into the shared group buffer, transform them by the group's matrix and accumulate each into its output vertex scaled
// by the blend weight (SSE when the processor supports it, x87 otherwise)
// Leaf: TransformVertices
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE0E60, 0x1C9 bytes (padded to 464), `ret 14h`, `and esp,-16` frame.
// Callees: GFClear4DVectors(C4DVector*, unsigned long, C4DVector*) 0x00A5C300 (cdecl), C3DGroup2's GetList 0x00AC40A0,
//   GetGroupVectorBuffer(unsigned long) 0x00AE05A0 (__fastcall), CMatrix4x4::AlignedTransformTransposedAssumeW
//   0x00A5CB90. Data: the SSE-available flag byte 0x013D2880.
// Layout: local model. C3DGroup2 is 16 bytes (matrix index, then a 12-byte vector of 8-byte C3DVertexBlend2 {index,
//   weight}); CMatrix4x4 is 64 bytes; C4DVector is the 16-byte SSE-aligned vector.
#include <xmmintrin.h>
struct C4DVector {
    union {
        struct { float X, Y, Z, W; };
        __m128 M;
    };
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; }
    C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    C4DVector& operator+=(const C4DVector& o) { X += o.X; Y += o.Y; Z += o.Z; W += o.W; return *this; }
};
void __cdecl GFClear4DVectors(C4DVector* vectors, unsigned long count, C4DVector* value);   // 0x00A5C300
C4DVector* __fastcall GetGroupVectorBuffer(unsigned long count);                            // 0x00AE05A0
extern bool g_sseAvailable;                                                                 // 0x013D2880
struct CMatrix4x4 {
    float M[16];
    void AlignedTransformTransposedAssumeW(C4DVector* out, C4DVector* in, unsigned long count, float w) const;   // 0x00A5CB90
};
struct C3DVertexBlend2 {
    unsigned long Index;
    float         Weight;
};
struct C3DGroup2 {
    long              MatrixIndex;                      // +0x00
    C3DVertexBlend2*  Blends_first;                     // +0x04
    C3DVertexBlend2*  Blends_last;                      // +0x08
    C3DVertexBlend2*  Blends_end;                       // +0x0C
    unsigned long GetCount() const { return Blends_last - Blends_first; }
    const C3DVertexBlend2* GetList() const;             // 0x00AC40A0
};
struct CGroupArray {
    C3DGroup2* _M_start;
    C3DGroup2* _M_finish;
    C3DGroup2* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const C3DGroup2& operator[](unsigned long i) const { return _M_start[i]; }
};
class CParticleProgram {
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
                           const CGroupArray& groups) const;
public:
    void Run(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices, const CGroupArray& groups)
    {
        TransformVertices(out, in, count, matrices, groups);
    }
};
void CParticleProgram::TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count,
                                         const CMatrix4x4* matrices, const CGroupArray& groups) const
{
    {
        C4DVector zero(0.0f, 0.0f, 0.0f, 0.0f);
        GFClear4DVectors(out, count, &zero);
    }
    for (unsigned long i = 0; i < groups.size(); ++i) {
        const C3DGroup2& group = groups[i];
        unsigned long blendCount = group.GetCount();
        if (!blendCount)
            continue;
        const CMatrix4x4& matrix = matrices[group.MatrixIndex];
        const C3DVertexBlend2* blends = group.GetList();
        C4DVector* buffer = GetGroupVectorBuffer(blendCount);
        for (unsigned long j = 0; j < blendCount; ++j)
            buffer[j] = in[blends[j].Index];
        matrix.AlignedTransformTransposedAssumeW(buffer, buffer, blendCount, 1.0f);
        for (unsigned long k = 0; k < blendCount; ++k) {
            const C3DVertexBlend2& blend = blends[k];
            const C4DVector* source = &buffer[k];
            C4DVector* target = &out[blend.Index];
            if (g_sseAvailable) {
                C4DVector weight;
                weight.X = blend.Weight;
                __m128 s = _mm_load_ps(&source->X);
                __m128 t = _mm_load_ps(&target->X);
                __m128 w = _mm_load_ps(&weight.X);
                w = _mm_shuffle_ps(w, w, 0);
                s = _mm_mul_ps(s, w);
                t = _mm_add_ps(t, s);
                _mm_store_ps(&target->X, t);
            } else {
                *target += *source * blend.Weight;
            }
        }
    }
}
