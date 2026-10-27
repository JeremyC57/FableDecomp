#include <cstdio>
#include <cstring>
// Behaviour model: loading a context without the names gives the defaults; saving then loading round-trips.
struct Ctx { bool loading; const char* names[8]; long vals[8]; int n;
    template <class T> void Transfer(const char* name, T& v, const T& def) {
        if (!loading) { names[n] = name; vals[n++] = (long)v; return; }
        for (int i = 0; i < n; ++i) if (!std::strcmp(names[i], name)) { v = (T)vals[i]; return; }
        v = def; } };
int main()
{
    Ctx save = { false }; save.n = 0; bool a = true; long w = 5; save.Transfer("CompletedKnotholePunchClub", a, false); save.Transfer("W", w, 0L);
    Ctx load = save; load.loading = true; bool b = false; long x = 0; bool c = true;
    load.Transfer("CompletedKnotholePunchClub", b, false); load.Transfer("W", x, 0L); load.Transfer("missing", c, false);
    if (!b || x != 5 || c) { std::printf("00e82190_TEST FAIL\n"); return 1; }
    std::printf("00e82190_TEST PASS\n");
    return 0;
}