// Behaviour test for 0x00780800: AddFireDamage subtracts damage and smoulders while resistance is left
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SFirePoint { long Resistance; int Smoulders;
    void SetAsSmoldering() { ++Smoulders; }
    void AddFireDamage(long d) { Resistance -= d; if (Resistance > 0) SetAsSmoldering(); } };
int main()
{
    SFirePoint p; p.Resistance = 10; p.Smoulders = 0;
    p.AddFireDamage(3); CHECK(p.Resistance == 7 && p.Smoulders == 1);
    p.AddFireDamage(7); CHECK(p.Resistance == 0 && p.Smoulders == 1);
    p.AddFireDamage(-2); CHECK(p.Resistance == 2 && p.Smoulders == 2);
    if (!g_fail) printf("00780800_TEST PASS\n");
    return g_fail;
}