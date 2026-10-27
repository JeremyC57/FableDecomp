// Behaviour test for 0x007EF6F0: Copy takes the decapitated flag from the other component
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SDecap { bool Decapitated; void Copy(const SDecap& o) { Decapitated = o.Decapitated; } };
int main()
{
    SDecap a, b; a.Decapitated = false; b.Decapitated = true;
    a.Copy(b); CHECK(a.Decapitated);
    b.Decapitated = false; a.Copy(b); CHECK(!a.Decapitated);
    if (!g_fail) printf("007ef6f0_TEST PASS\n");
    return g_fail;
}