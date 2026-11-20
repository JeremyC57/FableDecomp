// Behaviour test for 0x012297F0: the atexit thunk destroys BUILDING_BOUNDRY exactly once (mock out-of-line CCharString members).
#include <cstdio>
#include "fable_string.h"
static int g_ctors = 0, g_dtors = 0;
CCharString::CCharString() { ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
extern const CCharString BUILDING_BOUNDRY;
const CCharString BUILDING_BOUNDRY;
void __fastcall GlobalMethodTailThunk_012297f0()
{
    BUILDING_BOUNDRY.CCharString::~CCharString();
}
int main()
{
    int before = g_dtors;
    GlobalMethodTailThunk_012297f0();
    if (g_ctors == 1 && g_dtors == before + 1) { std::printf("AUTO_TINY_012297f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_012297f0_TEST FAIL\n");
    return 1;
}
