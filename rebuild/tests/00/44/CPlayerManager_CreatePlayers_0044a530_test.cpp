#include <cstdio>
class CMainGameComponent;
class CGameDefinitionManager;
class CPlayer;
class CPlayerManager {
public:
    void*                   _vfptr_or_pad;               // +0x00
    CMainGameComponent*     Component;                   // +0x04
    CGameDefinitionManager* DefinitionManager;           // +0x08
    CPlayer**               Players[3];                  // +0x0c STLport vector<CPlayer*>
    long                    PlayerNeutral;               // +0x18
    long                    MainPlayer;                  // +0x1c
    void CreatePlayers();
private:
    void CreatePlayer(long player_number);               // 0x0044A1A0
    friend struct CPlayerManager_Test;                   // behaviour-test hook (CreatePlayer is private)
};
void CPlayerManager::CreatePlayers()
{
    MainPlayer = 0;
    for (long i = 0; i < 4; ++i)
        CreatePlayer(i);
    PlayerNeutral = 4;
    CreatePlayer(4);
}

static long g_calls[8]; static int g_n;
void CPlayerManager::CreatePlayer(long n) { g_calls[g_n++] = n; if (MainPlayer != 0) g_n = 100; }
struct CPlayerManager_Test {
    static int run() {
        CPlayerManager m; m.PlayerNeutral = 9; m.MainPlayer = 7;
        m.CreatePlayers();
        if (g_n != 5) return 0;
        for (int i = 0; i < 5; ++i) if (g_calls[i] != i) return 0;
        return m.MainPlayer == 0 && m.PlayerNeutral == 4;
    }
};
int main() { if (CPlayerManager_Test::run()) std::printf("0044a530_TEST PASS\n"); else std::printf("FAIL\n"); return 0; }