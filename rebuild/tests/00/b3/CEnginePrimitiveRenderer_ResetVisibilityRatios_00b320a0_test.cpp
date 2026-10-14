#include <cstdio>
static int g_enum = 0, g_ctor = 0, g_dtor = 0; static void* g_arg = (void*)1;
struct CBase { virtual void v00() { } CBase() { ++g_ctor; } ~CBase() { ++g_dtor; } };
struct CEngineSceneEnumeratorResetVisibilityRatio : CBase { void* m_argument; CEngineSceneEnumeratorResetVisibilityRatio(void* a) : CBase(), m_argument(a) { } };
struct CEngineSceneGrid_00b320a0 { void Enumerate(CEngineSceneEnumeratorResetVisibilityRatio& e) { ++g_enum; g_arg = e.m_argument; } };
struct CEnginePrimitiveRenderer { unsigned char _pad[0x338]; CEngineSceneGrid_00b320a0** m_pFirstGrid; CEngineSceneGrid_00b320a0** m_pLastGrid;
    void ResetVisibilityRatios() { CEngineSceneEnumeratorResetVisibilityRatio enumerator(0); for (CEngineSceneGrid_00b320a0** grid = m_pFirstGrid; grid != m_pLastGrid; ++grid) (*grid)->Enumerate(enumerator); } };
int main() {
    CEnginePrimitiveRenderer* r = (CEnginePrimitiveRenderer*)new char[sizeof(CEnginePrimitiveRenderer)]; CEngineSceneGrid_00b320a0 g1, g2; CEngineSceneGrid_00b320a0* grids[2] = { &g1, &g2 };
    r->m_pFirstGrid = grids; r->m_pLastGrid = grids + 2; r->ResetVisibilityRatios(); if (g_enum != 2 || g_arg != 0 || g_ctor != 1 || g_dtor != 1) { std::printf("00b320a0_TEST FAIL\n"); return 1; }
    r->m_pLastGrid = grids; r->ResetVisibilityRatios(); if (g_enum != 2 || g_dtor != 2) { std::printf("00b320a0_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b320a0_TEST PASS\n"); return 0;
}