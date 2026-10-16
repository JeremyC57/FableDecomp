// Behaviour test for 0x00cbe245: IsThingOutOfAction: dead, killed, knocked out or unconscious
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Thing { bool alive, killed, ko, unconscious; };
static bool Out(const Thing& t) { char result = !t.alive || t.killed || t.ko || t.unconscious; return result != 0; }
static void run() { Thing a = { true, false, false, false }; CHECK(!Out(a)); a.unconscious = true; CHECK(Out(a)); Thing b = { false, false, false, false }; CHECK(Out(b)); Thing c = { true, true, false, false }; CHECK(Out(c)); Thing d = { true, false, true, false }; CHECK(Out(d)); }

int main()
{
    run();
    if (g_fail) { std::printf("00cbe245_TEST FAIL\n"); return 1; }
    std::printf("00cbe245_TEST PASS\n");
    return 0;
}
