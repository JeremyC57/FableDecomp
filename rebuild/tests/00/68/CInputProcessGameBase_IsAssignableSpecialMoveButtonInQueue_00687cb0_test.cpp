#include <cstdio>
// CInputProcessGameBase::IsAssignableSpecialMoveButtonInQueue: forward (a, b, MyPlayer->GetPlayerNumber()) to the player interface
static long g_a, g_b, g_player, g_calls;
class CPlayer { public: long number; long GetPlayerNumber() { ++g_calls; return number; } };
class CGamePlayerInterface { public:
    bool result;
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType, long playerNumber) { g_a = button; g_b = buttonEventType; g_player = playerNumber; return result; }
};
struct CInputProcessGameBase {
    void* vtable; long pad[4]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; CGamePlayerInterface* PGamePlayerInterface;
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType) { return PGamePlayerInterface->IsAssignableSpecialMoveButtonInQueue(button, buttonEventType, MyPlayer->GetPlayerNumber()); }
};
int main()
{
    CPlayer player; player.number = 2;
    CGamePlayerInterface gpi; gpi.result = true;
    CInputProcessGameBase p; p.MyPlayer = &player; p.PGamePlayerInterface = &gpi;
    bool r1 = p.IsAssignableSpecialMoveButtonInQueue(17, 3);
    bool ok = r1 && g_a == 17 && g_b == 3 && g_player == 2 && g_calls == 1;
    gpi.result = false; player.number = 0;
    bool r2 = p.IsAssignableSpecialMoveButtonInQueue(5, 1);
    ok = ok && !r2 && g_a == 5 && g_b == 1 && g_player == 0 && g_calls == 2;
    if (!ok) { std::printf("00687cb0_TEST FAIL\n"); return 1; }
    std::printf("00687cb0_TEST PASS\n");
    return 0;
}