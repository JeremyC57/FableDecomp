#include <cstdio>
// CInputProcessGameBase::IsGameActionEventInQueue: forward (a, b, MyPlayer->GetPlayerNumber()) to the player interface
static long g_a, g_b, g_player, g_calls;
struct CPlayer { long number; long GetPlayerNumber() { ++g_calls; return number; } };
struct CGamePlayerInterface {
    bool result;
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber) { g_a = action; g_b = buttonEventType; g_player = playerNumber; return result; }
};
struct CInputProcessGameBase {
    void* vtable; long pad[4]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; CGamePlayerInterface* PGamePlayerInterface;
    bool IsGameActionEventInQueue(long action, long buttonEventType) { return PGamePlayerInterface->IsGameActionEventInQueue(action, buttonEventType, MyPlayer->GetPlayerNumber()); }
};
int main()
{
    CPlayer player; player.number = 2;
    CGamePlayerInterface gpi; gpi.result = true;
    CInputProcessGameBase p; p.MyPlayer = &player; p.PGamePlayerInterface = &gpi;
    bool r1 = p.IsGameActionEventInQueue(17, 3);
    bool ok = r1 && g_a == 17 && g_b == 3 && g_player == 2 && g_calls == 1;
    gpi.result = false; player.number = 0;
    bool r2 = p.IsGameActionEventInQueue(5, 1);
    ok = ok && !r2 && g_a == 5 && g_b == 1 && g_player == 0 && g_calls == 2;
    if (!ok) { std::printf("00687d50_TEST FAIL\n"); return 1; }
    std::printf("00687d50_TEST PASS\n");
    return 0;
}
