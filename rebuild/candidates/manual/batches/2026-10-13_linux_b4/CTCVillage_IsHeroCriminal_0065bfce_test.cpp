#include <cstdio>

struct Obs { int n; void OnHeroCriminalStatusChanged(long) { ++n; } };
static Obs g_obs; Obs* g_pCriminalStatusObserver = &g_obs;
struct V { unsigned char pad[0xe4]; bool Current; long fine;
    long GetFine() { return fine; }
    bool Is() { bool c = GetFine() > 0; if (Current != c) { g_pCriminalStatusObserver->OnHeroCriminalStatusChanged(0); Current = c; } return Current; } };
int main()
{
    V v; v.Current = false; v.fine = 0; g_obs.n = 0; if (v.Is() || g_obs.n) { std::printf("0065bfce_TEST FAIL\n"); return 1; }
    v.fine = 5; if (!v.Is() || g_obs.n != 1) { std::printf("0065bfce_TEST FAIL\n"); return 1; }
    if (!v.Is() || g_obs.n != 1) { std::printf("0065bfce_TEST FAIL\n"); return 1; }
    std::printf("0065bfce_TEST PASS\n");
    return 0;
}
