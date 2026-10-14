#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_n = 0; static long g_a[5];
struct CHeroLogBook_Retail { void AddLogEntry(long a, long b, long c, long d, long f) { g_n = 5; g_a[0] = a; g_a[1] = b; g_a[2] = c; g_a[3] = d; g_a[4] = f; } }; static CHeroLogBook_Retail g_book;
struct CPlayer_Retail { CHeroLogBook_Retail* m_pLogBook; }; static CPlayer_Retail g_player = { &g_book };
struct CPlayerManager_Retail { CPlayer_Retail* GetPlayer() { return &g_player; } }; static CPlayerManager_Retail g_pm;
struct CDisplayEngine_Retail; struct CWorld;
struct CGameScriptInterface { virtual void v000() { } CWorld* World; long m_08; CDisplayEngine_Retail* DisplayEngine; long m_10; CPlayerManager_Retail* PlayerManager;
    void AddLogBookEntry(long a, long b, long c, long d) const { CPlayer_Retail* player = PlayerManager->GetPlayer(); player->m_pLogBook->AddLogEntry(a, b, c, d, 1); } };
int main() {
    CGameScriptInterface i; i.PlayerManager = &g_pm; i.AddLogBookEntry(1, 2, 3, 4); if (g_n != 5 || g_a[0] != 1 || g_a[1] != 2 || g_a[2] != 3 || g_a[3] != 4 || g_a[4] != 1) { std::printf("0088fe00_TEST FAIL\n"); return 1; }
    std::printf("0088fe00_TEST PASS\n"); return 0;
}