// Behaviour test for 0x007D1AA0: implicit destructor: every coin is destroyed through its virtual destructor, then the tavern-game base
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static int g_coins = 0, g_base = 0;
struct SCoin { virtual ~SCoin() { ++g_coins; } long Owner; };
struct STavernGame { virtual ~STavernGame() { ++g_base; } };
struct SCoinBase : STavernGame { SCoin Coins[3]; SCoin* Held; };
int main()
{
    { SCoinBase g; g.Held = &g.Coins[1]; }
    CHECK(g_coins == 3 && g_base == 1);
    STavernGame* p = new SCoinBase; delete p;
    CHECK(g_coins == 6 && g_base == 2);
    if (!g_fail) printf("007d1aa0_TEST PASS\n");
    return g_fail;
}