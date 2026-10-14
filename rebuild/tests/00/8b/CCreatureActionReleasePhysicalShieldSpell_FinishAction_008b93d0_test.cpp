#include <cstdio>
static int g_rel = 0, g_fin = 0; static bool g_have = true;
struct Mgr { void ReleaseScriptedControl() { ++g_rel; } }; static Mgr g_mgr;
struct Cam { virtual bool GetScriptedCameraManager(Mgr** pp) { *pp = &g_mgr; return g_have; } }; static Cam g_cam;
struct World { unsigned char pad[0x34]; Cam* cam; }; static World g_world;
struct Base { virtual void FinishAction() { ++g_fin; } World* GetWorld() { return &g_world; } };
struct Act : Base { void FinishAction() { Mgr* m; if (GetWorld()->cam->GetScriptedCameraManager(&m)) m->ReleaseScriptedControl(); Base::FinishAction(); } };
int main()
{
    g_world.cam = &g_cam; Act a; a.FinishAction(); g_have = false; a.FinishAction();
    if (g_rel != 1 || g_fin != 2) { std::printf("008b93d0_TEST FAIL\n"); return 1; }
    std::printf("008b93d0_TEST PASS\n");
    return 0;
}