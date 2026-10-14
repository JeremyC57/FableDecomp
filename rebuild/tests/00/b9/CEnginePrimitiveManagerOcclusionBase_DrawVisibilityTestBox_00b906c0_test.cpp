#include <cstdio>
#include <string>
static std::string g_seq; static void* g_mesh = 0; static void* g_box = 0; static void* g_colour = 0;
struct CEngineRenderer_00b906c0 { void BeginUntextured() { g_seq += 'B'; } void EndUntextured(void* c) { g_seq += 'E'; g_colour = c; } };
static CEngineRenderer_00b906c0 g_r; static CEngineRenderer_00b906c0* g_pRenderer_01436e5c = &g_r;
struct COcclusionData_00b906c0 { unsigned char _pad[0x94]; void* m_pTestMesh; };
struct CEnginePrimitiveManagerOcclusionBase { unsigned char _pad[0x14]; COcclusionData_00b906c0* m_pOcclusionData; void RenderUntexturedStaticMeshPrimitive(void* mesh, void* box) { g_seq += 'R'; g_mesh = mesh; g_box = box; }
    void DrawVisibilityTestBox(void* box, void* colour) { g_pRenderer_01436e5c->BeginUntextured(); RenderUntexturedStaticMeshPrimitive(m_pOcclusionData->m_pTestMesh, box); g_pRenderer_01436e5c->EndUntextured(colour); } };
int main() {
    CEnginePrimitiveManagerOcclusionBase* m = (CEnginePrimitiveManagerOcclusionBase*)new char[sizeof(CEnginePrimitiveManagerOcclusionBase)]; COcclusionData_00b906c0 d; int mesh = 0, box = 0, colour = 0; d.m_pTestMesh = &mesh; m->m_pOcclusionData = &d;
    m->DrawVisibilityTestBox(&box, &colour); if (g_seq != "BRE" || g_mesh != &mesh || g_box != &box || g_colour != &colour) { std::printf("00b906c0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b906c0_TEST PASS\n"); return 0;
}