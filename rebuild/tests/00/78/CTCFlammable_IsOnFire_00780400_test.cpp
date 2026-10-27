// Behaviour test for 0x00780400: IsOnFire: fire resistance used up (<= 0)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SFlammable { long FireResistance; bool IsOnFire() const { return FireResistance <= 0; } };
int main()
{
    SFlammable f; f.FireResistance = 10; CHECK(!f.IsOnFire());
    f.FireResistance = 0; CHECK(f.IsOnFire());
    f.FireResistance = -5; CHECK(f.IsOnFire());
    if (!g_fail) printf("00780400_TEST PASS\n");
    return g_fail;
}