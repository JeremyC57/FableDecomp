// Behaviour test for 0x00CFEE30 COracleTablet::RemoveCursedTablets: the first cursed tablet the hero carries (1..8)
// is taken from him, unless the thread is terminating; nothing happens when he carries none.
#include <cstdio>
#include <cstring>
static int g_has[9];
static int g_taken;
static int g_queries;
static bool g_term;
struct Tablet {
    bool Has(int n) { ++g_queries; return g_has[n] != 0; }
    bool IsActiveThreadTerminating() { return g_term; }
    void Take(int n) { g_taken = n; }
    void RemoveCursedTablets()
    {
        for (int n = 1; n <= 8; ++n) {
            if (Has(n)) {
                if (IsActiveThreadTerminating()) return;
                Take(n);
                return;
            }
        }
    }
};
static int Run(int carried, bool term)
{
    std::memset(g_has, 0, sizeof(g_has));
    if (carried) g_has[carried] = 1;
    if (carried == 3) g_has[6] = 1;
    g_taken = 0; g_queries = 0; g_term = term;
    Tablet t; t.RemoveCursedTablets();
    return g_taken;
}
int main()
{
    int bad = 0;
    if (Run(0, false) != 0 || g_queries != 8) bad++;
    if (Run(1, false) != 1 || g_queries != 1) bad++;
    if (Run(3, false) != 3 || g_queries != 3) bad++;
    if (Run(8, false) != 8 || g_queries != 8) bad++;
    if (Run(5, true) != 0 || g_queries != 5) bad++;
    if (bad) { std::printf("00cfee30_TEST FAIL\n"); return 1; }
    std::printf("00cfee30_TEST PASS\n"); return 0;
}