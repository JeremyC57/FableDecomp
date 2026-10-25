// Behaviour test for 0x0099CFE0 CSystemRegistry::GetBOOL (model): GetSLONG's value (or the default) as 0/1.
#include <cstdio>
static long g_stored = 0; static bool g_present = false;
struct Registry {
    long GetSLONG(const char*, long def) const { return g_present ? g_stored : def; }
    int GetBOOL(const char* name, int def) const { return GetSLONG(name, def) != 0; }
};
int main()
{
    Registry r;
    int ok = r.GetBOOL("x", 7) == 1 && r.GetBOOL("x", 0) == 0;
    g_present = true; g_stored = 42;
    ok &= r.GetBOOL("x", 0) == 1;
    g_stored = 0;
    ok &= r.GetBOOL("x", 1) == 0;
    std::printf(ok ? "0099cfe0_TEST PASS\n" : "0099cfe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
