// Test for 0x00415FBC CMainGameComponent::PeekDefinitionManager: forwards to the CGameDefinitionManager singleton.
#include <stdio.h>
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    int Id;
};
static CGameDefinitionManager* s_p;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return *s_p; }
class CMainGameComponent {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;
    int Dummy;
};
int main()
{
    CGameDefinitionManager a, b; a.Id = 1; b.Id = 2;
    CMainGameComponent c; c.Dummy = 0;
    const CMainGameComponent& rc = c;
    s_p = &a;
    if (&rc.PeekDefinitionManager() != &a) { printf("00415fbc_TEST FAIL\n"); return 1; }
    s_p = &b;
    if (rc.PeekDefinitionManager().Id != 2) { printf("00415fbc_TEST FAIL reread\n"); return 1; }
    printf("00415fbc_TEST PASS\n");
    return 0;
}
