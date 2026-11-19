// p989 audit: sim.Size != InitialSimulation.Size, the GFInterpolateVectors strides and the anim / bone_morphs pointers are checked.
// Behaviour test for CParticleProgram::ClingToMesh (0x00AE1320; p989 Ego_d text): bone matrices become animated * bone-space (through the
// inline AlignedPreMultiplyTransposed), the initial positions are
// skinned into the vector buffer and blended into the simulation by the per-particle alphas;
// both the SSE path (the shared kernel macro, its C++ twin under -DFABLE_PORTABLE) and the x87 path are run on dyadic
// inputs, so they agree bit for bit. A model of the row over engine/ helper headers (collaborators are stubs).
#include <stdio.h>
#include <string.h>
#include "engine/CMatrix4x4AlignedSSE.h"
#if defined(_MSC_VER)
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
class C3DVector { public: float X, Y, Z; };
struct ALIGN16 C4DVector { float X, Y, Z, W; };
static bool GEnableSIMD;
struct ALIGN16 CMatrix4x4 {
    float Data[4][4];
    void AlignedPreScale(const C3DVector& scale);
    void AlignedPreScaleTransposed(const C3DVector& scale);
    void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b)   // the header's inline member (SSE kernel / x87 loop)
    {
        if (GEnableSIMD) {
            const CMatrix4x4* pa = &a;
            const CMatrix4x4* pb = &b;
            CMatrix4x4* pdst = this;
            GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
        } else {                                                        // the Ego_d x87 arm
            CMatrix4x4& m = *this;
            for (long c1 = 0; c1 < 4; ++c1)
                for (long c2 = 0; c2 < 4; ++c2)
                    m.Data[c1][c2] = a.Data[c1][0] * b.Data[0][c2] + a.Data[c1][1] * b.Data[1][c2] + a.Data[c1][2] * b.Data[2][c2] + a.Data[c1][3] * b.Data[3][c2];
        }
    }
    void AlignedSetMultiplyTransposed(const CMatrix4x4& a, const CMatrix4x4& b) { AlignedSetMultiply(b, a); }
    void AlignedPreMultiplyTransposed(const CMatrix4x4& matrix) { ALIGN16 CMatrix4x4 m; m = *this; AlignedSetMultiplyTransposed(matrix, m); }
};
static float Dy(int i) { return (float)(((i * 7) % 13) - 6) * 0.25f; }          // dyadic, products and sums exact
static void Fill(CMatrix4x4& m, int seed) { for (int i = 0; i < 16; ++i) m.Data[i / 4][i % 4] = Dy(i + seed); }
static inline void RefMul(float out[4][4], const CMatrix4x4& a, const CMatrix4x4& b)
{
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float v = 0.0f;
            for (int k = 0; k < 4; ++k) v += a.Data[r][k] * b.Data[k][c];
            out[r][c] = v;
        }
}
static int g_bad;
static void Expect(const char* what, int pass, float got, float want)
{
    if (got != want) { printf("  pass %d %s: %.9g, expected %.9g\n", pass, what, got, want); ++g_bad; }
}
class CParticleSimulation { public:
    unsigned long Size;
    C4DVector* Positions;
    C4DVector* LastPositions;
    float* SimulationAlphas;
    bool CopyOk;
    bool CopyFrom(const CParticleSimulation& other)
    {
        if (!CopyOk) return false;
        Size = other.Size;
        memcpy(Positions, other.Positions, Size * sizeof(C4DVector));
        return true;
    }
};
class C3DMesh2 { public: unsigned long BoneCount; CMatrix4x4* BoneSpaceTransforms; };
class C3DAnimationBlendState { public: int Tag; };
static int g_calcCalls;
static const void* g_seenAnim; static const void* g_seenMorphs;   // argument identity
class C3DAnimationManager { public:
    void CalcMeshTransformations(const C3DAnimationBlendState& anim, const C3DVector* morphs, const C3DMesh2& mesh, CMatrix4x4* matrices, unsigned long count, bool world)
    {
        ++g_calcCalls; g_seenAnim = &anim; g_seenMorphs = morphs;
        if (!world || count != mesh.BoneCount) ++g_bad;
        for (unsigned long i = 0; i < count; ++i) Fill(matrices[i], 20 + (int)i * 3);
    }
};
static ALIGN16 CMatrix4x4 g_matrixBuffer[8];
static ALIGN16 C4DVector g_vectorBuffer[8];   // ClingToMesh skins into it
// TransformVertices model: vertex i by bone i % 2 (x, y, z, 1) * M, W from the matrix
static void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* m)
{
    for (unsigned long i = 0; i < count; ++i) {
        const CMatrix4x4& b = m[i % 2];
        const float v[4] = { in[i].X, in[i].Y, in[i].Z, 1.0f };
        float r[4];
        for (int c = 0; c < 4; ++c) r[c] = v[0] * b.Data[0][c] + v[1] * b.Data[1][c] + v[2] * b.Data[2][c] + v[3] * b.Data[3][c];
        out[i].X = r[0]; out[i].Y = r[1]; out[i].Z = r[2]; out[i].W = r[3];
    }
}
static void ExpectSkinned(int pass, const C4DVector* got, const C4DVector* in, unsigned long count, const C3DMesh2& mesh)
{
    for (unsigned long i = 0; i < count; ++i) {
        CMatrix4x4 bone, want;
        Fill(bone, 20 + (int)(i % 2) * 3);
        RefMul(want.Data, bone, mesh.BoneSpaceTransforms[i % 2]);
        const float v[4] = { in[i].X, in[i].Y, in[i].Z, 1.0f };
        float r[4];
        for (int c = 0; c < 4; ++c) r[c] = v[0] * want.Data[0][c] + v[1] * want.Data[1][c] + v[2] * want.Data[2][c] + v[3] * want.Data[3][c];
        Expect("skinned.X", pass, got[i].X, r[0]); Expect("skinned.Y", pass, got[i].Y, r[1]);
        Expect("skinned.Z", pass, got[i].Z, r[2]); Expect("skinned.W", pass, got[i].W, r[3]);
    }
}
// GFInterpolateVectors model: out = from + (to - from) * weight (per element, strided)
static void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, const C4DVector* to,
                                 unsigned long toStride, const float* weights, unsigned long weightStride, unsigned long count)
{
    if (outStride != sizeof(C4DVector) || fromStride != sizeof(C4DVector) || toStride != sizeof(C4DVector) || weightStride != sizeof(float)) ++g_bad;
    for (unsigned long i = 0; i < count; ++i) {
        const float w = weights[i];
        out[i].X = from[i].X + (to[i].X - from[i].X) * w; out[i].Y = from[i].Y + (to[i].Y - from[i].Y) * w;
        out[i].Z = from[i].Z + (to[i].Z - from[i].Z) * w; out[i].W = from[i].W + (to[i].W - from[i].W) * w;
    }
}
class CParticleProgram { public:
    CParticleSimulation InitialSimulation;
    static CMatrix4x4* GetMatrixBuffer(unsigned long) { return g_matrixBuffer; }
    static C4DVector* GetVectorBuffer(unsigned long) { return g_vectorBuffer; }
    void ClingToMesh(CParticleSimulation& sim, const C3DAnimationBlendState& anim, C3DAnimationManager& anim_manager,
                     const C3DVector* bone_morphs, const C3DMesh2& mesh) const
    {
        unsigned long no_vertices = InitialSimulation.Size;
        C4DVector* mesh_vertices = GetVectorBuffer(no_vertices);
        unsigned long no_bones = mesh.BoneCount;
        CMatrix4x4* bone_transforms = GetMatrixBuffer(no_bones);
        anim_manager.CalcMeshTransformations(anim, bone_morphs, mesh, bone_transforms, no_bones, true);
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone]);
        TransformVertices(mesh_vertices, InitialSimulation.Positions, no_vertices, bone_transforms);
        GFInterpolateVectors(sim.Positions, sizeof(C4DVector), mesh_vertices, sizeof(C4DVector), sim.Positions, sizeof(C4DVector),
                             sim.SimulationAlphas, sizeof(float), no_vertices);
    }
};
int main()
{
    for (int pass = 0; pass < 2; ++pass) {
        GEnableSIMD = pass == 0;
        static ALIGN16 CMatrix4x4 inverse[2];
        Fill(inverse[0], 7); Fill(inverse[1], 11);
        C3DMesh2 mesh = { 2, inverse };
        static ALIGN16 C4DVector initial[4], current[4], before[4];
        float alphas[4] = { 0.0f, 1.0f, 0.5f, 0.25f };
        for (int i = 0; i < 4; ++i) {
            initial[i].X = Dy(i); initial[i].Y = Dy(i + 1); initial[i].Z = Dy(i + 2); initial[i].W = 1.0f;
            current[i].X = Dy(i + 3); current[i].Y = Dy(i + 4); current[i].Z = Dy(i + 5); current[i].W = 1.0f;
            before[i] = current[i];
        }
        CParticleProgram program;
        program.InitialSimulation.Size = 4; program.InitialSimulation.Positions = initial;
        CParticleSimulation sim;
        sim.Size = 3; sim.Positions = current; sim.SimulationAlphas = alphas;   // Size differs from InitialSimulation.Size (4): the row counts the initial set
        C3DAnimationBlendState state = { 0 };
        C3DAnimationManager manager;
        C3DVector where = { 0.0f, 0.0f, 0.0f };
        program.ClingToMesh(sim, state, manager, &where, mesh);
        ExpectSkinned(pass, g_vectorBuffer, initial, 4, mesh);
        Expect("anim/morphs identity", pass, (g_seenAnim == &state && g_seenMorphs == &where) ? 1.0f : 0.0f, 1.0f);
        for (int i = 0; i < 4; ++i) {           // alpha 0 -> the skinned position, alpha 1 -> unchanged
            const float* s = &g_vectorBuffer[i].X;
            const float* b = &before[i].X;
            const float* got = &current[i].X;
            for (int c = 0; c < 4; ++c) Expect("blend", pass, got[c], s[c] + (b[c] - s[c]) * alphas[i]);
        }
        Expect("alpha0", pass, current[0].X, g_vectorBuffer[0].X);
        Expect("alpha1", pass, current[1].Y, before[1].Y);
    }
    Expect("calc calls", 0, (float)g_calcCalls, 2.0f);
    printf(g_bad ? "00ae1320_TEST FAIL\n" : "00ae1320_TEST PASS\n");
    return g_bad ? 1 : 0;
}