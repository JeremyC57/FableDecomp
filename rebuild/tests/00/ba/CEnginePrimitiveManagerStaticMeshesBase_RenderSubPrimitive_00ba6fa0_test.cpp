#include <cstdio>
static int g_mesh = 0, g_cloth = 0; static long g_meshPass = -1, g_clothPass = -1, g_findIndex = -1; static void* g_clothRenderer = 0;
struct CEngineClothSimulationStaticMesh; class CEngineSubPrimitiveBase;
class CEnginePrimitiveSimulationInfo { public: CEngineClothSimulationStaticMesh* m_pSim; CEngineClothSimulationStaticMesh* FindSimulation(long index) { g_findIndex = index; return m_pSim; } };
struct CEngineClothSimulationStaticMesh { void Render(void* renderer, CEngineSubPrimitiveBase&, long pass) { ++g_cloth; g_clothPass = pass; g_clothRenderer = renderer; } };
struct CEngineInternalPrimitiveStaticMesh_00ba6fa0 { unsigned char _pad[0x64]; CEnginePrimitiveSimulationInfo* m_pSimulationInfo; };
struct SMeshSlot_00ba6fa0 { void* m_a; void* m_b; void* m_c; long m_index; };
struct CMeshRenderer_00ba6fa0 { unsigned char _pad[0x20]; SMeshSlot_00ba6fa0** m_ppCurrent; long GetCurrentMeshIndex() const { return (*m_ppCurrent)->m_index; } };
class CEngineSubPrimitiveBase { public: unsigned char _pad0[0xc]; CEngineInternalPrimitiveStaticMesh_00ba6fa0* m_pPrimitive; unsigned char _pad10[8]; CMeshRenderer_00ba6fa0* m_pRenderer; unsigned char _pad1c[4]; bool m_cloth; };
struct CEnginePrimitiveManagerStaticMeshesBase { void RenderMeshPrimitive(CEngineSubPrimitiveBase&, long pass) { ++g_mesh; g_meshPass = pass; }
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass) { if (sub.m_cloth) { CEnginePrimitiveSimulationInfo* info = sub.m_pPrimitive->m_pSimulationInfo; if (info) { CMeshRenderer_00ba6fa0* renderer = sub.m_pRenderer; CEngineClothSimulationStaticMesh* simulation = info->FindSimulation(renderer->GetCurrentMeshIndex()); if (simulation) simulation->Render(renderer, sub, pass); } } else { RenderMeshPrimitive(sub, pass); } } };
int main() {
    CEnginePrimitiveManagerStaticMeshesBase m; CEngineInternalPrimitiveStaticMesh_00ba6fa0 prim; CEnginePrimitiveSimulationInfo info; CEngineClothSimulationStaticMesh sim; SMeshSlot_00ba6fa0 slot = { 0, 0, 0, 4 }; SMeshSlot_00ba6fa0* pslot = &slot; CMeshRenderer_00ba6fa0 r; r.m_ppCurrent = &pslot;
    CEngineSubPrimitiveBase s; s.m_pPrimitive = &prim; s.m_pRenderer = &r; s.m_cloth = false; prim.m_pSimulationInfo = &info; info.m_pSim = &sim;
    m.RenderSubPrimitive(s, 2); if (g_mesh != 1 || g_meshPass != 2 || g_cloth != 0) { std::printf("00ba6fa0_TEST FAIL\n"); return 1; }
    s.m_cloth = true; m.RenderSubPrimitive(s, 3); if (g_mesh != 1 || g_findIndex != 4 || g_cloth != 1 || g_clothPass != 3 || g_clothRenderer != &r) { std::printf("00ba6fa0_TEST FAIL\n"); return 1; }
    info.m_pSim = 0; m.RenderSubPrimitive(s, 3); if (g_cloth != 1) { std::printf("00ba6fa0_TEST FAIL\n"); return 1; }
    prim.m_pSimulationInfo = 0; m.RenderSubPrimitive(s, 3); if (g_cloth != 1 || g_mesh != 1) { std::printf("00ba6fa0_TEST FAIL\n"); return 1; }
    std::printf("00ba6fa0_TEST PASS\n"); return 0;
}