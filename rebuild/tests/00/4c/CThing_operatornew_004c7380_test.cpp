// Behaviour test for 0x004C7380 CThing::operator new(size, flag): the thing manager is asked with the flag; on yes the
// global operator new supplies `size` bytes, on no the result is null and nothing is allocated.
#include <cstdio>
#include <cstdlib>

static int g_allocs = 0; static bool g_lastFlag = false; static bool g_accept = true;
struct ThingManager { bool CanAllocateThing(bool f) { g_lastFlag = f; return g_accept; } };
static ThingManager g_tm; static ThingManager* g_pThingManager = &g_tm;
static void* GlobalNew(unsigned size) { ++g_allocs; return std::malloc(size); }

static void* ThingNew(unsigned size, bool flag)
{
    if (g_pThingManager->CanAllocateThing(flag)) return GlobalNew(size);
    return 0;
}

int main()
{
    void* p = ThingNew(0x98, true);
    if (!p || g_allocs != 1 || !g_lastFlag) { std::printf("004c7380_TEST FAIL\n"); return 1; }
    g_accept = false;
    void* q = ThingNew(0x98, false);
    if (q || g_allocs != 1 || g_lastFlag) { std::printf("004c7380_TEST FAIL\n"); return 1; }
    std::free(p);
    std::printf("004c7380_TEST PASS\n");
    return 0;
}