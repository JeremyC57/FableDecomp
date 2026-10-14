#include <cstdio>
struct SHandle_00b32f20 { long m_id; long m_tag; }; static long g_a = 0, g_k = 0;
struct CEnginePrimitiveManagerStaticMeshesBase { SHandle_00b32f20 AddStaticMesh(long a, long b, long c, long d, long e, long f, long g, long h, long i, long j, long k) { g_a = a; g_k = k; SHandle_00b32f20 r = { a + b + c + d + e + f + g + h + i + j + k, k }; return r; } };
struct CEnginePrimitiveRenderer { unsigned char _pad[0x280]; CEnginePrimitiveManagerStaticMeshesBase* m_pStaticMeshes;
    SHandle_00b32f20 AddStaticMesh(long a, long b, long c, long d, long e, long f, long g, long h, long i, long j, long k) { return m_pStaticMeshes->AddStaticMesh(a, b, c, d, e, f, g, h, i, j, k); } };
int main() {
    CEnginePrimitiveRenderer* r = (CEnginePrimitiveRenderer*)new char[sizeof(CEnginePrimitiveRenderer)]; CEnginePrimitiveManagerStaticMeshesBase mgr; r->m_pStaticMeshes = &mgr;
    SHandle_00b32f20 h = r->AddStaticMesh(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11); if (h.m_id != 66 || h.m_tag != 11 || g_a != 1 || g_k != 11) { std::printf("00b32f20_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b32f20_TEST PASS\n"); return 0;
}
