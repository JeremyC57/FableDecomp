// Behaviour test for 0x0079A4D0: implicit destructor: destroys the vector's elements, frees the buffer, then the pointers and the base
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static int g_order[16]; static int g_n = 0;
struct SPtr { int Id; SPtr() : Id(0) {} ~SPtr() { g_order[g_n++] = Id; } };
struct SBase { ~SBase() { g_order[g_n++] = 100; } };
struct SShopKeeper : SBase { SPtr Customer; SPtr Shop; SPtr Previous[2]; };
int main()
{
    {
        SShopKeeper k; k.Customer.Id = 1; k.Shop.Id = 2; k.Previous[0].Id = 3; k.Previous[1].Id = 4;
    }
    CHECK(g_n == 5);
    CHECK(g_order[0] == 4 && g_order[1] == 3 && g_order[2] == 2 && g_order[3] == 1 && g_order[4] == 100);
    if (!g_fail) printf("0079a4d0_TEST PASS\n");
    return g_fail;
}