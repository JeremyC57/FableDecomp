// test for 004496b0 CPlayer mode predicate
#include <stdio.h>
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
struct CPlayer_Methods : CPlayer {
    bool IsInMode17Or18() const;
};
bool CPlayer_Methods::IsInMode17Or18() const
{
    return GetCurrentMode() == PLAYER_MODE_FREEZE_CONTROLS || GetCurrentMode() == PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS;
}

static int g_mode; static int g_calls;
EPlayerMode CPlayer::GetCurrentMode() const { ++g_calls; return (EPlayerMode)g_mode; }   // stub of 0x00633BE0
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