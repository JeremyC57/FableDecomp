#include <cstdio>
struct CCamera { int x; }; static CCamera* g_observer = 0; static bool g_processed = false; static int g_order = 0, g_oa = 0, g_op = 0;
struct CEngineCamera { void AddExclusiveObserver(CCamera* c) { g_observer = c; g_oa = ++g_order; } void Process(bool f) { g_processed = f; g_op = ++g_order; } };
static CEngineCamera g_cam; static CEngineCamera* g_pEngineCamera = &g_cam;
struct CEngine { unsigned char _pad[0x14]; CCamera* CurrentCamera; void SetCamera(CCamera* c) { g_pEngineCamera->AddExclusiveObserver(c); g_pEngineCamera->Process(true); CurrentCamera = c; } };
int main() { CCamera c; CEngine e; e.CurrentCamera = 0; e.SetCamera(&c); if (g_observer != &c || !g_processed || e.CurrentCamera != &c || g_oa != 1 || g_op != 2) { std::printf("00b23b50_TEST FAIL\n"); return 1; } std::printf("00b23b50_TEST PASS\n"); return 0; }
