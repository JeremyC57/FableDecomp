// test for 00446b10 IsGameActionEventInQueue (3-arg) -- NF171 2026-11-20: on engine/CGamePlayerInterface.h (retail path), links the row;
// the four-argument overload 0x00445CB0 is stubbed. The row passes its three arguments and a zeroed float through, and returns the result.
#include <stdio.h>
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES
#include "engine/CGamePlayerInterface.h"

static int g_a, g_t; static long g_p; static float g_v; static const void* g_this;
bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction a, EInputButtonEventType t, long p, float& v) const
{ g_a = a; g_t = t; g_p = p; g_v = v; g_this = this; v = 2.5f; return a == 7; }
int main()
{
    static double buf[0x400];
    const CGamePlayerInterface* gi = (const CGamePlayerInterface*)buf;
    int ok = gi->IsGameActionEventInQueue((EGameAction)7, (EInputButtonEventType)3, 1L) == true
          && g_a == 7 && g_t == 3 && g_p == 1 && g_v == 0.0f && g_this == gi;
    ok &= gi->IsGameActionEventInQueue((EGameAction)6, (EInputButtonEventType)3, 0L) == false;
    printf(ok ? "00446b10_TEST PASS\n" : "00446b10_TEST FAIL\n");
    return ok ? 0 : 1;
}
