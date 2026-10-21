// test for 00446b10 IsGameActionEventInQueue (3-arg)
#include <stdio.h>
#include "engine/CGamePlayerInterface.h"
enum EGameAction { GAME_ACTION_NULL };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_NULL };
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player, float& value) const;  // 0x00445CB0
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player) const;
};
bool CGamePlayerInterface_Methods::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player) const
{
    float value = 0.0f;
    return IsGameActionEventInQueue(action, event_type, player, value);
}

static int g_a, g_t; static long g_p; static float g_v; static const void* g_this;
bool CGamePlayerInterface_Methods::IsGameActionEventInQueue(EGameAction a, EInputButtonEventType t, long p, float& v) const
{ g_a = a; g_t = t; g_p = p; g_v = v; g_this = this; v = 2.5f; return a == 7; }
int main()
{
    static double buf[0x400];
    const CGamePlayerInterface_Methods* gi = (const CGamePlayerInterface_Methods*)buf;
    int ok = gi->IsGameActionEventInQueue((EGameAction)7, (EInputButtonEventType)3, 1L) == true
          && g_a == 7 && g_t == 3 && g_p == 1 && g_v == 0.0f && g_this == gi;
    ok &= gi->IsGameActionEventInQueue((EGameAction)6, (EInputButtonEventType)3, 0L) == false;
    printf(ok ? "00446b10_TEST PASS\n" : "00446b10_TEST FAIL\n");
    return ok ? 0 : 1;
}