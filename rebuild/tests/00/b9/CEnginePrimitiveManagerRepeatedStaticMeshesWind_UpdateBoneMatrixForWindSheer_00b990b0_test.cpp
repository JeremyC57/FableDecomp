// 00b990b0 behaviour test: CEnginePrimitiveManagerRepeatedStaticMeshes::UpdateBoneMatrixForWindSheer (match.cpp body, host-modelled)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };
struct SEngineSystem_01436e48 { C3DVector WindSheerHistory[256]; };
static SEngineSystem_01436e48 g_sys;
static SEngineSystem_01436e48* g_pEngineSystem_01436e48 = &g_sys;
class CEngineInternalPrimitiveRepeatedStaticMesh { public: float WindScaleFactor; };
struct CEnginePrimitiveManagerRepeatedStaticMeshes_Wind {
    void UpdateBoneMatrixForWindSheer(CEngineInternalPrimitiveRepeatedStaticMesh* primitive, C4DVector* matrices,
                                      unsigned char* delays, long count)
    {
        float u0 = 0.0f;
        C3DVector* history = g_pEngineSystem_01436e48->WindSheerHistory;
        for (long i = 0; i < count; ++i) {
            const C3DVector& wind = history[delays[i]];
            matrices[i].Z = wind.X * primitive->WindScaleFactor;
            matrices[i].W = wind.Y * primitive->WindScaleFactor;
        }
    }
};
int main()
{
    for (int i = 0; i < 256; ++i) { g_sys.WindSheerHistory[i].X = (float)i; g_sys.WindSheerHistory[i].Y = (float)(i * 2); g_sys.WindSheerHistory[i].Z = -1.0f; }
    CEngineInternalPrimitiveRepeatedStaticMesh prim = { 0.5f };
    C4DVector rows[7]; unsigned char delays[7] = { 0, 3, 255, 10, 1, 7, 200 };
    for (int i = 0; i < 7; ++i) { rows[i].X = 11.0f; rows[i].Y = 12.0f; rows[i].Z = 13.0f; rows[i].W = 14.0f; }
    CEnginePrimitiveManagerRepeatedStaticMeshes_Wind mgr;
    mgr.UpdateBoneMatrixForWindSheer(&prim, rows, delays, 6);
    for (int i = 0; i < 6; ++i) {
        CHECK(rows[i].Z == delays[i] * 0.5f);
        CHECK(rows[i].W == delays[i] * 2 * 0.5f);
        CHECK(rows[i].X == 11.0f && rows[i].Y == 12.0f);
    }
    CHECK(rows[6].Z == 13.0f && rows[6].W == 14.0f);
    mgr.UpdateBoneMatrixForWindSheer(&prim, rows, delays, 0);
    CHECK(rows[0].Z == 0.0f && rows[6].Z == 13.0f);
    if (!g_fail) printf("00b990b0_TEST PASS\n");
    return g_fail;
}