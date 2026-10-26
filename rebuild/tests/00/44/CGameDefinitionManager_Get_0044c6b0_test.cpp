// Test for 0x0044C6B0 CGameDefinitionManager::Get: returns the singleton the global points at.
#include <stdio.h>
class CGameDefinitionManager;
CGameDefinitionManager* g_pGameDefinitionManager;
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    int Id;
};
int main()
{
    CGameDefinitionManager a, b; a.Id = 1; b.Id = 2;
    g_pGameDefinitionManager = &a;
    if (&CGameDefinitionManager::Get() != &a || CGameDefinitionManager::Get().Id != 1) { printf("0044c6b0_TEST FAIL\n"); return 1; }
    g_pGameDefinitionManager = &b;
    if (&CGameDefinitionManager::Get() != &b) { printf("0044c6b0_TEST FAIL reread\n"); return 1; }
    printf("0044c6b0_TEST PASS\n");
    return 0;
}
