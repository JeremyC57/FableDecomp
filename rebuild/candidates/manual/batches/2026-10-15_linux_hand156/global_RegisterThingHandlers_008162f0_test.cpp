// Behaviour test for 0x008162F0: handler registration: four (id, member handler) pairs go into the global map in order
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Thing { int hits; void H1() { hits += 1; } void H2() { hits += 10; } void H3() { hits += 100; } void H4() { hits += 1000; } };
typedef void (Thing::*PH)();
struct Map { long keys[8]; PH vals[8]; int n; Map() : n(0) {}
    bool insert(long k, PH v) { for (int i = 0; i < n; ++i) if (keys[i] == k) return false; keys[n] = k; vals[n] = v; ++n; return true; } };
static Map g_map;
static void Register() { g_map.insert(1, &Thing::H1); g_map.insert(2, &Thing::H2); g_map.insert(3, &Thing::H3); g_map.insert(4, &Thing::H4); }
static void Run()
{
    Register();
    CHECK(g_map.n == 4 && g_map.keys[0] == 1 && g_map.keys[3] == 4);
    Thing t; t.hits = 0;
    for (int i = 0; i < g_map.n; ++i) (t.*g_map.vals[i])();
    CHECK(t.hits == 1111);
    Register(); CHECK(g_map.n == 4);   // duplicates are rejected by insert
}

int main()
{
    Run();
    if (g_fail) { std::printf("008162f0_TEST FAIL\n"); return 1; }
    std::printf("008162f0_TEST PASS\n");
    return 0;
}
