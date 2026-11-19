// Behaviour test for C3DAnimationManager::ApplyBoneMorphScaling (0x00A9D750), a model of the row over the shared kernel header:
// every bone matrix gets its morph vector applied to the 3x3 part, per column when `transpose` is set (AlignedPreScaleTransposed:
// column c times scale[c]) and per row otherwise (AlignedPreScale: row r times scale[r]). E14/E24/E34 and row 4 stay as they were.
// The SSE kernels and the x87 path are both run on dyadic inputs, so they agree bit for bit. g++ builds it with -DFABLE_PORTABLE,
// which selects the kernels' C++ expansion.
#include <stdio.h>
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
    void AlignedPreScale(const C3DVector& scale)
    {
        if (g_SSEAvailable) {
            C4DVector vect; vect.X = scale.X; vect.Y = scale.Y; vect.Z = scale.Z; vect.W = 0.0f;
            float* dest = &Data[0][0];
            C3DVector right = { Data[0][3], Data[1][3], Data[2][3] };
            GF_SSE_MATRIX_PRESCALE(dest, vect);
            Data[0][3] = right.X; Data[1][3] = right.Y; Data[2][3] = right.Z;
            return;
        }
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c) Data[r][c] *= (&scale.X)[r];
    }
    void AlignedPreScaleTransposed(const C3DVector& scale)
    {
        if (g_SSEAvailable) {
            C4DVector vect; vect.X = scale.X; vect.Y = scale.Y; vect.Z = scale.Z; vect.W = 0.0f;
            float* dest = &Data[0][0];
            C3DVector right = { Data[0][3], Data[1][3], Data[2][3] };
            GF_SSE_MATRIX_PRESCALE_TRANSPOSED(dest, vect);
            Data[0][3] = right.X; Data[1][3] = right.Y; Data[2][3] = right.Z;
            return;
        }
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c) Data[r][c] *= (&scale.X)[c];
    }
};
static void ApplyBoneMorphScaling(const C3DVector* bone_morphs, CMatrix4x4* dest, unsigned long no_bones, bool transpose)
{
    if (transpose) {
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone) dest[c_bone].AlignedPreScaleTransposed(bone_morphs[c_bone]);
    } else {
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone) dest[c_bone].AlignedPreScale(bone_morphs[c_bone]);
    }
}
static float In(int bone, int i) { return (float)(((i * 7 + bone * 3) % 13) - 6) * 0.25f; }
int main()
{
    int bad = 0;
    const C3DVector morphs[3] = { { 2.0f, -0.5f, 4.0f }, { 0.25f, 1.0f, -2.0f }, { -1.0f, 8.0f, 0.5f } };
    for (int pass = 0; pass < 4; ++pass) {
        g_SSEAvailable = (pass & 1) == 0;
        bool transpose = (pass & 2) != 0;
        ALIGN16 CMatrix4x4 m[4];
        for (int b = 0; b < 4; ++b)
            for (int i = 0; i < 16; ++i) m[b].Data[i / 4][i % 4] = In(b, i);
        ApplyBoneMorphScaling(morphs, m, 3, transpose);
        for (int b = 0; b < 4; ++b)
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) {
                    float in = In(b, r * 4 + c);
                    float want = in;
                    if (b < 3 && r < 3 && c < 3) want = in * (&morphs[b].X)[transpose ? c : r];
                    if (m[b].Data[r][c] != want) {
                        printf("pass %d bone %d [%d][%d] %g != %g\n", pass, b, r, c, m[b].Data[r][c], want); ++bad;
                    }
                }
    }
    printf(bad ? "00a9d750_TEST FAIL\n" : "00a9d750_TEST PASS\n");
    return bad ? 1 : 0;
}
