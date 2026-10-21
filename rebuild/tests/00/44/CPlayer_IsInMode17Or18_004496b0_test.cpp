// test for 004496b0 CPlayer mode predicate
#include <stdio.h>
#include "engine/CPlayer.h"
enum EPlayerMode { PLAYER_MODE_17 = 0x11, PLAYER_MODE_18 = 0x12 };
struct CPlayer_Methods : CPlayer {
    EPlayerMode GetCurrentMode() const;
    bool IsInMode17Or18() const;
};
bool CPlayer_Methods::IsInMode17Or18() const
{
    return GetCurrentMode() == PLAYER_MODE_17 || GetCurrentMode() == PLAYER_MODE_18;
}

static int g_mode; static int g_calls;
EPlayerMode CPlayer_Methods::GetCurrentMode() const { ++g_calls; return (EPlayerMode)g_mode; }
int main()
{
    static double buf[0x1000];
    CPlayer_Methods* p = (CPlayer_Methods*)buf;
    g_mode = 0x11; int ok = p->IsInMode17Or18() == true && g_calls == 1;
    g_mode = 0x12; g_calls = 0; ok &= p->IsInMode17Or18() == true && g_calls == 2;
    g_mode = 0x10; g_calls = 0; ok &= p->IsInMode17Or18() == false && g_calls == 2;
    g_mode = 0; ok &= p->IsInMode17Or18() == false;
    printf(ok ? "004496b0_TEST PASS\n" : "004496b0_TEST FAIL\n");
    return ok ? 0 : 1;
}