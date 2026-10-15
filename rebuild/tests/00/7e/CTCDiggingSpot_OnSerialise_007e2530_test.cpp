// Behaviour test for CTCDiggingSpot::OnSerialise(CPersistContext&, bool) @ 0x007E2530: "Enabled" (default true) only on
// a full transfer, "Hidden" (default false) always.
#include <cstdio>
#include <cstring>
static int g_transfers = 0;
static const char* g_names[4];
static bool g_defaults[4];
static bool* g_values[4];
struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue)
    { g_names[g_transfers] = name; g_defaults[g_transfers] = defaultValue; g_values[g_transfers] = &value; ++g_transfers; }
};
struct CTCDiggingSpot {
    virtual void v00() { }
    unsigned char _base[0x8];
    bool          Enabled;
    bool          Hidden;
    void OnSerialise(CPersistContext& context, bool fullState);
};
void CTCDiggingSpot::OnSerialise(CPersistContext& context, bool fullState)
{
    if (fullState)
        context.Transfer("Enabled", Enabled, true);
    context.Transfer("Hidden", Hidden, false);
}
int main()
{
    CTCDiggingSpot s; CPersistContext c;
    s.OnSerialise(c, true);
    if (g_transfers != 2 || std::strcmp(g_names[0], "Enabled") != 0 || g_defaults[0] != true || g_values[0] != &s.Enabled
        || std::strcmp(g_names[1], "Hidden") != 0 || g_defaults[1] != false || g_values[1] != &s.Hidden) { std::printf("007e2530_TEST FAIL\n"); return 1; }
    g_transfers = 0;
    s.OnSerialise(c, false);
    if (g_transfers != 1 || std::strcmp(g_names[0], "Hidden") != 0) { std::printf("007e2530_TEST FAIL\n"); return 1; }
    std::printf("007e2530_TEST PASS\n");
    return 0;
}