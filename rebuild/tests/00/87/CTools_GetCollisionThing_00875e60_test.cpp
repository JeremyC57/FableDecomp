// Behaviour test for 0x00875e60: CTools::GetCollisionThing: first meshed thing whose mesh the segment hits, its hit position written out
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct V { float x, y, z; };
struct Thing { bool hasMesh; bool hit; V where; };
static Thing* Get(Thing** b, Thing** e, V& out) { for (Thing** it = b; it != e; ++it) { Thing* t = *it; if (t->hasMesh) { if (t->hit) { out = t->where; return *it; } } } return 0; }
static void run() { Thing a = { false, true, { 1, 1, 1 } }, b = { true, false, { 2, 2, 2 } }, c = { true, true, { 3, 4, 5 } }; Thing* v[3] = { &a, &b, &c }; V out = { 0, 0, 0 };
  CHECK(Get(v, v + 3, out) == &c && out.x == 3 && out.z == 5); CHECK(Get(v, v + 2, out) == 0); }

int main()
{
    run();
    if (g_fail) { std::printf("00875e60_TEST FAIL\n"); return 1; }
    std::printf("00875e60_TEST PASS\n");
    return 0;
}