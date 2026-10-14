#include <cstdio>
static int g_n = 0; static long g_pass[4];
struct CEngineSubPrimitiveBase { long m_a; unsigned long m_flags; };
struct CEnginePrimitiveManagerAnimatedMeshesBase { void RenderSubPrimitive(CEngineSubPrimitiveBase&, long pass) { g_pass[g_n++] = pass; } };
struct CEnginePrimitiveManager2DAnimatedMeshes : CEnginePrimitiveManagerAnimatedMeshesBase { void RenderSubPrimitive(CEngineSubPrimitiveBase& s, long pass) { if (s.m_flags & 1) CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(s, 1); CEnginePrimitiveManagerAnimatedMeshesBase::RenderSubPrimitive(s, pass); } };
int main() {
    CEnginePrimitiveManager2DAnimatedMeshes m; CEngineSubPrimitiveBase s = { 0, 0 }; m.RenderSubPrimitive(s, 3); if (g_n != 1 || g_pass[0] != 3) { std::printf("00bd7a00_TEST FAIL\n"); return 1; }
    s.m_flags = 1; m.RenderSubPrimitive(s, 2); if (g_n != 3 || g_pass[1] != 1 || g_pass[2] != 2) { std::printf("00bd7a00_TEST FAIL\n"); return 1; }
    std::printf("00bd7a00_TEST PASS\n"); return 0;
}