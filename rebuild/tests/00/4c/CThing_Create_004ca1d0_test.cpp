// Behaviour test for 0x004CA1D0 CThing::Create(defIndex, player, map, persistent): asks the manager (flag false), allocates
// and constructs a type-15 thing, forwards all four parameters to ConstructFromParams and returns the thing only when that
// succeeds; a refused allocation or a failed initialisation gives null (the thing is not deleted).
#include <cstdio>
#include <cstdlib>
#include <new>

static bool g_accept = true, g_initOk = true, g_lastFlag = true;
static int g_ctors = 0;
struct Thing {
    unsigned long type; long a, b, c; bool d;
    explicit Thing(unsigned long t) : type(t) { ++g_ctors; }
    bool ConstructFromParams(long x, long y, long z, bool p) { a = x; b = y; c = z; d = p; return g_initOk; }
};
static void* ThingNew(unsigned size, bool flag) { g_lastFlag = flag; return g_accept ? std::malloc(size) : 0; }

static Thing* Create(long x, long y, long z, bool p)
{
    void* mem = ThingNew(sizeof(Thing), false);
    Thing* t = mem ? new (mem) Thing(15) : 0;
    if (t && t->ConstructFromParams(x, y, z, p)) return t;
    return 0;
}

int main()
{
    Thing* t = Create(7, 2, 30, true);
    if (!t || t->type != 15 || t->a != 7 || t->b != 2 || t->c != 30 || !t->d || g_lastFlag || g_ctors != 1) {
        std::printf("004ca1d0_TEST FAIL\n"); return 1;
    }
    g_initOk = false;
    if (Create(1, 1, 1, false) != 0 || g_ctors != 2) { std::printf("004ca1d0_TEST FAIL\n"); return 1; }
    g_accept = false;
    if (Create(1, 1, 1, false) != 0 || g_ctors != 2) { std::printf("004ca1d0_TEST FAIL\n"); return 1; }
    std::printf("004ca1d0_TEST PASS\n");
    return 0;
}