// Behaviour test for 0x00B80CE0 CEngineClothSimulationMeshBase::GetDeltaTransform: the row's defines, includes and body verbatim over the
// shared headers, run on a zeroed object image (the class is abstract) holding CurrentTransform at +0x18 and LastTransform at +0x48.
// dest must equal LastTransform * inverse(CurrentTransform) (row-vector convention: rotation rows, translation row E4k), checked
// element by element against an independent double-precision computation, for two non-symmetric transforms; and for Current == Last
// the delta must be the identity.
#include <stdio.h>
#include <string.h>
#include <math.h>
// ---- row body ----
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#include "engine/CMatrix3x4.h"
#include "engine/CEngineClothSimulationMeshBase.h"

void CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& dest) const
{
    dest.SetInverse(CurrentTransform);
    dest.PreMultiply(LastTransform);
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static void Expected(const float* c, const float* l, double* out)    // out = l * inverse(c), 4x3 row layout
{
    double a = c[0], b = c[1], cc = c[2], d = c[3], e = c[4], f = c[5], g = c[6], h = c[7], i = c[8];
    double det = a * (e * i - f * h) - b * (d * i - f * g) + cc * (d * h - e * g);
    double inv[12];
    inv[0] = (e * i - f * h) / det; inv[1] = (cc * h - b * i) / det; inv[2] = (b * f - cc * e) / det;
    inv[3] = (f * g - d * i) / det; inv[4] = (a * i - cc * g) / det; inv[5] = (cc * d - a * f) / det;
    inv[6] = (d * h - e * g) / det; inv[7] = (b * g - a * h) / det; inv[8] = (a * e - b * d) / det;
    for (int k = 0; k < 3; ++k) inv[9 + k] = -(c[9] * inv[k] + c[10] * inv[3 + k] + c[11] * inv[6 + k]);
    for (int r = 0; r < 4; ++r)
        for (int k = 0; k < 3; ++k) {
            double s = l[r * 3] * inv[k] + l[r * 3 + 1] * inv[3 + k] + l[r * 3 + 2] * inv[6 + k];
            if (r == 3) s += inv[9 + k];
            out[r * 3 + k] = s;
        }
}
static void Run(const float* cur, const float* last, const char* what)
{
    static unsigned char image[0x100];
    memset(image, 0, sizeof image);
    memcpy(image + 0x18, cur, 48); memcpy(image + 0x48, last, 48);
    const CEngineClothSimulationMeshBase* cloth = (const CEngineClothSimulationMeshBase*)image;
    CMatrix3x4 dest;
    memset(&dest, 0x55, sizeof dest);
    cloth->CEngineClothSimulationMeshBase::GetDeltaTransform(dest);
    double want[12]; Expected(cur, last, want);
    const float* got = (const float*)&dest;
    for (int k = 0; k < 12; ++k)
        if (fabs(got[k] - want[k]) > 1e-4 * (1.0 + fabs(want[k]))) { printf("element %d: %f v %f\n", k, got[k], want[k]); check(0, what); }
    check(memcmp(image + 0x18, cur, 48) == 0 && memcmp(image + 0x48, last, 48) == 0, "transforms unchanged");
}
int main()
{
    check(sizeof(CMatrix3x4) == 48 && offsetof(CEngineClothSimulationMeshBase, CurrentTransform) == 0x18
          && offsetof(CEngineClothSimulationMeshBase, LastTransform) == 0x48, "layout");
    static const float cur1[12] = { 2.0f, 0.5f, -1.0f,  0.25f, 3.0f, 0.75f,  -0.5f, 1.5f, 4.0f,  10.0f, -20.0f, 30.0f };
    static const float last1[12] = { 1.0f, -2.0f, 0.5f,  3.0f, 0.25f, -1.5f,  0.75f, 2.5f, -3.0f,  -7.0f, 5.0f, 11.0f };
    static const float cur2[12] = { 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 2.0f,  4.0f, 8.0f, -16.0f };
    Run(cur1, last1, "general transforms");
    Run(cur2, last1, "rotation and scale");
    Run(last1, cur1, "swapped roles");
    // Current == Last: the delta is the identity
    {
        static unsigned char image[0x100];
        memcpy(image + 0x18, cur1, 48); memcpy(image + 0x48, cur1, 48);
        CMatrix3x4 dest;
        ((const CEngineClothSimulationMeshBase*)image)->CEngineClothSimulationMeshBase::GetDeltaTransform(dest);
        const float* got = (const float*)&dest;
        static const float id[12] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
        for (int k = 0; k < 12; ++k) check(fabs(got[k] - id[k]) < 1e-4, "same transform gives identity");
    }
    if (!g_fail) printf("00b80ce0_TEST PASS\n");
    return g_fail;
}