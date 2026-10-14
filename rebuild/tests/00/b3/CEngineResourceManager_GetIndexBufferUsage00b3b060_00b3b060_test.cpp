#include <cstdio>
static unsigned long g_cost = 0;
struct CResourceOwner_01436e1c { unsigned long GetIndexBufferUsageCost() { return g_cost; } }; static CResourceOwner_01436e1c g_o; static CResourceOwner_01436e1c* g_pResourceOwner_01436e1c = &g_o;
struct CEngineResourceManager { static long GetIndexBufferUsage_00b3b060() { long usage = 8; if (g_pResourceOwner_01436e1c->GetIndexBufferUsageCost() < 0x44c) usage = 24; return usage; } };
int main() {
    g_cost = 0x44b; if (CEngineResourceManager::GetIndexBufferUsage_00b3b060() != 24) { std::printf("00b3b060_TEST FAIL\n"); return 1; }
    g_cost = 0x44c; if (CEngineResourceManager::GetIndexBufferUsage_00b3b060() != 8) { std::printf("00b3b060_TEST FAIL\n"); return 1; }
    g_cost = 0xffffffffUL; if (CEngineResourceManager::GetIndexBufferUsage_00b3b060() != 8) { std::printf("00b3b060_TEST FAIL\n"); return 1; }
    std::printf("00b3b060_TEST PASS\n"); return 0;
}