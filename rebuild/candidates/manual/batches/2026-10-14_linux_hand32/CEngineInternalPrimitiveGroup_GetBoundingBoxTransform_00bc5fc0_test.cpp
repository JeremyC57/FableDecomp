#include <cstdio>
struct CMatrix3x4 { float m_rows[12]; }; static bool g_have = true; static int g_calc = 0;
struct C3DBoundingBox { float Min[3]; float Max[3]; void CalculateMatrix(CMatrix3x4* m) const { ++g_calc; m->m_rows[0] = Min[0]; } };
struct CGroup { virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& b) { b.Min[0] = 9; return g_have; }
    bool GetBoundingBoxTransform(CMatrix3x4& t) { C3DBoundingBox box; if (GetBoundingBoxWorldSpace(box)) { box.CalculateMatrix(&t); return true; } return false; } };
int main() { CGroup g; CMatrix3x4 m; m.m_rows[0] = 0; if (!g.GetBoundingBoxTransform(m) || g_calc != 1 || m.m_rows[0] != 9) { std::printf("00bc5fc0_TEST FAIL\n"); return 1; } g_have = false; if (g.GetBoundingBoxTransform(m) || g_calc != 1) { std::printf("00bc5fc0_TEST FAIL\n"); return 1; } std::printf("00bc5fc0_TEST PASS\n"); return 0; }
