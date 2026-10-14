#include <cstdio>
#include <cstring>
#include <new>

static long g_removed = -1; static int g_removes;
struct CPlayer { long m_mode; long GetCurrentMode() { return m_mode; } void RemoveMode(long mode) { g_removed = mode; ++g_removes; } };
struct CPlayerManager { CPlayer m_player; CPlayer* GetMainPlayer() { return &m_player; } };
struct CGameScriptInterface { void* vt; void* a; void* b; void* c; void* d; CPlayerManager* PlayerManager;
    void HeroStopDigging() { CPlayer* player = PlayerManager->GetMainPlayer(); long mode = player->GetCurrentMode(); if (mode == 0x1e) player->RemoveMode(mode); } };
int main()
{
    CPlayerManager pm; CGameScriptInterface si; si.PlayerManager = &pm;
    pm.m_player.m_mode = 5; si.HeroStopDigging(); if (g_removes != 0) { std::printf("0088dfa0_TEST FAIL\n"); return 1; }
    pm.m_player.m_mode = 0x1e; si.HeroStopDigging(); if (g_removes != 1 || g_removed != 0x1e) { std::printf("0088dfa0_TEST FAIL\n"); return 1; }
    std::printf("0088dfa0_TEST PASS\n");
    return 0;
}
