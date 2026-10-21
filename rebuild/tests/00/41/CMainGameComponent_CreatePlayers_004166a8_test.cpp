#include <cstdio>
#include "engine/CNetworkClient.h"
#include "engine/CPlayerManager.h"
struct CGameDefinitionManager_Methods {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};
struct CPlayerManager_Methods : CPlayerManager {
    void CreatePlayers();                                // 0x0044A530
};
struct CNetworkClient_Methods : CNetworkClient {
    bool InitialiseAsLocal(CMainGameComponent* component);  // 0x004AE940
    bool IsValid() const;                                // 0x0099A330 (CInitBaseClass)
    void Uninitialise();                                 // 0x004AE990
};
struct CMainGameComponent {
    unsigned char            _pad00[0x1c];
    CPlayerManager_Methods*  PPlayerManager;             // +0x1c scoped_ptr<CPlayerManager>
    unsigned char            _pad20[0x13ab8 - 0x20];
    CNetworkClient_Methods   Client;                     // +0x13ab8
protected:
    void CreatePlayers();
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void CMainGameComponent::CreatePlayers()
{
    const CGameDefinitionManager& definitions = CGameDefinitionManager_Methods::Get();
    PPlayerManager->CreatePlayers();
    if (!Client.InitialiseAsLocal(this) && Client.IsValid())
        Client.Uninitialise();
}

static int g_log[8]; static int g_n = 0; static bool g_init = false, g_valid = false;
static int g_dummy;
CGameDefinitionManager& __fastcall CGameDefinitionManager_Methods::Get() { g_log[g_n++] = 1; return *(CGameDefinitionManager*)&g_dummy; }
void CPlayerManager_Methods::CreatePlayers() { g_log[g_n++] = 2; }
bool CNetworkClient_Methods::InitialiseAsLocal(CMainGameComponent*) { g_log[g_n++] = 3; return g_init; }
bool CNetworkClient_Methods::IsValid() const { g_log[g_n++] = 4; return g_valid; }
void CNetworkClient_Methods::Uninitialise() { g_log[g_n++] = 5; }
static CMainGameComponent g_c;
static CPlayerManager_Methods g_pm;
struct CMainGameComponent_Test { static void Run() { g_c.PPlayerManager = &g_pm; g_c.CreatePlayers(); } };
static bool Case(bool init, bool valid, int n, int last)
{
    g_n = 0; g_init = init; g_valid = valid; CMainGameComponent_Test::Run();
    return g_n == n && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[n - 1] == last;
}
int main()
{
    if (Case(true, true, 3, 3) && Case(false, false, 4, 4) && Case(false, true, 5, 5)) std::printf("004166a8_TEST PASS\n");
    return 0;
}