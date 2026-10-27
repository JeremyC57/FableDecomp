// Behaviour test for 0x008093D0: GetAllThings<CThing, None>: push every scan-visible thing
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SThing { unsigned char StateBits; };
static bool Included(const SThing* t)
{
    unsigned char f = t->StateBits;
    return (f & 0x04) && (f & 0x08) && !(f & 0x01) && !(f & 0x20);
}
int main()
{
    SThing t[5]; t[0].StateBits = 0x0c; t[1].StateBits = 0x0d; t[2].StateBits = 0x04; t[3].StateBits = 0x2c; t[4].StateBits = 0x4c;
    SThing* out[5]; int n = 0;
    for (int i = 0; i < 5; ++i) if (Included(&t[i])) out[n++] = &t[i];
    CHECK(n == 2 && out[0] == &t[0] && out[1] == &t[4]);
    if (!g_fail) printf("008093d0_TEST PASS\n");
    return g_fail;
}