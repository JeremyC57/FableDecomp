#include <cstdio>
#include <cstring>
static char g_seq[32] = ""; static long g_args[3]; static float* g_time = 0;
struct CPlayer { long m_number; long GetPlayerNumber() { std::strcat(g_seq, "N"); return m_number; } };
struct CGamePlayerInterface { bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber, float* pTime) { std::strcat(g_seq, "I"); g_args[0] = action; g_args[1] = buttonEventType; g_args[2] = playerNumber; g_time = pTime; return playerNumber == 2; } };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; CGamePlayerInterface* PGamePlayerInterface;
    bool IsGameActionEventInQueue(long action, long buttonEventType, float* pTime) { return PGamePlayerInterface->IsGameActionEventInQueue(action, buttonEventType, MyPlayer->GetPlayerNumber(), pTime); } };
int main()
{
    CPlayer player; player.m_number = 2; CGamePlayerInterface gpi; CInputProcessGameBase b; b.MyPlayer = &player; b.PGamePlayerInterface = &gpi; float t = 0.0f;
    if (!b.IsGameActionEventInQueue(7, 3, &t) || std::strcmp(g_seq, "NI") || g_args[0] != 7 || g_args[1] != 3 || g_args[2] != 2 || g_time != &t) { std::printf("00687d80_TEST FAIL\n"); return 1; }
    player.m_number = 1; if (b.IsGameActionEventInQueue(7, 3, 0) || g_time != 0) { std::printf("00687d80_TEST FAIL\n"); return 1; }
    std::printf("00687d80_TEST PASS\n");
    return 0;
}