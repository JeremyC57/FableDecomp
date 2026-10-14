#include <cstdio>
struct CMatrix3x4 { float m_rows[12]; };
static bool g_clip = true; static const void* g_box = 0; static CMatrix3x4* g_matrix = 0;
struct C3DBoundingBox { void CalculateMatrix(CMatrix3x4* m) const { g_box = this; m->m_rows[0] = 7.0f; } };
struct CEngineCamera { bool ClipBox(CMatrix3x4* m) { g_matrix = m; return g_clip && m->m_rows[0] == 7.0f; } };
static CEngineCamera g_cam; static CEngineCamera* g_pEngineCamera = &g_cam;
struct CEngine { bool ClipBox(const C3DBoundingBox& box) { CMatrix3x4 matrix; box.CalculateMatrix(&matrix); return g_pEngineCamera->ClipBox(&matrix); } };
int main() { C3DBoundingBox box; CEngine e; if (!e.ClipBox(box) || g_box != &box || !g_matrix) { std::printf("00b242a0_TEST FAIL\n"); return 1; } g_clip = false; if (e.ClipBox(box)) { std::printf("00b242a0_TEST FAIL\n"); return 1; } std::printf("00b242a0_TEST PASS\n"); return 0; }
