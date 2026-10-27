// Behaviour test for 0x00787BFE: ManualDeactivate clears Active and resets the trap
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct STrap { bool Active; int Resets; void Reset() { ++Resets; } void ManualDeactivate() { Active = false; Reset(); } };
int main()
{
    STrap t; t.Active = true; t.Resets = 0;
    t.ManualDeactivate();
    CHECK(!t.Active && t.Resets == 1);
    if (!g_fail) printf("00787bfe_TEST PASS\n");
    return g_fail;
}