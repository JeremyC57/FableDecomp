// Behaviour test for 0x00557ca0 (CKeyRedefiner::GetSubTypeForAction): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_next38 = 100, g_next37 = 200;
struct CActionInputControl { long GameAction; long direction; long GetDirection() const { return direction; } };
static long GetSubTypeForAction(const CActionInputControl& c)
{
    switch (c.GameAction) {
    case 0x37: return ++g_next37;
    case 0x38: return ++g_next38;
    case 0x3c:
        switch (c.GetDirection()) { case 1: return 10; case 2: return 11; case 3: return 12; case 4: return 13; }
        break;
    }
    return 0;
}
static void Run()
{
    CActionInputControl c; c.direction = 0;
    c.GameAction = 0x37; CHECK(GetSubTypeForAction(c) == 201 && GetSubTypeForAction(c) == 202);
    c.GameAction = 0x38; CHECK(GetSubTypeForAction(c) == 101);
    c.GameAction = 0x3c; for (long d = 1; d <= 4; ++d) { c.direction = d; CHECK(GetSubTypeForAction(c) == 9 + d); }
    c.direction = 5; CHECK(GetSubTypeForAction(c) == 0); c.direction = 0; CHECK(GetSubTypeForAction(c) == 0);
    c.GameAction = 0x39; CHECK(GetSubTypeForAction(c) == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00557ca0_TEST FAIL\n"); return 1; }
    std::printf("00557ca0_TEST PASS\n");
    return 0;
}
