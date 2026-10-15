// Behaviour test for CEnginePrimitiveManager2DStaticMeshes::RenderSubPrimitive @ 0x00BA77B0.
#include <cstdio>
static int g_log[16]; static long g_args[16]; static int g_n = 0;
static void Log(int what, long arg) { g_log[g_n] = what; g_args[g_n] = arg; ++g_n; }
struct CEngineSubPrimitiveBase;
struct CEngineClothSimulationStaticMesh { void Render(void*, CEngineSubPrimitiveBase&, long pass) { Log(3, pass); } };
static CEngineClothSimulationStaticMesh g_sim; static long g_wantIndex = 7;
struct CEnginePrimitiveSimulationInfo { CEngineClothSimulationStaticMesh* FindSimulation(long i) { Log(2, i); return i == g_wantIndex ? &g_sim : 0; } };
struct CEngineInternalPrimitiveStaticMesh { CEnginePrimitiveSimulationInfo* m_pSimulationInfo; };
struct SMeshSlot { long m_index; };
struct CMeshRenderer { SMeshSlot** m_ppCurrent; long GetCurrentMeshIndex() const { return (*m_ppCurrent)->m_index; } };
struct CEngineSubPrimitiveBase { unsigned char m_flags; CEngineInternalPrimitiveStaticMesh* m_pPrimitive; CMeshRenderer* m_pRenderer; bool m_cloth; };
struct CEnginePrimitiveManagerStaticMeshesBase {
    void RenderMeshPrimitive(CEngineSubPrimitiveBase&, long pass) { Log(4, pass); }
    void RenderSubPrimitive(CEngineSubPrimitiveBase&, long pass) { Log(1, pass); }
};
struct CEnginePrimitiveManager2DStaticMeshes : CEnginePrimitiveManagerStaticMeshesBase {
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass)
    {
        if (sub.m_flags & 1) CEnginePrimitiveManagerStaticMeshesBase::RenderSubPrimitive(sub, 1);
        if (sub.m_cloth) {
            CEnginePrimitiveSimulationInfo* info = sub.m_pPrimitive->m_pSimulationInfo;
            if (info) {
                CMeshRenderer* renderer = sub.m_pRenderer;
                CEngineClothSimulationStaticMesh* sim = info->FindSimulation(renderer->GetCurrentMeshIndex());
                if (sim) sim->Render(renderer, sub, pass);
            }
        } else RenderMeshPrimitive(sub, pass);
    }
};
int main()
{
    CEnginePrimitiveManager2DStaticMeshes m; CEnginePrimitiveSimulationInfo info; CEngineInternalPrimitiveStaticMesh prim = { &info };
    SMeshSlot slot = { 7 }; SMeshSlot* pslot = &slot; CMeshRenderer r = { &pslot };
    CEngineSubPrimitiveBase s = { 1, &prim, &r, false };
    m.RenderSubPrimitive(s, 5);
    int ok = g_n == 2 && g_log[0] == 1 && g_args[0] == 1 && g_log[1] == 4 && g_args[1] == 5;
    g_n = 0; s.m_flags = 0; s.m_cloth = true; m.RenderSubPrimitive(s, 6);
    ok = ok && g_n == 2 && g_log[0] == 2 && g_args[0] == 7 && g_log[1] == 3 && g_args[1] == 6;
    g_n = 0; slot.m_index = 8; m.RenderSubPrimitive(s, 6);
    ok = ok && g_n == 1 && g_log[0] == 2;
    g_n = 0; prim.m_pSimulationInfo = 0; m.RenderSubPrimitive(s, 6);
    ok = ok && g_n == 0;
    if (!ok) { std::printf("00ba77b0_TEST FAIL\n"); return 1; }
    std::printf("00ba77b0_TEST PASS\n");
    return 0;
}