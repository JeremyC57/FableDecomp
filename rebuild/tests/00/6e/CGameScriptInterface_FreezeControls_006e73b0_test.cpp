#include <cstdio>
#include <cstring>
#include <new>

class CThing { public: unsigned char _pad[0x91]; unsigned char m_stateFlags; };
static long g_frozenPlayer = -1; static int g_calls;
class CPlayer { public: CThing* m_creature; CThing* GetControlledCreature() { return m_creature; } };
class CPlayerManager { public: CPlayer m_player; CPlayer* GetMainPlayer() { return &m_player; } void Toggle(long player) { g_frozenPlayer = player; ++g_calls; } };
class CGameScriptInterface { public:
    void* vt; void* World; void* Component; void* DisplayEngine; void* DefinitionManager; CPlayerManager* PlayerManager; void* ThingSearchTools; long CurrentPlayer;
    void Run() { CThing* creature = PlayerManager->GetMainPlayer()->GetControlledCreature();
        if (creature && !(creature->m_stateFlags & 1)) PlayerManager->Toggle(CurrentPlayer); }
};
int main()
{
    CPlayerManager pm; CThing thing; std::memset(&thing, 0, sizeof thing); pm.m_player.m_creature = 0;
    CGameScriptInterface si; si.PlayerManager = &pm; si.CurrentPlayer = 3;
    si.Run(); if (g_calls != 0) { std::printf("006e73b0_TEST FAIL\n"); return 1; }                                            // no creature
    pm.m_player.m_creature = &thing; thing.m_stateFlags = 1; si.Run(); if (g_calls != 0) { std::printf("006e73b0_TEST FAIL\n"); return 1; }   // bit 0 set
    thing.m_stateFlags = 2; si.Run(); if (g_calls != 1 || g_frozenPlayer != 3) { std::printf("006e73b0_TEST FAIL\n"); return 1; }
    std::printf("006e73b0_TEST PASS\n");
    return 0;
}