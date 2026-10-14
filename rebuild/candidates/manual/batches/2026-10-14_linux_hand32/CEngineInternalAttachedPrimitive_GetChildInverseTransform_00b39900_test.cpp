#include <cstdio>
struct CMatrix3x4 { float m_rows[12]; }; static int g_calls = 0; static bool g_result = true;
struct CAttached { unsigned char _pad[0x7c]; long m_childType; unsigned char _pad80[0x28]; CMatrix3x4 m_inv; bool m_valid;
    bool GetChildInverseTransformStaticMesh(CMatrix3x4* m) { ++g_calls; m->m_rows[0] = 5; return g_result; }
    bool GetChildInverseTransform() { if (m_valid) return true; bool valid = false; switch (m_childType) { case 1: valid = GetChildInverseTransformStaticMesh(&m_inv); break; } m_valid = valid; return valid; } };
int main() { CAttached a; a.m_valid = false; a.m_childType = 2; if (a.GetChildInverseTransform() || g_calls || a.m_valid) { std::printf("00b39900_TEST FAIL\n"); return 1; } a.m_childType = 1; if (!a.GetChildInverseTransform() || g_calls != 1 || !a.m_valid || a.m_inv.m_rows[0] != 5) { std::printf("00b39900_TEST FAIL\n"); return 1; }
    if (!a.GetChildInverseTransform() || g_calls != 1) { std::printf("00b39900_TEST FAIL\n"); return 1; } a.m_valid = false; g_result = false; if (a.GetChildInverseTransform() || a.m_valid) { std::printf("00b39900_TEST FAIL\n"); return 1; } std::printf("00b39900_TEST PASS\n"); return 0; }
