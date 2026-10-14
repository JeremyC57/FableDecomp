#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_mode = 0;
struct CPlayer_Retail { long GetCurrentMode() { return g_mode; } }; static CPlayer_Retail g_player;
struct CPlayerManager_Retail { CPlayer_Retail* GetPlayer() { return &g_player; } }; static CPlayerManager_Retail g_pm;
struct CDisplayEngine_Retail; struct CWorld;
struct CGameScriptInterface { virtual void v000() { } CWorld* World; long m_08; CDisplayEngine_Retail* DisplayEngine; long m_10; CPlayerManager_Retail* PlayerManager;
    bool IsHeroControlledByPlayer() const { switch (PlayerManager->GetPlayer()->GetCurrentMode()) { case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 17: case 18: case 21: case 23: case 26: case 27: case 29: case 30: case 32: case 36: case 37: case 41: return false; } return true; } };
int main() {
    CGameScriptInterface i; i.PlayerManager = &g_pm; static const int f[] = { 4,5,6,7,8,9,10,11,12,13,14,15,17,18,21,23,26,27,29,30,32,36,37,41 }; static const int tr[] = { 0,1,2,3,16,19,20,22,24,25,28,31,33,34,35,38,39,40,42,43,100,-1 };
    for (int k = 0; k < 24; ++k) { g_mode = f[k]; if (i.IsHeroControlledByPlayer()) { std::printf("0088e980_TEST FAIL\n"); return 1; } } for (int k = 0; k < 22; ++k) { g_mode = tr[k]; if (!i.IsHeroControlledByPlayer()) { std::printf("0088e980_TEST FAIL\n"); return 1; } }
    std::printf("0088e980_TEST PASS\n"); return 0;
}
