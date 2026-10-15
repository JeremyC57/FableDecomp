// Behaviour test for 0x00B3B7F0 (see src/00b3b7f0.cpp): ProcessLoading fanning out to the banks and managers.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b3b7f0_TEST FAIL\n"); return 1; } } while (0)

struct CMeshDataBank { int updates; int queues; bool flag; double until; void UpdateLoading() { ++updates; } void ProcessLoadingQueue(bool f, double u) { ++queues; flag = f; until = u; } };
struct CGraphicBankManager { int calls; double until; void ProcessLoading(bool, double u) { ++calls; until = u; } };
struct CSystemManager { CGraphicBankManager* GraphicBankManager; };
struct CSurface { int calls; void ProcessLoading(bool, double) { ++calls; } };
static CGraphicBankManager g_graphic = { 0, 0 };
static CSystemManager g_system = { &g_graphic };
static CSystemManager* GFGetSystemManager() { return &g_system; }
static CSurface* g_pSurface = 0;
struct CEngineResourceManager {
    CMeshDataBank* MeshBanks[3];
    void ProcessLoading(bool flag, double until)
    {
        int i;
        for (i = 0; i < 3; ++i) if (MeshBanks[i]) MeshBanks[i]->UpdateLoading();
        CSystemManager* system = GFGetSystemManager();
        system->GraphicBankManager->ProcessLoading(flag, until);
        for (i = 0; i < 3; ++i) if (MeshBanks[i]) MeshBanks[i]->ProcessLoadingQueue(flag, until);
        if (g_pSurface) g_pSurface->ProcessLoading(flag, until);
    }
};
int main()
{
    CMeshDataBank a = { 0, 0, false, 0 }, c = { 0, 0, false, 0 };
    CEngineResourceManager m; m.MeshBanks[0] = &a; m.MeshBanks[1] = 0; m.MeshBanks[2] = &c;
    m.ProcessLoading(true, 2.5);
    CHECK(a.updates == 1 && c.updates == 1 && a.queues == 1 && c.queues == 1 && a.flag && c.until == 2.5 && g_graphic.calls == 1 && g_graphic.until == 2.5);
    CSurface s = { 0 }; g_pSurface = &s;
    m.ProcessLoading(false, 3.0);
    CHECK(s.calls == 1 && a.updates == 2 && !a.flag && g_graphic.calls == 2);
    std::printf("00b3b7f0_TEST PASS\n");
    return 0;
}