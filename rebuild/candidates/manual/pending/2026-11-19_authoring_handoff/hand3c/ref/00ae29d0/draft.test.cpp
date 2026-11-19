// Behaviour test for CParticleProgram::SetupSimulation (0x00AE29D0): CopyFrom failure returns false with nothing else run; otherwise the
// copy is skinned with animated * bone-space matrices, constraints run with the context step count / delta, LastPositions
// copies the result;
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
struct C3DVector { float X, Y, Z; };
struct ALIGN16 C4DVector { float X, Y, Z, W; };
static bool g_SSEAvailable;
struct ALIGN16 CMatrix4x4 {
    float Data[4][4];
    void AlignedPreScale(const C3DVector& scale);
    void AlignedPreScaleTransposed(const C3DVector& scale);
    void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b)   // the header's inline member (SSE kernel / x87 loop)
    {
        if (g_SSEAvailable) {
            const CMatrix4x4* pa = &a;
            const CMatrix4x4* pb = &b;
            CMatrix4x4* pdst = this;
            GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
        } else {
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) {
                    float v = a.Data[r][0] * b.Data[0][c];
                    v += a.Data[r][1] * b.Data[1][c];
                    v += a.Data[r][2] * b.Data[2][c];
                    v += a.Data[r][3] * b.Data[3][c];
                    Data[r][c] = v;
                }
        }
    }
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
struct CParticleSimulation {
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
struct C3DMesh2 { unsigned long BoneCount; CMatrix4x4* BoneSpaceTransforms; };
struct C3DAnimationBlendState { int Tag; };
static int g_calcCalls;
struct C3DAnimationManager {
    void CalcMeshTransformations(const C3DAnimationBlendState&, const C3DVector*, const C3DMesh2& mesh, CMatrix4x4* matrices, unsigned long count, bool world)
    {
        ++g_calcCalls;
        if (!world || count != mesh.BoneCount) ++g_bad;
        for (unsigned long i = 0; i < count; ++i) Fill(matrices[i], 20 + (int)i * 3);
    }
};
static ALIGN16 CMatrix4x4 g_matrixBuffer[8];
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
struct CParticleContext {
    long Steps; float Delta;
    long GetStepCount() const { return Steps; }
    float GetTimeDelta() const { return Delta; }
};
static long g_steps = -1; static float g_delta = -1.0f; static int g_constraintCalls;
struct CParticleProgram {
    CParticleSimulation InitialSimulation;
    static CMatrix4x4* GetMatrixBuffer(unsigned long) { return g_matrixBuffer; }
    static void ApplyConstraints(CParticleSimulation& simulation, long steps, float delta)
    {
        ++g_constraintCalls; g_steps = steps; g_delta = delta;
        simulation.Positions[0].X += 1.0f;      // visible in LastPositions only if the copy follows the constraints
    }
    bool SetupSimulation(CParticleSimulation& simulation, const C3DAnimationBlendState& blend_state, C3DAnimationManager& animation_manager,
                         const C3DVector* position, const C3DMesh2& mesh, const CParticleContext& context) const
    {
        if (!simulation.CopyFrom(InitialSimulation))
            return false;
        unsigned long bone_count = mesh.BoneCount;
        CMatrix4x4* matrices = GetMatrixBuffer(bone_count);
        animation_manager.CalcMeshTransformations(blend_state, position, mesh, matrices, bone_count, true);
        for (unsigned long i = 0; i < bone_count; ++i) {
            const CMatrix4x4& inverse = mesh.BoneSpaceTransforms[i];
            ALIGN16 CMatrix4x4 bone = matrices[i];
            matrices[i].AlignedSetMultiply(bone, inverse);
        }
        TransformVertices(simulation.Positions, InitialSimulation.Positions, simulation.Size, matrices);
        ApplyConstraints(simulation, context.GetStepCount(), context.GetTimeDelta());
        memcpy(simulation.LastPositions, simulation.Positions, simulation.Size * sizeof(C4DVector));
        return true;
    }
};
int main()
{
    for (int pass = 0; pass < 3; ++pass) {
        g_SSEAvailable = pass == 0;
        static ALIGN16 CMatrix4x4 inverse[2];
        Fill(inverse[0], 2); Fill(inverse[1], 9);
        C3DMesh2 mesh = { 2, inverse };
        static ALIGN16 C4DVector initial[3], positions[3], last[3];
        for (int i = 0; i < 3; ++i) {
            initial[i].X = Dy(i * 2); initial[i].Y = Dy(i * 2 + 1); initial[i].Z = Dy(i + 7); initial[i].W = 1.0f;
            positions[i].X = positions[i].Y = positions[i].Z = positions[i].W = 0.0f;
            last[i] = positions[i];
        }
        CParticleProgram program;
        program.InitialSimulation.Size = 3; program.InitialSimulation.Positions = initial;
        CParticleSimulation sim;
        sim.Size = 0; sim.Positions = positions; sim.LastPositions = last; sim.CopyOk = pass != 2;
        C3DAnimationBlendState state = { 0 };
        C3DAnimationManager manager;
        CParticleContext context = { 5, 0.25f };
        C3DVector where = { 0.0f, 0.0f, 0.0f };
        const int calls = g_constraintCalls;
        const bool ok = program.SetupSimulation(sim, state, manager, &where, mesh, context);
        if (pass == 2) {                         // CopyFrom failed: nothing else runs
            Expect("result", pass, ok ? 1.0f : 0.0f, 0.0f);
            Expect("calls", pass, (float)g_constraintCalls, (float)calls);
            continue;
        }
        Expect("result", pass, ok ? 1.0f : 0.0f, 1.0f);
        Expect("steps", pass, (float)g_steps, 5.0f);
        Expect("delta", pass, g_delta, 0.25f);
        positions[0].X -= 1.0f;                  // undo the constraint marker before checking the skinning
        ExpectSkinned(pass, positions, initial, 3, mesh);
        positions[0].X += 1.0f;
        for (int i = 0; i < 3; ++i) Expect("last", pass, last[i].X, positions[i].X);
    }
    printf(g_bad ? "00ae29d0_TEST FAIL\n" : "00ae29d0_TEST PASS\n");
    return g_bad ? 1 : 0;
}
