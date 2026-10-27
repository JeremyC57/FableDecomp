// Behaviour test for 0x00787BF1: Reset closes the trap opener (state + timer) and calls the derived ResetTrap
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SOpener { long State; long Timer; void Close() { State = 2; Timer = 0; } };
struct STrapBase { SOpener TrapOpener; int ResetCalls;
    virtual ~STrapBase() {}
    virtual void ResetTrap() = 0;
    void Reset() { TrapOpener.Close(); ResetTrap(); } };
struct STrap : STrapBase { void ResetTrap() { ++ResetCalls; } };
int main()
{
    STrap t; t.TrapOpener.State = 1; t.TrapOpener.Timer = 77; t.ResetCalls = 0;
    t.Reset();
    CHECK(t.TrapOpener.State == 2 && t.TrapOpener.Timer == 0 && t.ResetCalls == 1);
    if (!g_fail) printf("00787bf1_TEST PASS\n");
    return g_fail;
}